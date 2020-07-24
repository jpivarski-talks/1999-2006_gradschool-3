// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRCathodeParams
// 
// Description: fake namespace to hold ADRCathode geometry parameters.
//
// Implementation:
//     These are definitions of static parameters for ADRCathode.
//
// Author:      Inga Karliner
// Created:     May 17, 1999
//
// Revision history
// 6/28/00  update cathode pitch from 0.4554" to 0.45486"
// 12/11/00 I.Karliner: Survey measurements of 8 panels were added
//                      PhiMinEast,     PhiMaxEast, 
//                      ZOffEastPhiMin, ZOffEastPhiMax  
//                      together with the overall shift kADCathodeZShift 

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "ADRGeom/ADRCathodeParams.h"
#include "Experiment/units.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRCathodeParams" ;

// static data member definitions
//
//const double ADRCathodeParams:: kADRCathodePitch =0.4554 * k_in;
  const double ADRCathodeParams:: kADRCathodePitch =0.45486 * k_in;
//const double ADRCathodeParams:: kADRCathodePitch =11.57 * k_mm;
  const double ADRCathodeParams:: kADRCathodePadLength =10.54  *  k_mm ;
//
// measured points are DelMin, DelMax from panel's edge
  const double ADRCathodeParams:: kPanelMeasuredDelMin = k_in * 4.5;
  const double ADRCathodeParams:: kPanelMeasuredDelMax = k_in * 4.25;

// measured points at East side are at PhiMin, PhiMax
  double ADRCathodeParams:: kPanelMeasuredPhiMinEast[] =
  {
    0.1450,  0.9310, 1.7160,  2.5020,  3.2870,  4.0720,  4.8580,  5.6430
  };
  double ADRCathodeParams:: kPanelMeasuredPhiMaxEast[] =
  {
    0.6480,  1.4330, 2.2190,  3.0040,  3.7900,  4.5750,  5.3600,  6.1460
  };

// measured points at East side are ZOffEastPhiMin, ZOffEastPhiMax
// from surface 65 
  double ADRCathodeParams:: kPanelMeasuredZOffEastPhiMin[] =
  {
    k_in*1.7150,  k_in*1.7000, k_in*1.6660,  k_in*1.6970,   
    k_in*1.6880,  k_in*1.7070, k_in*1.7080,  k_in*1.6830    
  };
  double ADRCathodeParams:: kPanelMeasuredZOffEastPhiMax[] =
  {
    k_in*1.7140,  k_in*1.7060, k_in*1.6840,  k_in*1.7010, 
    k_in*1.6890,  k_in*1.7210, k_in*1.7200,  k_in*1.6850
  };
// overall Cathode shift in z
  double ADRCathodeParams:: kADCathodeZShift[] =
  { -0.0005,  -0.0005, -0.0005, -0.0005,
    -0.0005,  -0.0005, -0.0005, -0.0005
  };

//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
// The rest is class baggage, since we can't call this a namespace. //
//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//\\//
//
// constructors and destructor
//
ADRCathodeParams::ADRCathodeParams()
{
}

// ADRCathodeParams::ADRCathodeParams( const ADRCathodeParams& rhs )
// {
//    // do actual copying here, if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ADRCathodeParams::~ADRCathodeParams()
{
}

//
// assignment operators
//
// const ADRCathodeParams& ADRCathodeParams::operator=( const ADRCathodeParams& rhs )
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
// static member functions
//











