// -*- C++ -*-
//
// Package:     HandSiAlignmentMod
// Module:      ChangeConstantsCommand
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Mon Mar 13 15:41:23 EST 2000
// $Id: ChangeConstantsCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $
//
// Revision history
//
// $Log: ChangeConstantsCommand.cc,v $
// Revision 1.1.1.1  2000/03/17 15:49:46  cdj
// imported HandSiAlignmentMod source
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HandSiAlignmentMod/ChangeConstantsCommand.h"
#include "HandSiAlignmentMod/HandSiAlignmentMod.h"
#include "CommandPattern/Conversion.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "HandSiAlignmentMod.ChangeConstantsCommand" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChangeConstantsCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $";
static const char* const kTagString = "$Name: v01_01_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
ChangeConstantsCommand::ChangeConstantsCommand( 
   const Command::Name& iName, 
   HandSiAlignmentMod* target )
   : Command( iName, target, false ) // "false" to make it external command
{
   // The arguments are: 
   // 1.) command name (what you will type at the Suez command line)
   // 2.) minimum number of arguments expected
   // 3.) maxiumu number of arguments expected
   // 4.) pointer to a member function which will handle the subcommand
   // 5.) the "this" pointer to point to this command object.
   addSubcommand(
      new Subcommand< ChangeConstantsCommand >(
	 "ladder", 4, 5,
	 &ChangeConstantsCommand::ladderHandler,
	 this )
      );
   addSubcommand(
      new Subcommand< ChangeConstantsCommand >(
	 "sensor", 4, 5,
	 &ChangeConstantsCommand::sensorHandler,
	 this )
      );
   addSubcommand(
      new Subcommand< ChangeConstantsCommand >(
	 "movey", 2, 3,
	 &ChangeConstantsCommand::moveyHandler,
	 this )
      );
   addSubcommand(
      new Subcommand< ChangeConstantsCommand >(
	 "replot", 2, 2,
	 &ChangeConstantsCommand::replotHandler,
	 this )
      );
}

// ChangeConstantsCommand::ChangeConstantsCommand( const ChangeConstantsCommand& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ChangeConstantsCommand::~ChangeConstantsCommand()
{
}

//
// assignment operators
//
// const ChangeConstantsCommand& ChangeConstantsCommand::operator=( const ChangeConstantsCommand& rhs )
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
//ChangeConstantsCommand::execute( int argc, char* argv[] )
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
ChangeConstantsCommand::helpHandler()
{
   int returnValue = COMMAND_OK;

   // print help from header
   static const string helpMessage =
      string( " calls FrameIteratorModuleBase.\n" )+
      string( "                    \n" )+
      string( " Subcommands are:   \n" )+
      string( "    help            \n" )+
      string( "    go              \n" );

   report( INFO, kFacilityString )
      << "\n\nDescription: " << name() << "\n\n"
      << helpMessage
      << endl;

   return returnValue;
}

int
ChangeConstantsCommand::ladderHandler()
{
   // to get access to arguments use: 
   // unsigned int m_argc; --> number of arguments including the main command:
   // Suez> arg0 arg1 arg2 ...
   // char* m_argv[]; --> actual argument

   uint which = 0;
   char* digit = m_argv[2];
   while( *digit != 0 )
   {
      if ( *digit < '0'  ||  *digit > '9' )
      {
	 report( SYSTEM, kFacilityString )
	    << "syntax is \"const ladder N dir [+/-val]\"   where N is a natural number!"
	    << endl;
	 return COMMAND_ERROR;
      }

      which *= 10;
      which += *digit - '0';
      digit++;
   }

   if ( which < 1  ||  which > 61 )
   {
      report( SYSTEM, kFacilityString )
	    << "Ladders range from 1 to 61." << endl;
      return COMMAND_ERROR;
   }

   HandSiAlignmentMod::Direction dir;
   if ( strcmp( m_argv[3], "x" ) == 0 )
      dir = HandSiAlignmentMod::kDeltaX;
   else if ( strcmp( m_argv[3], "y" ) == 0 )
      dir = HandSiAlignmentMod::kDeltaY;
   else if ( strcmp( m_argv[3], "z" ) == 0 )
      dir = HandSiAlignmentMod::kDeltaZ;
   else if ( strcmp( m_argv[3], "phix" ) == 0 )
      dir = HandSiAlignmentMod::kPhiX;
   else if ( strcmp( m_argv[3], "phiy" ) == 0 )
      dir = HandSiAlignmentMod::kPhiY;
   else if ( strcmp( m_argv[3], "phiz" ) == 0 )
      dir = HandSiAlignmentMod::kPhiZ;
   else
   {
      report( SYSTEM, kFacilityString )
	    << "Valid directions are \"x\", \"y\", \"z\", \"phix\", \"phiy\", \"phiz\"."
	    << endl;
      return COMMAND_ERROR;
   }

   if ( m_argc == 4 )
   {
      report( SYSTEM, kFacilityString )
	 << ( target()->get_constant( HandSiAlignmentMod::kLadder, which, dir )
	      * 1000000. );
      switch ( dir )
      {
	 case HandSiAlignmentMod::kDeltaX:
	 case HandSiAlignmentMod::kDeltaY:
	 case HandSiAlignmentMod::kDeltaZ:
	    report( SYSTEM, kFacilityString ) << " microns." << endl;  break; 
	 case HandSiAlignmentMod::kPhiX:
	 case HandSiAlignmentMod::kPhiY:
	 case HandSiAlignmentMod::kPhiZ:
	    report( SYSTEM, kFacilityString ) << " microradians." << endl;  break; 
      }
      return COMMAND_OK;
   }

   int val = 0;
   DABoolean neg = false;
   digit = m_argv[4];
   while( *digit != 0 )
   {
      if ( *digit == '+'  ||  *digit == '-' )
      {
	 if ( *digit == '-' )
	    neg = true;
	 digit++;
	 continue;
      }

      if ( *digit < '0'  ||  *digit > '9' )
      {
	 report( SYSTEM, kFacilityString )
	    << "syntax is \"const ladder N dir [+/-val]\"   where val is an integer!" << endl;
	 return COMMAND_ERROR;
      }

      val *= 10;
      val += *digit - '0';
      digit++;
   }

   if ( neg ) val = -1 * val;

   if ( m_argc == 5 )
   {
      target()->set_constant(
	 HandSiAlignmentMod::kLadder, which, dir,
	 ( target()->get_constant( HandSiAlignmentMod::kLadder, which, dir )
	 + ( double( val ) / 1000000. ) ) );
      report( SYSTEM, kFacilityString )
	 << ( target()->get_constant( HandSiAlignmentMod::kLadder, which, dir )
	      * 1000000. );
      switch ( dir )
      {
	 case HandSiAlignmentMod::kDeltaX:
	 case HandSiAlignmentMod::kDeltaY:
	 case HandSiAlignmentMod::kDeltaZ:
	    report( SYSTEM, kFacilityString ) << " microns." << endl;  break; 
	 case HandSiAlignmentMod::kPhiX:
	 case HandSiAlignmentMod::kPhiY:
	 case HandSiAlignmentMod::kPhiZ:
	    report( SYSTEM, kFacilityString ) << " microradians." << endl;  break; 
      }
      return COMMAND_OK;
   }

   report( SYSTEM, kFacilityString ) << "wrong # of args" << endl;
   return COMMAND_ERROR;
}

int
ChangeConstantsCommand::sensorHandler()
{
   // to get access to arguments use: 
   // unsigned int m_argc; --> number of arguments including the main command:
   // Suez> arg0 arg1 arg2 ...
   // char* m_argv[]; --> actual argument
   
   uint which = 0;
   char* digit = m_argv[2];
   while( *digit != 0 )
   {
      if ( *digit < '0'  ||  *digit > '9' )
      {
	 report( SYSTEM, kFacilityString )
	    << "syntax is \"const sensor N dir [+/-val]\"   where N is a natural number!"
	    << endl;
	 return COMMAND_ERROR;
      }

      which *= 10;
      which += *digit - '0';
      digit++;
   }

   if ( which < 1  ||  which > 447 )
   {
      report( SYSTEM, kFacilityString )
	    << "Sensors range from 1 to 447." << endl;
      return COMMAND_ERROR;
   }

   HandSiAlignmentMod::Direction dir;
   if ( strcmp( m_argv[3], "x" ) == 0 )
      dir = HandSiAlignmentMod::kDeltaX;
   else if ( strcmp( m_argv[3], "y" ) == 0 )
      dir = HandSiAlignmentMod::kDeltaY;
   else if ( strcmp( m_argv[3], "z" ) == 0 )
      dir = HandSiAlignmentMod::kDeltaZ;
   else if ( strcmp( m_argv[3], "phix" ) == 0 )
      dir = HandSiAlignmentMod::kPhiX;
   else if ( strcmp( m_argv[3], "phiy" ) == 0 )
      dir = HandSiAlignmentMod::kPhiY;
   else if ( strcmp( m_argv[3], "phiz" ) == 0 )
      dir = HandSiAlignmentMod::kPhiZ;
   else
   {
      report( SYSTEM, kFacilityString )
	    << "Valid directions are \"x\", \"y\", \"z\", \"phix\", \"phiy\", \"phiz\"."
	    << endl;
      return COMMAND_ERROR;
   }

   if ( m_argc == 4 )
   {
      report( SYSTEM, kFacilityString )
	 << ( target()->get_constant( HandSiAlignmentMod::kSensor, which, dir )
	      * 1000000. );
      switch ( dir )
      {
	 case HandSiAlignmentMod::kDeltaX:
	 case HandSiAlignmentMod::kDeltaY:
	 case HandSiAlignmentMod::kDeltaZ:
	    report( SYSTEM, kFacilityString ) << " microns." << endl;  break; 
	 case HandSiAlignmentMod::kPhiX:
	 case HandSiAlignmentMod::kPhiY:
	 case HandSiAlignmentMod::kPhiZ:
	    report( SYSTEM, kFacilityString ) << " microradians." << endl;  break; 
      }
      return COMMAND_OK;
   }

   int val = 0;
   DABoolean neg = false;
   digit = m_argv[4];
   while( *digit != 0 )
   {
      if ( *digit == '+'  ||  *digit == '-' )
      {
	 if ( *digit == '-' )
	    neg = true;
	 digit++;
	 continue;
      }

      if ( *digit < '0'  ||  *digit > '9' )
      {
	 report( SYSTEM, kFacilityString )
	    << "syntax is \"const sensor N dir [+/-val]\"   where val is an integer!" << endl;
	 return COMMAND_ERROR;
      }

      val *= 10;
      val += *digit - '0';
      digit++;
   }

   if ( neg ) val = -1 * val;

   if ( m_argc == 5 )
   {
      target()->set_constant(
	 HandSiAlignmentMod::kSensor, which, dir,
	 ( target()->get_constant( HandSiAlignmentMod::kSensor, which, dir )
	 + ( double( val ) / 1000000. ) ) );
      report( SYSTEM, kFacilityString )
	 << ( target()->get_constant( HandSiAlignmentMod::kSensor, which, dir )
	      * 1000000. );
      switch ( dir )
      {
	 case HandSiAlignmentMod::kDeltaX:
	 case HandSiAlignmentMod::kDeltaY:
	 case HandSiAlignmentMod::kDeltaZ:
	    report( SYSTEM, kFacilityString ) << " microns." << endl;  break; 
	 case HandSiAlignmentMod::kPhiX:
	 case HandSiAlignmentMod::kPhiY:
	 case HandSiAlignmentMod::kPhiZ:
	    report( SYSTEM, kFacilityString ) << " microradians." << endl;  break; 
      }
      return COMMAND_OK;
   }

   report( SYSTEM, kFacilityString ) << "wrong # of args" << endl;
   return COMMAND_ERROR;
}

int
ChangeConstantsCommand::moveyHandler()
{
   // to get access to arguments use: 
   // unsigned int m_argc; --> number of arguments including the main command:
   // Suez> arg0 arg1 arg2 ...
   // char* m_argv[]; --> actual argument

   int which = target()->get_showing();
   
   if ( m_argc == 2 )
   {
      report( SYSTEM, kFacilityString )
	 << ( target()->get_constant( HandSiAlignmentMod::kSensor, which, HandSiAlignmentMod::kDeltaY )
	      * 1000000. ) << " microns." << endl;
      return COMMAND_OK;
   }

   int val = 0;
   DABoolean neg = false;
   char* digit = m_argv[2];
   while( *digit != 0 )
   {
      if ( *digit == '+'  ||  *digit == '-' )
      {
	 if ( *digit == '-' )
	    neg = true;
	 digit++;
	 continue;
      }

      if ( *digit < '0'  ||  *digit > '9' )
      {
	 report( SYSTEM, kFacilityString )
	    << "syntax is \"const movey [+/-val]\"   where val is an integer!" << endl;
	 return COMMAND_ERROR;
      }

      val *= 10;
      val += *digit - '0';
      digit++;
   }

   if ( neg ) val = -1 * val;

   if ( m_argc == 3 )
   {
      target()->set_constant(
	 HandSiAlignmentMod::kLadder, which, HandSiAlignmentMod::kDeltaY,
	 ( target()->get_constant( HandSiAlignmentMod::kLadder, which, HandSiAlignmentMod::kDeltaY )
	 + ( double( val ) / 1000000. ) ) );
      report( SYSTEM, kFacilityString )
	 << "Ladder " << which << " now at "
	 << ( target()->get_constant( HandSiAlignmentMod::kLadder, which, HandSiAlignmentMod::kDeltaY )
	      * 1000000. ) << " microns." << endl;

      for ( int sensor = 1;  sensor <= 447;  sensor++ )
	 if ( target()->get_cotTheta( sensor ) != 0. )
	 {
	    target()->set_constant(
	       HandSiAlignmentMod::kSensor, sensor, HandSiAlignmentMod::kDeltaZ,
	       ( target()->get_constant( HandSiAlignmentMod::kSensor, sensor,
					 HandSiAlignmentMod::kDeltaZ )
		 + ( double( val ) * target()->get_cotTheta( sensor ) / 1000000. ) ) );
	    report( SYSTEM, kFacilityString )
	       << "Sensor " << sensor << " now at "
	       << ( target()->get_constant( HandSiAlignmentMod::kSensor, sensor,
					    HandSiAlignmentMod::kDeltaZ )
		    * 1000000. ) << " microns." << endl;
	 }

      return COMMAND_OK;
   }

   report( SYSTEM, kFacilityString ) << "wrong # of args" << endl;
   return COMMAND_ERROR;
}

int
ChangeConstantsCommand::replotHandler()
{
   // to get access to arguments use: 
   // unsigned int m_argc; --> number of arguments including the main command:
   // Suez> arg0 arg1 arg2 ...
   // char* m_argv[]; --> actual argument

   target()->replot();
   return COMMAND_OK;
}

HandSiAlignmentMod*
ChangeConstantsCommand::target()
{
   return (HandSiAlignmentMod*)Command::target();
}

//
// const member functions
//
const HandSiAlignmentMod*
ChangeConstantsCommand::target() const
{
   return (const HandSiAlignmentMod*)Command::target();
}

//
// static member functions
//
