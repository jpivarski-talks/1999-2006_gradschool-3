// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRWireSurveyConstants
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:     Inga Karliner
// Created:    September 24, 1999
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
#include "ADRGeom/ADRWireSurveyConstants.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRWireSurveyConstants" ;
 
//
// static data member definitions
//

//
// constructors and destructor
//
ADRWireSurveyConstants::ADRWireSurveyConstants() :
  m_DRWireNumber(0),
  m_Correction_East_x_Microns(0),
  m_Correction_East_y_Microns(0),
  m_Correction_East_z_Microns(0),
  m_Correction_West_x_Microns(0),
  m_Correction_West_y_Microns(0),
  m_Correction_West_z_Microns(0)
{
}

ADRWireSurveyConstants::ADRWireSurveyConstants(
			      unsigned int DRWireNumber,
			      double Correction_East_x_Microns,
			      double Correction_East_y_Microns,
			      double Correction_East_z_Microns,
			      double Correction_West_x_Microns,
			      double Correction_West_y_Microns,
			      double Correction_West_z_Microns)
  :  m_DRWireNumber(DRWireNumber),
     m_Correction_East_x_Microns(Correction_East_x_Microns),
     m_Correction_East_y_Microns(Correction_East_y_Microns),
     m_Correction_East_z_Microns(Correction_East_z_Microns),
     m_Correction_West_x_Microns(Correction_West_x_Microns),
     m_Correction_West_y_Microns(Correction_West_y_Microns),
     m_Correction_West_z_Microns(Correction_West_z_Microns)
{
}

// ADRWireSurveyConstants::ADRWireSurveyConstants( const ADRWireSurveyConstants& )
// {
// }

ADRWireSurveyConstants::~ADRWireSurveyConstants()
{
}

//
// assignment operators
//
// const ADRWireSurveyConstants& ADRWireSurveyConstants::operator=( const ADRWireSurveyConstants& )
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











