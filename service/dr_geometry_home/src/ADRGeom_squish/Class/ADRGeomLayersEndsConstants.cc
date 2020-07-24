// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRGeomLayersEndsConstants
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
#include "ADRGeom/ADRGeomLayersEndsConstants.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRGeomLayersEndsConstants" ;
 
//
// static data member definitions
//

//
// constructors and destructor
//
ADRGeomLayersEndsConstants::ADRGeomLayersEndsConstants() : 
  m_DRLayerNumber(0),
  m_x0_WestMM(0),
  m_y0_WestMM(0),
  m_z0_WestMM(0),
  m_x0_EastMM(0),
  m_y0_EastMM(0)
{
}

ADRGeomLayersEndsConstants::ADRGeomLayersEndsConstants(
			      unsigned int DRLayerNumber,
			      double Radius_MM,
			      double X_WestMM,
			      double Y_WestMM,
			      double Z_WestMM,
			      double X_EastMM,
			      double Y_EastMM)
  :  m_DRLayerNumber(DRLayerNumber),
     m_radius0MM(Radius_MM),
     m_x0_WestMM(X_WestMM),
     m_y0_WestMM(Y_WestMM),
     m_z0_WestMM(Z_WestMM),
     m_x0_EastMM(X_EastMM),
     m_y0_EastMM(Y_EastMM)
{
}

// ADRGeomLayersEndsConstants::ADRGeomLayersEndsConstants( const ADRGeomLayersEndsConstants& )
// {
// }

ADRGeomLayersEndsConstants::~ADRGeomLayersEndsConstants()
{
}

//
// assignment operators
//
// const ADRGeomLayersEndsConstants& 
// ADRGeomLayersEndsConstants::operator=( const ADRGeomLayersEndsConstants& )
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











