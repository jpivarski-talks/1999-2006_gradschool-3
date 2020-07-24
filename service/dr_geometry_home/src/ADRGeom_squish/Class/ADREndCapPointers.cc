// -*- C++ -*-
//
// Package:     <ADRGeom>
// Module:      ADREndCapPointers
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon and Inga Karliner
// $Id: ADREndCapPointers.cc,v 1.2 2003/01/27 19:29:51 cdj Exp $
//
// Revision history
//
// $Log: ADREndCapPointers.cc,v $
// Revision 1.2  2003/01/27 19:29:51  cdj
// get iomanip.h from C++Std package
//
// Revision 1.1  1999/11/18 23:14:33  inga
// add ADREndCapPointers
//

#include "Experiment/Experiment.h"

// system include files
#include "C++Std/iomanip.h"
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ADRGeom/ADREndCapPointers.h"
#include "DetectorGeometry/DGConstVolumePtr.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADREndCapPointers" ;

//
// static data member definitions
//

//
// constructors and destructor
//


ADREndCapPointers::ADREndCapPointers(
                        const DGConstLabVolumePtr& spCake1aEast,
			const DGConstLabVolumePtr& spCake2aEast,
			const DGConstLabVolumePtr& spCake3aEast,
			const DGConstLabVolumePtr& spCake4aEast,
			const DGConstLabVolumePtr& spCake5aEast,
			const DGConstLabVolumePtr& spCake6aEast,
			const DGConstLabVolumePtr& spCake7aEast,
			const DGConstLabVolumePtr& spCake8aEast,
			const DGConstLabVolumePtr& spCake1aWest,
			const DGConstLabVolumePtr& spCake2aWest,
			const DGConstLabVolumePtr& spCake3aWest,
			const DGConstLabVolumePtr& spCake4aWest,
			const DGConstLabVolumePtr& spCake5aWest,
			const DGConstLabVolumePtr& spCake6aWest,
			const DGConstLabVolumePtr& spCake7aWest,
			const DGConstLabVolumePtr& spCake8aWest,
			const DGConstLabVolumePtr& spConicalEndPlateEast1,
			const DGConstLabVolumePtr& spConicalEndPlateWest1) :
  m_spCake1aEast(spCake1aEast), m_spCake2aEast(spCake2aEast), 
  m_spCake3aEast(spCake3aEast), m_spCake4aEast(spCake4aEast),
  m_spCake5aEast(spCake5aEast), m_spCake6aEast(spCake6aEast),
  m_spCake7aEast(spCake7aEast), m_spCake8aEast(spCake8aEast), 
  m_spCake1aWest(spCake1aWest), m_spCake2aWest(spCake2aWest), 
  m_spCake3aWest(spCake3aWest), m_spCake4aWest(spCake4aWest), 
  m_spCake5aWest(spCake5aWest), m_spCake6aWest(spCake6aWest), 
  m_spCake7aWest(spCake7aWest), m_spCake8aWest(spCake8aWest),
  m_spConicalEndPlateEast1(spConicalEndPlateEast1),
  m_spConicalEndPlateWest1(spConicalEndPlateWest1)
{}

ADREndCapPointers::~ADREndCapPointers()
{}


//ADREndCapPointers::ADREndCapPointers(const ADREndCapPointers& ){}

//
// assignment operators
//
// const ADREndCapPointers& ADREndCapPointers::operator=( const ADREndCapPointers& )
// {}
