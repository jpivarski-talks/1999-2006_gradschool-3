// -*- C++ -*-
//
// Package:     HandSiAlignmentMod
// Module:      DoInitsCommand
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Wed Apr 10 16:01:15 EST 2002
// $Id: DoInitsCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $
//
// Revision history
//
// $Log: DoInitsCommand.cc,v $
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
#include "HandSiAlignmentMod/DoInitsCommand.h"
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
static const char* const kFacilityString = "HandSiAlignmentMod.DoInitsCommand" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoInitsCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $";
static const char* const kTagString = "$Name: v01_01_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoInitsCommand::DoInitsCommand( 
   const Command::Name& iName, 
   HandSiAlignmentMod* target )
   : Command( iName, target, false ) // "false" to make it external command
{
}

// DoInitsCommand::DoInitsCommand( const DoInitsCommand& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DoInitsCommand::~DoInitsCommand()
{
}

//
// assignment operators
//
// const DoInitsCommand& DoInitsCommand::operator=( const DoInitsCommand& rhs )
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

DoInitsCommand::execute( int argc, char* argv[] )
{
   int result = COMMAND_OK;

   // expect two arguments: "print" or "help"
   setArgs( argc, argv );
   if ( 2 == argc )
   {
      if ( 0 == strcmp( argv[1], "help" ) ) {
	 result = helpHandler();

      } else {
	 report( ERROR, kFacilityString )
	    << "bad parameter" << endl;
	 helpHandler();
	 result = COMMAND_ERROR;
      }

   } else if ( 1 == argc ) {
      result = ( target()->doinits() ? COMMAND_OK : COMMAND_ERROR );

   } else {
      report( ERROR, kFacilityString ) << "wrong # args" << endl;
      helpHandler();
      result = COMMAND_ERROR;
   } // end if wrong number of arguments

   return result;
}

int
DoInitsCommand::helpHandler()
{
   int returnValue = COMMAND_OK;

   report( INFO, kFacilityString )
      << "doinits" << endl;

   return returnValue;
}

HandSiAlignmentMod*
DoInitsCommand::target()
{
   return (HandSiAlignmentMod*)Command::target();
}

//
// const member functions
//
const HandSiAlignmentMod*
DoInitsCommand::target() const
{
   return (const HandSiAlignmentMod*)Command::target();
}

//
// static member functions
//
