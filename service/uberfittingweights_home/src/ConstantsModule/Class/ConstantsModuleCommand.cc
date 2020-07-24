// -*- C++ -*-
//
// Package:     ConstantsModule
// Module:      ConstantsModuleCommand
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Apr 21 10:20:17 EDT 1999
// $Id: ConstantsModuleCommand.cc,v 1.5 2000/06/19 19:05:55 cdj Exp $
//
// Revision history
//
// $Log: ConstantsModuleCommand.cc,v $
// Revision 1.5  2000/06/19 19:05:55  cdj
// now returns a Tcl error if problem creating source
//
// Revision 1.4  2000/04/27 21:17:19  cdj
// now works on OSF using MICO ORB
//
// Revision 1.3  2000/04/22 19:33:18  cdj
// added streams_exclude sub-command
//
// Revision 1.2  1999/09/28 15:18:18  cdj
// include file Conversion.h was moved to a different package
//
// Revision 1.1  1999/04/29 19:20:48  cdj
// rewritten to allow multiple databases at the same time
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ConstantsModule/ConstantsModuleCommand.h"
#include "ConstantsModule/ConstantsModule.h"
#include "ConstantsDelivery/DBConstantsSelectionTag.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"

#include "CommandPattern/Conversion.h"

// STL classes
#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ConstantsModule.ConstantsModuleCommand" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ConstantsModuleCommand.cc,v 1.5 2000/06/19 19:05:55 cdj Exp $";
static const char* const kTagString = "$Name:  $";

static const string helpMessage =
string( " Valid subcommands are:                                        \n")+
string( "                                                               \n")+
string( "  help                       see this help page                \n")+
string( "                                                               \n")+
string( "  in <dbname> meta <tag> [<tag>...] [streams [<stream>...]]    \n")+
string( "               [exclude_streams [<stream>...] ]                \n")+
string( "    reads from the database using meta selection tags          \n")+
string( "                                                               \n")+
string( "  in <dbname> standard <tag> [<tag>...] [streams [<stream>...]]\n")+
string( "               [exclude_streams [<stream>...] ]                \n")+
string( "    reads from the database using standard selection tags      \n")+
//string( "                                                               \n")+
//string( "  in <dbname> version <major> <minor>  [streams [<stream>...]] \n")+
//string( "    reads from the database using a specific meta version      \n")+
string( "                                                               \n");

//
// static data member definitions
//

//
// constructors and destructor
//
ConstantsModuleCommand::ConstantsModuleCommand( const Command::Name& iName, 
				ConstantsModule* target )
   : Command( iName, target, false ) // "false" to make it external command
{
}

// ConstantsModuleCommand::ConstantsModuleCommand( const ConstantsModuleCommand& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ConstantsModuleCommand::~ConstantsModuleCommand()
{
}

//
// assignment operators
//
// const ConstantsModuleCommand& ConstantsModuleCommand::operator=( const ConstantsModuleCommand& rhs )
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
int
ConstantsModuleCommand::execute( int argc, char* argv[] )
{
   int result = COMMAND_OK;

   // expect two arguments: "print" or "help"
   setArgs( argc, argv );
   if( 2 <= argc )
   {
      if( 0 == strcmp( argv[1], "help" ) )
      {
         result = helpHandler();
      } else
      if( 0 == strcmp( argv[1], "in" ) )
      {
	 result = sourceHandler();
      }
      else { // bad command argument!
         report( ERROR, kFacilityString ) << "invalid command arg" << endl;
	 helpHandler();
         result = COMMAND_ERROR;
      }
   }
   else // number of arguments
   {
      report( ERROR, kFacilityString ) << "wrong # args" << endl;
      helpHandler();
      result = COMMAND_ERROR;
   }


   return result;
}

int
ConstantsModuleCommand::helpHandler()
{
   // print help from header

   report( INFO, kFacilityString )
      << "\n\nDescription: " << name() << "\n\n"
      << helpMessage
      << endl;

   return COMMAND_OK;
}

int
ConstantsModuleCommand::sourceHandler()
{
   static const string kStreamsCommand("streams");
   static const string kExcludeStreamsCommand("exclude_streams");
   string const kSubCommandArray[] ={ kStreamsCommand, 
				     kExcludeStreamsCommand };
   const unsigned int kSubCommandArraySize = sizeof(kSubCommandArray)/sizeof(string);

   static const STL_SET(string) kSubCommands( kSubCommandArray, 
					     kSubCommandArray+kSubCommandArraySize );

   int returnValue = COMMAND_OK;

   // expect 3 or more arguments!
   if( m_argc < 5 ) {
      report( ERROR, kFacilityString) << "wrong # args" << endl;
      return COMMAND_ERROR;
   }

   //get the name of the database
   string dbname( getArgument( 2 ) );

   //see how the 'version' is specified
   DBConstantsSelectionTag selectionTag( string("") );
   
   const string versionMethod( getArgument(3 ) );
   int nextArgument= 0;
   if( versionMethod == "version") {
      report( ERROR, kFacilityString) <<"version is unsupported" << endl;
/*
      if( m_argc < 6 ) {
	 report( ERROR, kFacilityString) 
	    << "wrong # args need both major and minor version #" << endl;
	 return COMMAND_ERROR;
      }
      //get the major number
      DABoolean isNumber;
      int majorVersion = Conversion::stringToDigit( getArgument(4), isNumber );
      if( !isNumber ) {
	 report( ERROR, kFacilityString) 
	    <<"major version must be a number, not " << getArgument(4) << endl;
	 return COMMAND_ERROR;
      }
      int minorVersion = Conversion::stringToDigit( getArgument(5), isNumber );
      if( !isNumber ) {
	 report( ERROR, kFacilityString) 
	    <<"minor version must be a number, not " << getArgument(5) << endl;
	 return COMMAND_ERROR;
      }
      nextArgument = 6;
      */    

   } else if( versionMethod == "meta") {
      nextArgument = parseTags(4, //start at this argument
			       DBVersion::Meta,
			       kSubCommands,
			       selectionTag );
   } else if( versionMethod == "standard") {
      nextArgument = parseTags(4, //start at this argument
			       DBVersion::Standard,
			       kSubCommands,
			       selectionTag );
   } else {
      report( ERROR, kFacilityString) <<"unknown versioning method: " << versionMethod << endl;
      return COMMAND_ERROR;
   }

   //try to parse for streams
   StreamSet streams;
   StreamSet excludeStreams;

   if( nextArgument != m_argc ) {
       if( string(getArgument(nextArgument) ) == kStreamsCommand ) {
	  ++nextArgument;
	  if( nextArgument != m_argc ) {
	     parseStreams( streams, nextArgument );
	  }
       } else if( string(getArgument(nextArgument) ) == 
		  kExcludeStreamsCommand ) {
	  ++nextArgument;
	  if( nextArgument != m_argc ) {
	     parseStreams( excludeStreams, nextArgument );
	  }
       } else {
	  //need defaults
	  report( ERROR, kFacilityString) 
	     << "parsing error, was expecting \"streams\" or \"exclude_streams\" but got "
	     << getArgument(nextArgument) << endl;
	  return COMMAND_ERROR;
       } 
   }
   
   if( ! target()->openSource( dbname, selectionTag, 
			       streams, excludeStreams ) ) {
      returnValue = COMMAND_ERROR;
   }

   return returnValue;
}

ConstantsModule*
ConstantsModuleCommand::target()
{
   return (ConstantsModule*)Command::target();
}

//
// const member functions
//
const ConstantsModule*
ConstantsModuleCommand::target() const
{
   return (const ConstantsModule*)Command::target();
}

int
ConstantsModuleCommand::parseTags( int index, 
				   DBVersion::SelectType iSelectType,
				   const STL_SET(string)& iEndStrings,
				   DBConstantsSelectionTag& oTag ) const
{
   const char * arg;
   vector<string> tags;
   tags.reserve(5); //picked 5 out of the air
   STL_SET(string)::const_iterator itEnd = iEndStrings.end();
   while( ( 0 != ( arg = getArgument( index ) ) ) &&
	  ( itEnd == iEndStrings.find( string(arg) ) ) ) {
      tags.push_back( arg );
      ++index; //need to increment the index
   }
   oTag = DBConstantsSelectionTag( tags, iSelectType );
   return index;
}

//stolen from DBCommand.cc
void
ConstantsModuleCommand::parseStreams( StreamSet& streams, int index ) const
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

