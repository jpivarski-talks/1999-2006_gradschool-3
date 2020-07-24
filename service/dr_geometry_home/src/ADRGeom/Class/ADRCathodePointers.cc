// -*- C++ -*-
//
// Package:     <ADRGeom>
// Module:      ADRCathodePointers
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon and Inga Karliner
// Created:     Tue Nov 24 1999
// $Id: ADRCathodePointers.cc,v 1.2 2000/12/11 21:16:24 inga Exp $
//
// Revision history
//
// $Log: ADRCathodePointers.cc,v $
// Revision 1.2  2000/12/11 21:16:24  inga
// add cathode panels survey
//
// Revision 1.1  1999/11/29 18:36:28  inga
// add ADRCathodePointers.cc 
// 12/11/00 I. Karliner
//   Add pointer DGConstLabVolumePtr  m_spEastConicalEndCap
//

#include "Experiment/Experiment.h"

// system include files
//#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ADRGeom/ADRCathodePointers.h"
#include "DetectorGeometry/DGConstVolumePtr.h"

// STL classes
//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRCathodePointers" ;

//
//
// static data member definitions
//

//
// constructors and destructor
//

ADRCathodePointers::ADRCathodePointers(
		       const DGConstLabVolumePtr& spCathodePanel0,
		       const DGConstLabVolumePtr& spCathodePanel1,
		       const DGConstLabVolumePtr& spCathodePanel2,
		       const DGConstLabVolumePtr& spCathodePanel3,
		       const DGConstLabVolumePtr& spCathodePanel4,
		       const DGConstLabVolumePtr& spCathodePanel5,
		       const DGConstLabVolumePtr& spCathodePanel6,
		       const DGConstLabVolumePtr& spCathodePanel7,
		       const DGConstLabVolumePtr& spConicalEndPlateEast) :
  m_spCathodePanel0(spCathodePanel0), m_spCathodePanel1(spCathodePanel1), 
  m_spCathodePanel2(spCathodePanel2), m_spCathodePanel3(spCathodePanel3), 
  m_spCathodePanel4(spCathodePanel4), m_spCathodePanel5(spCathodePanel5), 
  m_spCathodePanel6(spCathodePanel6), m_spCathodePanel7(spCathodePanel7),
  m_spConicalEndPlateEast(spConicalEndPlateEast)
{}

ADRCathodePointers::~ADRCathodePointers()
{}

// ADRCathodePointers::ADRCathodePointers( const ADRCathodePointers& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//
// assignment operators
//
// const ADRCathodePointers& ADRCathodePointers::operator=( const ADRCathodePointers& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }






