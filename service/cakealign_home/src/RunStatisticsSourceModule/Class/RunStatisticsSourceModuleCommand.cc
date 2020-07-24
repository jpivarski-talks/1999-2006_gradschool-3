// -*- C++ -*-
//
// Package:     RunStatisticsSourceModule
// Module:      RunStatisticsSourceModuleCommand
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Tue Jan 25 09:04:46 EST 2000
// $Id: RunStatisticsSourceModuleCommand.cc,v 1.2 2001/03/02 01:13:12 lkg Exp $
//
// Revision history
//
// $Log: RunStatisticsSourceModuleCommand.cc,v $
// Revision 1.2  2001/03/02 01:13:12  lkg
// add startrun to the default stream list for the 'runstatistics in' command
//
// Revision 1.1.1.1  2000/01/29 20:47:37  cdj
// imported RunStatisticsSourceModule source
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "RunStatisticsSourceModule/RunStatisticsSourceModuleCommand.h"
#include "RunStatisticsSourceModule/RunStatisticsSourceModule.h"
#include "CommandPattern/Conversion.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "RunStatisticsSourceModule.RunStatisticsSourceModuleCommand" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: RunStatisticsSourceModuleCommand.cc,v 1.2 2001/03/02 01:13:12 lkg Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
RunStatisticsSourceModuleCommand::RunStatisticsSourceModuleCommand( const Command::Name& iName, 
				RunStatisticsSourceModule* target )
   : Command( iName, target, false ) // "false" to make it external command
{
   // The arguments are: 
   // 1.) command name (what you will type at the Suez command line)
   // 2.) minimum number of arguments expected
   // 3.) maxiumu number of arguments expected
   // 4.) pointer to a member function which will handle the subcommand
   // 5.) the "this" pointer to point to this command object.
   addSubcommand( 
      new Subcommand< RunStatisticsSourceModuleCommand >(
         "help", 2, 2,
         &RunStatisticsSourceModuleCommand::helpHandler,
         this )
      );
   addSubcommand( 
      new Subcommand< RunStatisticsSourceModuleCommand >(
         "in", 2, 50, //set outrageously high value for max streams
         &RunStatisticsSourceModuleCommand::sourceHandler,
         this )
      );
}

// RunStatisticsSourceModuleCommand::RunStatisticsSourceModuleCommand( const RunStatisticsSourceModuleCommand& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

RunStatisticsSourceModuleCommand::~RunStatisticsSourceModuleCommand()
{
}

//
// assignment operators
//
// const RunStatisticsSourceModuleCommand& RunStatisticsSourceModuleCommand::operator=( const RunStatisticsSourceModuleCommand& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//
// ---------------------------------------------------------------------
// This is one way of doing subcommand handling; but it's easier to 
// use the "Subcommand" class, as shown in an example above in the ctor.
//int
//RunStatisticsSourceModuleCommand::execute( int argc, char* argv[] )
//{
//   int result = COMMAND_OK;
//
//   // expect two arguments: "print" or "help"
//   setArgs( argc, argv );
//   if( 2 == argc )
//   {
//      if( 0 == strcmp( argv[1], "help" ) )
//      {
//         result = helpHandler();
//      } else
//      if( 0 == strcmp( argv[1], "do" ) )
//      {
//	 result = doHandler();
//      }
//      else { // bad command argument!
//         report( ERROR, kFacilityString ) << "invalid command arg" << endl;
//	 helpHandler();
//         result = COMMAND_ERROR;
//      }
//   }
//   else // number of arguments
//   {
//      report( ERROR, kFacilityString ) << "wrong # args" << endl;
//      helpHandler();
//      result = COMMAND_ERROR;
//   }
//
//   return result;
//}

int
RunStatisticsSourceModuleCommand::helpHandler()
{
   int returnValue = COMMAND_OK;

   // print help from header
   static const string helpMessage =
      string( " Valid Subcommands are\n" )+
      string( "                    \n" )+
      string( "    help            see this page\n" )+
      string( "                    \n" )+
      string( "    in [<stream> [<stream> ...] ]  \n" )+
      string( "       create a run statistics source.  If no streams are\n")+
      string( "        specified, defaults to BeginRun");

   report( INFO, kFacilityString )
      << "\n\nDescription: " << name() << "\n\n"
      << helpMessage
      << endl;

   return returnValue;
}

int
RunStatisticsSourceModuleCommand::sourceHandler()
{
   int returnValue = COMMAND_OK;

   // to get access to arguments use: 
   // unsigned int m_argc; --> number of arguments including the main command:
   // Suez> arg0 arg1 arg2 ...
   // char* m_argv[]; --> actual argument
   assert( m_argc >= 2 );

   StreamSet streams;
   if( 2 == m_argc ) {
      //use defualt streams
      streams.add( Stream::kBeginRun );
      streams.add( Stream::kStartRun );
   } else {
      parseStreams( streams, 3 );
   }

   target()->openSource( streams );
   return returnValue;
}

RunStatisticsSourceModule*
RunStatisticsSourceModuleCommand::target()
{
   return (RunStatisticsSourceModule*)Command::target();
}

//
// const member functions
//
const RunStatisticsSourceModule*
RunStatisticsSourceModuleCommand::target() const
{
   return (const RunStatisticsSourceModule*)Command::target();
}


//stolen from DBCommand.cc
void
RunStatisticsSourceModuleCommand::parseStreams( StreamSet& streams, int index ) const
{
   const char* arg;
   while ( 0 != ( arg = getArgument( index++ ) ) ) 
   {
      // check if stream string is non-standard --> issue warning
      // (this is a good idea, because the user might have 
      // mistyped the stream name
      Stream::Type newStreamType = StreamType( arg );
      if( true != newStreamType.isStandard() ) 
      {
         report( WARNING, kFacilityString )
            << "Using non-standard Stream type!" << arg << "." << endl;
      }
      streams.add( newStreamType );
   }
}

//
// static member functions
//
