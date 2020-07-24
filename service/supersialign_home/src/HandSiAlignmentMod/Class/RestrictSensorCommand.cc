// -*- C++ -*-
//
// Package:     HandSiAlignmentMod
// Module:      RestrictSensorCommand
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Wed Apr 10 16:01:15 EST 2002
// $Id: RestrictSensorCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $
//
// Revision history
//
// $Log: RestrictSensorCommand.cc,v $
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
#include "HandSiAlignmentMod/RestrictSensorCommand.h"
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
static const char* const kFacilityString = "HandSiAlignmentMod.RestrictSensorCommand" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: RestrictSensorCommand.cc,v 1.1.1.1 2000/03/17 15:49:46 cdj Exp $";
static const char* const kTagString = "$Name: v01_01_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
RestrictSensorCommand::RestrictSensorCommand( 
   const Command::Name& iName, 
   HandSiAlignmentMod* target )
   : Command( iName, target, false ) // "false" to make it external command
{
   m_sensormin[ 0] =   1; m_sensormax[ 0] =   3;
   m_sensormin[ 1] =   4; m_sensormax[ 1] =   6;
   m_sensormin[ 2] =   7; m_sensormax[ 2] =   9;
   m_sensormin[ 3] =  10; m_sensormax[ 3] =  12;
   m_sensormin[ 4] =  13; m_sensormax[ 4] =  15;
   m_sensormin[ 5] =  16; m_sensormax[ 5] =  18;
   m_sensormin[ 6] =  19; m_sensormax[ 6] =  21;
   m_sensormin[ 7] =  22; m_sensormax[ 7] =  25;
   m_sensormin[ 8] =  26; m_sensormax[ 8] =  29;
   m_sensormin[ 9] =  30; m_sensormax[ 9] =  33;
   m_sensormin[10] =  34; m_sensormax[10] =  37;
   m_sensormin[11] =  38; m_sensormax[11] =  41;
   m_sensormin[12] =  42; m_sensormax[12] =  45;
   m_sensormin[13] =  46; m_sensormax[13] =  49;
   m_sensormin[14] =  50; m_sensormax[14] =  53;
   m_sensormin[15] =  54; m_sensormax[15] =  57;
   m_sensormin[16] =  58; m_sensormax[16] =  61;
   m_sensormin[17] =  62; m_sensormax[17] =  68;
   m_sensormin[18] =  69; m_sensormax[18] =  75;
   m_sensormin[19] =  76; m_sensormax[19] =  82;
   m_sensormin[20] =  83; m_sensormax[20] =  89;
   m_sensormin[21] =  90; m_sensormax[21] =  96;
   m_sensormin[22] =  97; m_sensormax[22] = 103;
   m_sensormin[23] = 104; m_sensormax[23] = 110;
   m_sensormin[24] = 111; m_sensormax[24] = 117;
   m_sensormin[25] = 118; m_sensormax[25] = 124;
   m_sensormin[26] = 125; m_sensormax[26] = 131;
   m_sensormin[27] = 132; m_sensormax[27] = 138;
   m_sensormin[28] = 139; m_sensormax[28] = 145;
   m_sensormin[29] = 146; m_sensormax[29] = 152;
   m_sensormin[30] = 153; m_sensormax[30] = 159;
   m_sensormin[31] = 160; m_sensormax[31] = 166;
   m_sensormin[32] = 167; m_sensormax[32] = 173;
   m_sensormin[33] = 174; m_sensormax[33] = 180;
   m_sensormin[34] = 181; m_sensormax[34] = 187;
   m_sensormin[35] = 188; m_sensormax[35] = 197;
   m_sensormin[36] = 198; m_sensormax[36] = 207;
   m_sensormin[37] = 208; m_sensormax[37] = 217;
   m_sensormin[38] = 218; m_sensormax[38] = 227;
   m_sensormin[39] = 228; m_sensormax[39] = 237;
   m_sensormin[40] = 238; m_sensormax[40] = 247;
   m_sensormin[41] = 248; m_sensormax[41] = 257;
   m_sensormin[42] = 258; m_sensormax[42] = 267;
   m_sensormin[43] = 268; m_sensormax[43] = 277;
   m_sensormin[44] = 278; m_sensormax[44] = 287;
   m_sensormin[45] = 288; m_sensormax[45] = 297;
   m_sensormin[46] = 298; m_sensormax[46] = 307;
   m_sensormin[47] = 308; m_sensormax[47] = 317;
   m_sensormin[48] = 318; m_sensormax[48] = 327;
   m_sensormin[49] = 328; m_sensormax[49] = 337;
   m_sensormin[50] = 338; m_sensormax[50] = 347;
   m_sensormin[51] = 348; m_sensormax[51] = 357;
   m_sensormin[52] = 358; m_sensormax[52] = 367;
   m_sensormin[53] = 368; m_sensormax[53] = 377;
   m_sensormin[54] = 378; m_sensormax[54] = 387;
   m_sensormin[55] = 388; m_sensormax[55] = 397;
   m_sensormin[56] = 398; m_sensormax[56] = 407;
   m_sensormin[57] = 408; m_sensormax[57] = 417;
   m_sensormin[58] = 418; m_sensormax[58] = 427;
   m_sensormin[59] = 428; m_sensormax[59] = 437;
   m_sensormin[60] = 438; m_sensormax[60] = 447;

   m_restrict = false;
   m_sensors.clear();
}

// RestrictSensorCommand::RestrictSensorCommand( const RestrictSensorCommand& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

RestrictSensorCommand::~RestrictSensorCommand()
{
}

//
// assignment operators
//
// const RestrictSensorCommand& RestrictSensorCommand::operator=( const RestrictSensorCommand& rhs )
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

RestrictSensorCommand::execute( int argc, char* argv[] )
{
   int result = COMMAND_OK;

   // expect two arguments: "print" or "help"
   setArgs( argc, argv );
   if ( 2 == argc )
   {
      if ( 0 == strcmp( argv[1], "help" ) ) {
	 result = helpHandler();

      } else if ( 0 == strcmp( argv[1], "reset" ) ) {
	 m_sensors.clear();

      } else if ( 0 == strcmp( argv[1], "restrict" ) ) {
	 m_restrict = true;

      } else if ( 0 == strcmp( argv[1], "unrestrict" ) ) {
	 m_restrict = false;

      } else if ( 0 == strcmp( argv[1], "ladder" ) ) {
	 if ( target()->context() < 1  ||  target()->context() > 61 )
	    cout << "Choose a ladder with the ladder command first." << endl;
	 else {
	    cout << "Sensors for this ladder: ";
	    for ( int sen = m_sensormin[ target()->context() - 1 ];
		  sen <= m_sensormax[ target()->context() - 1 ];
		  sen++ )
	       cout << sen << " ";
	    cout << endl;
	 } // end context() is okay

      } else if ( 0 == strcmp( argv[1], "list" ) ) {
	 cout << "Sensors in the mask: ";
	 if ( m_sensors.empty() )
	    cout << "<none>" << endl;
	 else {
	    for ( vector< int >::const_iterator seniter = m_sensors.begin();
		  seniter != m_sensors.end();
		  seniter++ )
	       cout << *seniter << " ";
	    cout << endl;
	 } // end nonzero number of sensors

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

	 if ( value > 0  &&  value <= 447 )
	    m_sensors.push_back( value );
	 else
	    report( ERROR, kFacilityString ) << "invalid ladder number" << endl;

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
RestrictSensorCommand::helpHandler()
{
   int returnValue = COMMAND_OK;

   report( NOTICE, kFacilityString )
      << "sensor restrict        turn on restriction to particular sensor numbers" << endl
      << "sensor unrestrict      turn off restriction to particular sensor numbers" << endl
      << "sensor ladder          list all sensors available for the current ladder" << endl
      << "sensor #               add a sensor to the list" << endl
      << "sensor reset           clear the list" << endl
      << "sensor list            print the list" << endl;

   return returnValue;
}

HandSiAlignmentMod*
RestrictSensorCommand::target()
{
   return (HandSiAlignmentMod*)Command::target();
}

int
RestrictSensorCommand::sensormin( int ladder )
{
   return m_sensormin[ ladder-1 ];
}

int
RestrictSensorCommand::sensormax( int ladder )
{
   return m_sensormax[ ladder-1 ];
}

DABoolean
RestrictSensorCommand::restrict( int sensor )
{
   if ( m_restrict ) {
      DABoolean inlist( false );
      for ( vector< int >::const_iterator seniter = m_sensors.begin();
	    seniter != m_sensors.end();
	    seniter++ )
	 if ( sensor == *seniter ) inlist = true;
      return !inlist;
   }
   else
      return false;
	 
}

//
// const member functions
//
const HandSiAlignmentMod*
RestrictSensorCommand::target() const
{
   return (const HandSiAlignmentMod*)Command::target();
}

//
// static member functions
//
