// -*- C++ -*-
//
// Package:     Suez
// Module:      SuezMain
// 
// Description: The main "Main" in the Suez World
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Martin Lohner
// Created:     Thu Mar 20 15:13:11 EST 1997
// $Id: SuezMain.cc,v 1.72 2003/04/25 15:42:19 vk Exp $
//
// Revision history (at end of file)
//
#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#include "C++Std/fstream.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdexcept>
#if defined(XALLOC_NON_STANDARD_EXCEPTION_BUG)
#include <exception.h>
#endif

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <deque>
#include <map>
#endif

// user include files
#include "Suez/testBoolean.h"
#include "Suez/forceLinkStorageManagement.h"
#include "Experiment/report.h"
#include "DataHandler/FrameLogger.h"

#include "JobControl/JobControl.h"
#include "Suez/UserApplication.h"
#include "JobControl/ReportModule.h"
#include "Interpreter/TclInterpreter.h"
#include "Utility/StringTokenizer.h"

// signal handling
#include "Signal/SignalHandler.h"
#include "Signal/SIGINT_Handler.h"

// exceptions
#include "DAException/DAException.h"

// STL includes
#include <deque>


int TheMain( int argc, char** argv );
DABoolean SuezInit( Interpreter& interpreter, 
		    const string& runFile = "",
		    DABoolean runSetupFile = false );

// typedefs, enums, constants
#include "Suez/externConst.h"
//static const char* const kFacilityString = "Suez.SuezMain";

static const char* const IdString
= "$Id: SuezMain.cc,v 1.72 2003/04/25 15:42:19 vk Exp $";
static const char* const TagString
= "$Name:  $";
static const char* const VersionString
= "$Version$";


static string welcomeMsg = 
string( "// -----------------------------------------------------------\n" )+
string( "//  Welcome to Suez, the CLEO III data access program         \n" )+
string( "// -----------------------------------------------------------\n" );

//-----------------
//    Main
//-----------------

int
main( int argc, char** argv )
{
   // install SignalHandlers
   DABoolean callExit;
   SIGINT_Handler sigint_handler( callExit=true );
   SignalHandler::instance()->registerHandler( SignalHandler::kSigInt, 
					       &sigint_handler );
   //only if an exception is not thrown will this value be set to 0
   int returnValue = 1;
#ifndef CLEO_DEBUG
   // catch all (uncaught) exceptions and print message
   // catch first specific ones, then less specific
   try {
#endif

      returnValue = TheMain( argc, argv );

#ifndef CLEO_DEBUG
   }
   // all specific logic_error exceptions
   catch( length_error& thisException )
   {
      cerr << "ERROR: Suez caught \"length_error\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
   catch( domain_error& thisException )
   {
      cerr << "ERROR: Suez caught \"domain_error\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
   catch( out_of_range& thisException )
   {
      cerr << "ERROR: Suez caught \"out_of_range\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
   catch( invalid_argument& thisException )
   {
      cerr << "ERROR: Suez caught \"invalid_argument\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
   // now catch all other logic_error exceptions
   catch( logic_error& thisException )
   {
      cerr << "ERROR: Suez caught \"logic_error\" exception:\n\"" 
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
   // now all non-logic_error, non-runtime_error exceptions
#if !defined(UNDEF_BAD_ALLOC_EXCEPT_BUG)
   catch( bad_alloc& thisException )
   {
      cerr << "ERROR: Suez caught \"bad_alloc\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
#else
#if defined(XALLOC_NON_STANDARD_EXCEPTION_BUG)
   catch( xalloc& thisException ) 
   {
      cerr << "ERROR: Suez caught \"xalloc\" exception:\n\""
	   << thisException.why() << "\"\n ... exiting program" << endl;
   }
#endif
#endif
#if !defined(UNDEF_BAD_EXCEPTION_EXCEPT_BUG)
   catch( bad_exception& thisException )
   {
      cerr << "ERROR: Suez caught \"bad_exception\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
#endif
#if !defined(UNDEF_IOS_BASE_FAILURE_EXCEPT_BUG)
   catch( ios_base::failure& thisException )
   {
      cerr << "ERROR: Suez caught \"ios_base::failure\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
#endif
#if !defined(UNDEF_BAD_TYPEID_EXCEPT_BUG)
   catch( bad_typeid& thisException )
   {
      cerr << "ERROR: Suez caught \"bad_typeid\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
#endif
#if !defined(UNDEF_BAD_CAST_EXCEPT_BUG)
   catch( bad_cast& thisException )
   {
      cerr << "ERROR: Suez caught \"bad_cast\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
#endif
   // all specific runtime_error exceptions
#if !defined(UNDEF_UNDERFLOW_ERROR_EXCEPT_BUG)
   catch( underflow_error& thisException )
   {
      cerr << "ERROR: Suez caught \"underflow_error\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
#endif
#if !defined(UNDEF_OVERFLOW_ERROR_EXCEPT_BUG)
   catch( overflow_error& thisException )
   {
      cerr << "ERROR: Suez caught \"overflow_error\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
#endif
#if !defined(UNDEFINED_RANGE_ERROR_EXCEPT_BUG)
   catch( range_error& thisException )
   {
      cerr << "ERROR: Suez caught \"range_error\" exception:\n\""
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
#endif
   // now catch all other runtime_error exceptions
   catch( runtime_error& thisException )
   {
      cerr << "ERROR: Suez caught \"runtime_error\" exception:\n\"" 
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
   // now catch DAExceptions
   catch( DAExceptionBase& thisException )
   {
      cerr << "ERROR: Suez caught a DAException:\n\"" 
           << thisException.exceptionStack() << endl
	   << thisException.what() << "\"\n ... exiting program" << endl;
   }
   // catch non-standard exceptions
#if defined(XALLOC_NON_STANDARD_EXCEPTION_BUG)
   catch( xmsg& thisException ) 
   {
      cerr << "ERROR: Suez caught \"xmsg\" exception:\n\""
	   << thisException.why() << "\"\n ... exiting program" << endl;
   }
#endif
   // now just catch anything
   catch( ... )
   {
      cerr << "ERROR: Suez caught an (unknown/user-defined?) exception:\n" 
	   << " ... exiting program" << endl;
   }
#endif /* in debug mode don't catch exceptions */

   return returnValue;
}



int TheMain( int argc, char** argv )
{
   // setup message logger
   //FrameLogger logger( reportLevel, cout );
   //MessageLog::Tie( "." , logger );
   // try new logging via ReportModule
   ReportModule* reportModule = new ReportModule( INFO );

   // test proper Boolean behavior; if no good, will exit
   testBoolean();

   // force-link StorageManagement, so that nobody has else to
   forceLinkStorageManagement();

   // handle options
   DABoolean runGUI = false, runSetupFile = true;
   string runFile( "" );
   
   // handle options using the getopt standard function; if 
   int option;
   optarg = 0;
   const char* const ARGS = "f:xqo:-:e:";
   while( true )
   {
      option = getopt( argc, argv, ARGS );
      if( option == -1 ) break;

      switch( option ) {
	 case 'f': {
	    runFile = optarg;
	    break;
	 }
	 case 'x': {
	    runGUI = true;
	    break;
	 }
	 case 'q': {
	    runSetupFile = false;
	    break;
	 }
	 case '?': { // ignore
	    break;
	 }
	case '-': { // ignore
	   break;
	}
	case 'o': { // ignore
	   break;
	}
	case 'e': { // ignore
	   break;
	}
	 default: {
	    report( WARNING, kFacilityString )
	       << "don't understand option: " << option << endl;
	 }
      }
   }

   // print out any other non-option arguments (since we won't process them!)
   if( optind < argc ) {
      ostream& os = report( ERROR, kFacilityString );
      os <<"ignoring non-option ARGV-elements: \n";
      while( optind < argc ) {
	 os << argv[optind++] << "\n";
      }
      os << "continuing..." << endl;
   }
   
   // --------- welcome user -------------
   report( INFO, kFacilityString ) << "\n" << welcomeMsg << endl;
   report( INFO, kFacilityString ) << IdString  << "\n"
      //<< TagString << "\n"
      //<< VersionString << "\n"
				   << endl;
   
   // create Interpreter
   Interpreter::setInterpreter( new TclInterpreter() );
   Interpreter* interpreter = Interpreter::interpreter();
   assert( 0 != interpreter );

   // --------- Initialize Suez Application -----------------
   JobControl* jobControl = JobControl::instance();
   assert( 0 != jobControl );
   jobControl->setName( "Suez" );
   jobControl->addModule( reportModule, false ); // not owned by JobControl
   jobControl->initLogger( reportModule->logger() );
   
   // ------------------ create user application ----------------
   // create on the stack, no need to control deletion
   UserApplication userApplication( jobControl );
   
   if( true == runGUI ) 
   {
      report( INFO, kFacilityString )
	 << "starting suez in gui mode..." << endl;

      report( ERROR, kFacilityString )
	 << "not supported at the moment!" << endl;
      
      //// Need to enable commands before running initialization scripts
      ////interpreter->initialize( false, argc, argv );
      ////jobControl->enableCommands();

      //SuezInit( *interpreter, runFile, runSetupFile );
      //interpreter->initialize( runGUI, argc, argv );
   }
   else // default is pure cli mode
   {
      // Need to enable commands before running initialization scripts
      interpreter->initialize( false, argc, argv );
      interpreter->pushActiveModule(jobControl);

      SuezInit( *interpreter, runFile, runSetupFile );
      
      // ------------ Main Interaction Loop with User -----------
      userApplication.runEmbeddedCommands( *interpreter );
      if( true == userApplication.interactiveMode() ) {
	 interpreter->loop();
      }
   }
   
   // -------------- Clean Up --------------------------------
   jobControl->finalizeLogger( reportModule->logger() );
   jobControl->destroy();
   
   delete interpreter;
   delete reportModule; // finally delete ReportModule (to be last!)

   return 0;
}      

DABoolean
SuezInit( Interpreter& interpreter, 
	  const string& runFile,
	  DABoolean runSetupFile )
{
   DABoolean status = true;

   if( true == runSetupFile ) { // turn setup file off if requested

      // if file was specified from command line, run that.
      if( runFile != "" ) {
	 interpreter.runCommandFile( runFile.c_str() );
      }
      else {
	 // Run any initialization scripts (official, home directory etc.)
	 
	 // only run official startup script if personal doesn't exist
	 const char* const HOME = getenv( "HOME" );
	 if( 0 != HOME ) {
	    string personalStartupScript = string( HOME ) + string( "/.suezrc" );
	    
	    ifstream ifs( personalStartupScript.c_str() );
	    if( ifs ) { // personal file exists
	       ifs.close();
	       interpreter.runCommandFile( personalStartupScript.c_str() );
	       
	    }
	    else {      // no personal file exists; run official
	       const char* const C3_SUEZRC = getenv( "C3_SUEZRC" );
	       if( 0 == C3_SUEZRC ) {
		  report( WARNING, kFacilityString )
		     << "cannot find C3_SUEZRC environment variable!\n " 
		     << "May not be able to initialize properly!" << endl;
	       }
	       else {
		  // need to find first entry in C3_SUEZRC string
		  StringTokenizer c3_suezrc_tokens( C3_SUEZRC );
		  if( c3_suezrc_tokens.hasMoreElements() ) {
		     // now assemble file name
		     string officialStartupScript 
			= c3_suezrc_tokens.nextElement() + string( "/suezrc" );
		     interpreter.runCommandFile( officialStartupScript.c_str() );
		  }
	       }
	    }
	 }
	 else {
	    report( WARNING, kFacilityString )
	       << "cannot find HOME environment variable!\n " 
	       << "May not be able to initialize properly!" << endl;
	 }
      } // specified script on command-line or not
   } // run setup file or not

   return status;
}


// ----------------------------------------------------------------------
// Revision history
//
// $Log: SuezMain.cc,v $
// Revision 1.72  2003/04/25 15:42:19  vk
// Add DAException library to Makefile. When exception thrown add printout about DAExceptionStack
//
// Revision 1.71  2002/11/21 20:21:44  cleo3
// rearranged library links for cxx 6.5
//
// Revision 1.70  2002/10/30 19:40:41  topher
// Added environment variable setting option -e
//
// Revision 1.69  2002/10/16 20:46:36  cdj
// if an exception is caught, suez exits with a non zero return value
//
// Revision 1.68  2002/10/04 15:15:01  topher
// Added argument-passing options
//
// Revision 1.67  2002/01/03 21:07:26  cdj
// updated to work with new Interpreter handling
//
// Revision 1.66  2000/12/01 20:33:59  cdj
// updated to work with DynamicLoader package
//
// Revision 1.65  2000/07/29 17:02:28  mkl
// catch non-standard Solaris exceptions (base class and out-of-memory)
//
// Revision 1.64  2000/03/31 16:52:37  mkl
// for online benefit: print warning, but ignore any illegal options
//
// Revision 1.63  2000/03/14 22:52:06  mkl
// UserApplication header now comes from Suez
//
// Revision 1.62  2000/03/14 20:55:21  mkl
// new UserApplication functionality: new embeddedCommand and interactiveMode methods for running Suez in standalone mode in online
//
// Revision 1.61  2000/03/02 16:59:59  mkl
// new StringTokenizer parse handling
//
// Revision 1.60  2000/01/27 03:46:18  mkl
// create UserApplication on the stack, no need to control deletion
//
// Revision 1.59  2000/01/17 19:03:58  mkl
// run_file was not working correctly together with -f flag
//
// Revision 1.58  2000/01/03 19:00:07  mkl
// print 'ERROR' as part of exception-handling message
//
// Revision 1.57  1999/12/06 16:17:18  mkl
// name change: CleoException-->DAException
//
// Revision 1.56  1999/12/03 19:10:43  mkl
// reflect move of CleoException to its own package
//
// Revision 1.55  1999/12/02 23:36:43  mkl
// have to catch CleoExceptions now (as last resort)
//
// Revision 1.54  1999/11/05 17:43:52  mkl
// move force-link to StorageManagement into its own file (for better maintainance)
//
// Revision 1.53  1999/11/04 20:42:11  mkl
// force link to StorageManagement, so that nobody else has to
//
// Revision 1.52  1999/09/08 19:50:19  mkl
// ReportModule has to be deleted last so that all messages get put out
//
// Revision 1.51  1999/09/07 14:47:50  mkl
// move to new Histogram Interface
//
// Revision 1.50  1999/08/31 23:10:04  mkl
// removed deprecated report level option; fixed up handling of explicitly specified executable
//
// Revision 1.49  1999/08/26 03:02:03  mkl
// better than rethrow: dont catch in debug mode
//
// Revision 1.48  1999/08/26 02:29:29  mkl
// rethrow caught exceptions so that gdb can properly work
//
// Revision 1.47  1999/08/25 21:27:46  mkl
// ReportModule allows setting of report level and logging to files
//
// Revision 1.46  1999/05/12 20:34:21  mkl
// change default report level to INFO
//
// Revision 1.45  1999/05/02 02:45:46  mkl
// use new Signal library
//
// Revision 1.44  1999/04/30 18:09:25  mkl
// catch all exceptions
//
// Revision 1.43  1999/04/29 19:54:42  mkl
// take out MAILTO line from test scripts
//
// Revision 1.42  1999/04/19 18:01:50  mkl
// added info on tcl via web pages
//
// Revision 1.41  1999/02/12 15:53:10  mkl
// simplify nooption error
//
// Revision 1.40  1999/02/12 00:12:59  mkl
// allow run-time control over report level
//
// Revision 1.39  1999/01/27 23:02:54  mkl
// trivial change to get rid of insure++ complaint about null parameter
//
// Revision 1.38  1998/12/16 03:17:59  mkl
// print 'Welcome to Suez' instead of 'SUEZ' as a test for release system
//
// Revision 1.37  1998/12/09 22:46:43  mkl
// Logger now logs everything above DEBUG severity level.
//
// Revision 1.36  1998/11/30 20:38:18  mkl
// test TagString
//
// Revision 1.35  1998/11/30 19:51:11  mkl
// properly format SuezMain
//
