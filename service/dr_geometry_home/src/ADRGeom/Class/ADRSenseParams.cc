// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      DRSenseParams
// 
// Description: fake namespace to hold ADRSense geometry parameters.
//
// Implementation:
//     These are definitions of static parameters for ADRSenseWireStore.
//
// Author:      Inga Karliner
// Created:     May 17, 1999
//
// Revision history
//
// 

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ADRGeom/ADRSenseParams.h"
#include "Experiment/units.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRSenseParams" ;

// static data member definitions
//

const int  ADRSenseParams::kADRSenseWiresTotal = 9796;

const int  ADRSenseParams::kADR_NLayers = ADRSenseParams::kNumberOfLayers;

const int  ADRSenseParams::kADR_NWiresLyr1 =  64;
const int  ADRSenseParams::kADR_NWiresLyr2 =  64;  
const int  ADRSenseParams::kADR_NWiresLyr3 =  72;
const int  ADRSenseParams::kADR_NWiresLyr4 =  72;
const int  ADRSenseParams::kADR_NWiresLyr5 =  88;
const int  ADRSenseParams::kADR_NWiresLyr6 =  88;
const int  ADRSenseParams::kADR_NWiresLyr7 =  96;
const int  ADRSenseParams::kADR_NWiresLyr8 =  96;
const int  ADRSenseParams::kADR_NWiresLyr9 = 112;
const int  ADRSenseParams::kADR_NWiresLyr10= 112;
const int  ADRSenseParams::kADR_NWiresLyr11= 128;
const int  ADRSenseParams::kADR_NWiresLyr12= 128;
const int  ADRSenseParams::kADR_NWiresLyr13= 136;
const int  ADRSenseParams::kADR_NWiresLyr14= 136;
const int  ADRSenseParams::kADR_NWiresLyr15= 152;
const int  ADRSenseParams::kADR_NWiresLyr16= 152;
const int  ADRSenseParams::kADR_NWiresLyr17= 180;
const int  ADRSenseParams::kADR_NWiresLyr18= 180;
const int  ADRSenseParams::kADR_NWiresLyr19= 180;
const int  ADRSenseParams::kADR_NWiresLyr20= 180;
const int  ADRSenseParams::kADR_NWiresLyr21= 204;
const int  ADRSenseParams::kADR_NWiresLyr22= 204;
const int  ADRSenseParams::kADR_NWiresLyr23= 204;
const int  ADRSenseParams::kADR_NWiresLyr24= 204;
const int  ADRSenseParams::kADR_NWiresLyr25= 228;
const int  ADRSenseParams::kADR_NWiresLyr26= 228;
const int  ADRSenseParams::kADR_NWiresLyr27= 228;
const int  ADRSenseParams::kADR_NWiresLyr28= 228;
const int  ADRSenseParams::kADR_NWiresLyr29= 252;
const int  ADRSenseParams::kADR_NWiresLyr30= 252;
const int  ADRSenseParams::kADR_NWiresLyr31= 252;
const int  ADRSenseParams::kADR_NWiresLyr32= 252;
const int  ADRSenseParams::kADR_NWiresLyr33= 276;
const int  ADRSenseParams::kADR_NWiresLyr34= 276;
const int  ADRSenseParams::kADR_NWiresLyr35= 276;
const int  ADRSenseParams::kADR_NWiresLyr36= 276;
const int  ADRSenseParams::kADR_NWiresLyr37= 300;
const int  ADRSenseParams::kADR_NWiresLyr38= 300;
const int  ADRSenseParams::kADR_NWiresLyr39= 300;
const int  ADRSenseParams::kADR_NWiresLyr40= 300;
const int  ADRSenseParams::kADR_NWiresLyr41= 324;
const int  ADRSenseParams::kADR_NWiresLyr42= 324;
const int  ADRSenseParams::kADR_NWiresLyr43= 324;
const int  ADRSenseParams::kADR_NWiresLyr44= 324;
const int  ADRSenseParams::kADR_NWiresLyr45= 348;
const int  ADRSenseParams::kADR_NWiresLyr46= 348;
const int  ADRSenseParams::kADR_NWiresLyr47= 348;

const int  ADRSenseParams::kADR_NWiresInLayer[] =
{ 
  ADRSenseParams::kADR_NWiresLyr1,
  ADRSenseParams::kADR_NWiresLyr2,
  ADRSenseParams::kADR_NWiresLyr3,
  ADRSenseParams::kADR_NWiresLyr4,
  ADRSenseParams::kADR_NWiresLyr5,
  ADRSenseParams::kADR_NWiresLyr6,
  ADRSenseParams::kADR_NWiresLyr7,
  ADRSenseParams::kADR_NWiresLyr8,
  ADRSenseParams::kADR_NWiresLyr9,
  ADRSenseParams::kADR_NWiresLyr10,
  ADRSenseParams::kADR_NWiresLyr11,
  ADRSenseParams::kADR_NWiresLyr12,
  ADRSenseParams::kADR_NWiresLyr13,
  ADRSenseParams::kADR_NWiresLyr14,
  ADRSenseParams::kADR_NWiresLyr15,
  ADRSenseParams::kADR_NWiresLyr16,
  ADRSenseParams::kADR_NWiresLyr17,
  ADRSenseParams::kADR_NWiresLyr18,
  ADRSenseParams::kADR_NWiresLyr19,
  ADRSenseParams::kADR_NWiresLyr20,
  ADRSenseParams::kADR_NWiresLyr21,
  ADRSenseParams::kADR_NWiresLyr22,
  ADRSenseParams::kADR_NWiresLyr23,
  ADRSenseParams::kADR_NWiresLyr24,
  ADRSenseParams::kADR_NWiresLyr25,
  ADRSenseParams::kADR_NWiresLyr26,
  ADRSenseParams::kADR_NWiresLyr27,
  ADRSenseParams::kADR_NWiresLyr28,
  ADRSenseParams::kADR_NWiresLyr29,
  ADRSenseParams::kADR_NWiresLyr30,
  ADRSenseParams::kADR_NWiresLyr31,
  ADRSenseParams::kADR_NWiresLyr32,
  ADRSenseParams::kADR_NWiresLyr33,
  ADRSenseParams::kADR_NWiresLyr34,
  ADRSenseParams::kADR_NWiresLyr35,
  ADRSenseParams::kADR_NWiresLyr36,
  ADRSenseParams::kADR_NWiresLyr37,
  ADRSenseParams::kADR_NWiresLyr38,
  ADRSenseParams::kADR_NWiresLyr39,
  ADRSenseParams::kADR_NWiresLyr40,
  ADRSenseParams::kADR_NWiresLyr41,
  ADRSenseParams::kADR_NWiresLyr42,
  ADRSenseParams::kADR_NWiresLyr43,
  ADRSenseParams::kADR_NWiresLyr44,
  ADRSenseParams::kADR_NWiresLyr45,
  ADRSenseParams::kADR_NWiresLyr46,
  ADRSenseParams::kADR_NWiresLyr47
};

const double  ADRSenseParams::kADR_SenseWireDiameter= k_in *.0008 ;
const double  ADRSenseParams::kADR_FieldWireDiameter= k_in *.0043 ;

//

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
// The rest is class baggage, since we can't call this a namespace. //
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//
// constructors and destructor
//
ADRSenseParams::ADRSenseParams()
{
}

// ADRSenseeParams::ADRSenseeParams( const ADRSenseeParams& rhs )
// {
//    // do actual copying here, if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ADRSenseParams::~ADRSenseParams()
{
}

//
// assignment operators
//
// const ADRSenseParams& ADRSenseParams::operator=( const ADRSenseParams& rhs )
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

//
// const member functions
//

//
//  member functions
//








