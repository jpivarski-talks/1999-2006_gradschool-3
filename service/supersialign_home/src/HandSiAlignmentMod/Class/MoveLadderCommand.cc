// -*- C++ -*-
//
// Package:     HandSiAlignmentMod
// Module:      MoveLadderCommand
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Wed Apr 10 16:01:15 EST 2002
// $Id: MoveLadderCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $
//
// Revision history
//
// $Log: MoveLadderCommand.cc,v $
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
#include "HandSiAlignmentMod/MoveLadderCommand.h"
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
static const char* const kFacilityString = "HandSiAlignmentMod.MoveLadderCommand" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MoveLadderCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $";
static const char* const kTagString = "$Name: v01_01_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
MoveLadderCommand::MoveLadderCommand( 
   const Command::Name& iName, 
   HandSiAlignmentMod* target,
   int iDir )
   : Command( iName, target, false ) // "false" to make it external command
   , direction( iDir )
{
}

// MoveLadderCommand::MoveLadderCommand( const MoveLadderCommand& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MoveLadderCommand::~MoveLadderCommand()
{
}

//
// assignment operators
//
// const MoveLadderCommand& MoveLadderCommand::operator=( const MoveLadderCommand& rhs )
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

MoveLadderCommand::execute( int argc, char* argv[] )
{
   int result = COMMAND_OK;

   // expect two arguments: "print" or "help"
   setArgs( argc, argv );
   if ( 2 == argc )
   {
      if ( 0 == strcmp( argv[1], "help" ) ) {
	 result = helpHandler();

      } else if ( 0 == strcmp( argv[1], "reset" ) ) {
	 result = ( target()->move( ((HandSiAlignmentMod::Direction)(direction)), 0.,
				    HandSiAlignmentMod::kReset )
		    ? COMMAND_OK : COMMAND_ERROR );

      } else if ( 0 == strcmp( argv[1], "zero" ) ) {
	 result = ( target()->move( ((HandSiAlignmentMod::Direction)(direction)), 0.,
				    HandSiAlignmentMod::kZero )
		    ? COMMAND_OK : COMMAND_ERROR );

      } else if ( 0 == strcmp( argv[1], "round" ) ) {
	 result = ( target()->move( ((HandSiAlignmentMod::Direction)(direction)), 0.,
				    HandSiAlignmentMod::kRound )
		    ? COMMAND_OK : COMMAND_ERROR );

      } else {
	 char* digit( m_argv[1] );

	 double value( 0. );
	 double power( 1. );
	 DABoolean before_decimal( true );
	 DABoolean negative( false );

	 if ( *digit == '-' ) {
	    negative = true;
	    digit++;  // that is, move the pointer forward
	 }

	 while ( *digit != 0 ) {
	    if ( *digit == '.' ) {
	       before_decimal = false;

	    } else if ( before_decimal  &&  *digit >= '0'  &&  *digit <= '9' ) {
	       value *= 10.;
	       value += *digit - '0';
	       
	    } else if ( *digit >= '0'  &&  *digit <= '9' ) {
	       power /= 10.;
	       value += ( *digit - '0' ) * power;

	    } else {
	       report( ERROR, kFacilityString ) << "invalid command arg" << endl;
	       helpHandler();
	       result = COMMAND_ERROR;
	       break;

	    } // end switch over characters

	    digit++;  // that is, move the pointer forward
	 } // end while loop

	 if ( negative ) value *= -1.;

	 if ( result == COMMAND_OK )
	    result = ( target()->move( ((HandSiAlignmentMod::Direction)(direction)), value,
				       HandSiAlignmentMod::kMove )
		       ? COMMAND_OK : COMMAND_ERROR );
      } // end if arg 2 is "help" or a number

   } else {
      report( ERROR, kFacilityString ) << "wrong # args" << endl;
      helpHandler();
      result = COMMAND_ERROR;
   } // end if wrong number of arguments

   if ( result == COMMAND_ERROR )
      report( ERROR, kFacilityString ) << "something went wrong." << endl;

   return result;
}

int
MoveLadderCommand::helpHandler()
{
   int returnValue = COMMAND_OK;

   report( NOTICE, kFacilityString )
      << "x|y|z|phix|phiy|phiz|sagitta [-]N[.M]    moves the current ladder N microns" << endl;

   return returnValue;
}

HandSiAlignmentMod*
MoveLadderCommand::target()
{
   return (HandSiAlignmentMod*)Command::target();
}

//
// const member functions
//
const HandSiAlignmentMod*
MoveLadderCommand::target() const
{
   return (const HandSiAlignmentMod*)Command::target();
}

//
// static member functions
//
