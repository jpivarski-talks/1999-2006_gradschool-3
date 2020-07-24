// -*- C++ -*-
//
// Package:     HandSiAlignmentMod
// Module:      PickLadderCommand
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Wed Apr 10 16:01:15 EST 2002
// $Id: PickLadderCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $
//
// Revision history
//
// $Log: PickLadderCommand.cc,v $
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
#include "HandSiAlignmentMod/PickLadderCommand.h"
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
static const char* const kFacilityString = "HandSiAlignmentMod.PickLadderCommand" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: PickLadderCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $";
static const char* const kTagString = "$Name: v01_01_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
PickLadderCommand::PickLadderCommand( 
   const Command::Name& iName, 
   HandSiAlignmentMod* target )
   : Command( iName, target, false ) // "false" to make it external command
{
}

// PickLadderCommand::PickLadderCommand( const PickLadderCommand& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

PickLadderCommand::~PickLadderCommand()
{
}

//
// assignment operators
//
// const PickLadderCommand& PickLadderCommand::operator=( const PickLadderCommand& rhs )
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

PickLadderCommand::execute( int argc, char* argv[] )
{
   int result = COMMAND_OK;

   // expect two arguments: "print" or "help"
   setArgs( argc, argv );
   if ( 2 == argc )
   {
      if ( 0 == strcmp( argv[1], "help" ) ) {
	 result = helpHandler();
      } else {
	 int value = 0;
	 char* digit( m_argv[1] );
	 while ( *digit != 0 )
	    if ( *digit >= '0'  &&  *digit <= '9' ) {
	       value *= 10;
	       value += *digit - '0';
	       digit++;  // that is, move the pointer forward
	    } else {
	       report( ERROR, kFacilityString ) << "invalid command arg" << endl;
	       helpHandler();
	       result = COMMAND_ERROR;
	       break;
	    } // end if and while

	 if ( result == COMMAND_OK ) {
	    if ( value > 0  &&  value <= 61 )
	       result = ( target()->switchToContext( value ) ? COMMAND_OK : COMMAND_ERROR );
	    else {
	       report( ERROR, kFacilityString ) << "invalid ladder number" << endl;
	       helpHandler();
	       result = COMMAND_ERROR;
	    }
	 } // end if COMMAND_OK

      } // end if arg 2 is "help" or a number
   }
   else {
      report( ERROR, kFacilityString ) << "wrong # args" << endl;
      helpHandler();
      result = COMMAND_ERROR;
   } // end if wrong number of arguments

   return result;
}

int
PickLadderCommand::helpHandler()
{
   int returnValue = COMMAND_OK;

   report( INFO, kFacilityString )
      << "ladder N      changes the current context"
      << " to ladder N (reloading eskim files)" << endl;

   return returnValue;
}

HandSiAlignmentMod*
PickLadderCommand::target()
{
   return (HandSiAlignmentMod*)Command::target();
}

//
// const member functions
//
const HandSiAlignmentMod*
PickLadderCommand::target() const
{
   return (const HandSiAlignmentMod*)Command::target();
}

//
// static member functions
//
