// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRCathodePanelConstants
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:     Inga Karliner
// Created:    September 11, 2000
//
// Revision history
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#include "C++Std/iomanip.h"
#include <assert.h> 
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ADRGeom/ADRCathodePanelConstants.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRCathodePanelConstants" ;
 
//
// static data member definitions
//

//
// constructors and destructor
//
ADRCathodePanelConstants::ADRCathodePanelConstants() : 
 m_PanelNumber(0),
 m_PhiMinEast(0),
 m_PhiMaxEast(0),
 m_ZOffEastPhiMin(0),
 m_ZOffEastPhiMax(0),
 m_ZShift(0)
{
}

ADRCathodePanelConstants::ADRCathodePanelConstants(
			     unsigned int PanelNumber,
			     double PhiMinEast,
			     double PhiMaxEast,
			     double ZOffEastPhiMin,
			     double ZOffEastPhiMax,
			     double ZShift)
  :   m_PanelNumber(PanelNumber),
      m_PhiMinEast(PhiMinEast),
      m_PhiMaxEast(PhiMaxEast),
      m_ZOffEastPhiMin(ZOffEastPhiMin),
      m_ZOffEastPhiMax(ZOffEastPhiMax),
      m_ZShift(ZShift)
{
}

// ADRCathodePanelConstants::ADRCathodePanelConstants( const ADRCathodePanelConstants& )
// {
// }

ADRCathodePanelConstants::~ADRCathodePanelConstants()
{
}

//
// assignment operators
//
// const ADRCathodePanelConstants& 
// ADRCathodePanelConstants::operator=( const ADRCathodePanelConstants& )
// {
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











