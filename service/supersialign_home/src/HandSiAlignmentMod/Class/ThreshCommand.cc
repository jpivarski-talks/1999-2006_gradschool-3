// -*- C++ -*-
//
// Package:     HandSiAlignmentMod
// Module:      ThreshCommand
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Wed Apr 10 16:01:15 EST 2002
// $Id: ThreshCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $
//
// Revision history
//
// $Log: ThreshCommand.cc,v $
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
#include "HandSiAlignmentMod/ThreshCommand.h"
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
static const char* const kFacilityString = "HandSiAlignmentMod.ThreshCommand" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ThreshCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $";
static const char* const kTagString = "$Name: v01_01_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
ThreshCommand::ThreshCommand( 
   const Command::Name& iName, 
   HandSiAlignmentMod* target,
   double thresh,
   WhichThresh which )
   : Command( iName, target, false ) // "false" to make it external command
   , m_thresh( thresh )
   , m_which( which )
   , m_hv()
{
}

// ThreshCommand::ThreshCommand( const ThreshCommand& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ThreshCommand::~ThreshCommand()
{
}

//
// assignment operators
//
// const ThreshCommand& ThreshCommand::operator=( const ThreshCommand& rhs )
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

ThreshCommand::execute( int argc, char* argv[] )
{
   int result = COMMAND_OK;

   // expect two arguments: "print" or "help"
   setArgs( argc, argv );
   if ( 2 == argc )
   {
      if ( 0 == strcmp( argv[1], "help" ) ) {
	 result = helpHandler();
      }

      else if ( 0 == strcmp( argv[1], "natural" ) ) {
	 double value( target()->naturalThresh( m_which ) );

	 if ( m_which == kPosThresh ) m_hv.pos_thresh = value;
	 if ( m_which == kNegThresh ) m_hv.neg_thresh = value;

	 m_thresh = value;
	 cout << "Set to " << m_thresh << " meters." << endl;

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

	 if ( result == COMMAND_OK ) {
	    if ( m_which == kPosThresh ) m_hv.pos_thresh = value;
	    if ( m_which == kNegThresh ) m_hv.neg_thresh = value;

	    m_thresh = value;
	    cout << "Set to " << m_thresh << " meters." << endl;
	 }
      } // end if arg 2 is "help" or a number
    
   } else if ( 1 == argc ) {
      cout << m_thresh << " meters." << endl;

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
ThreshCommand::helpHandler()
{
   int returnValue = COMMAND_OK;

   report( NOTICE, kFacilityString )
      << "{p,n}thresh [[-]N[.M]]    shows/sets the positive/negative Z threshold (meters)" << endl;

   return returnValue;
}

HandSiAlignmentMod*
ThreshCommand::target()
{
   return (HandSiAlignmentMod*)Command::target();
}

//
// const member functions
//
const HandSiAlignmentMod*
ThreshCommand::target() const
{
   return (const HandSiAlignmentMod*)Command::target();
}

//
// static member functions
//
