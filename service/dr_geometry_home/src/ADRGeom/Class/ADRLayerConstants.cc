// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRLayerConstants
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:     Inga Karliner
// Created:    July 28, 1999
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
#include "ADRGeom/ADRLayerConstants.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRLayerConstants" ;
 
//
// static data member definitions
//

//
// constructors and destructor
//
ADRLayerConstants::ADRLayerConstants() :
  m_DRLayerNumber(0),
  m_NumberOfWires(0),
  m_ReadoutEnd(0),
  m_RadiusEndCapCM(0.),
  m_Phi0EastInDegrees(0.),
  m_Phi0WestInDegrees(0.),
  m_TwistCakeInDegrees(0.),
  m_CakeNumber_Axial(0),
  m_ZStepPolycone_StereoLayer(0),
  m_ZBowingInch(0.),
  m_ZBushing(0.),
  m_ZPin(0.),
  m_Pin_WireRadiusInch(0.),
  m_SagXMicron(0.),
  m_SagYMicron(0.),
  m_FieldRadiusLessEndCapCM(0.),
  m_FieldRadiusMoreEndCapCM(0.)
{
}

ADRLayerConstants::ADRLayerConstants(
		    unsigned int DRLayerNumber,
		    unsigned int NumberOfWires,
		    int   ReadoutEnd,
		    double RadiusEndCapCM,
		    double Phi0EastInDegrees,
		    double Phi0WestInDegrees,
		    double TwistCakeInDegrees,
		    unsigned int CakeNumber_Axial,
		    unsigned int ZStepPolycone_StereoLayer,
		    double ZBowingInch,
		    double ZBushing,
		    double ZPin,
		    double Pin_WireRadiusInch,
		    double SagXMicron,
		    double SagYMicron,
		    double FieldRadiusLessEndCapCM,
		    double FieldRadiusMoreEndCapCM) :
  m_DRLayerNumber(DRLayerNumber),
  m_NumberOfWires(NumberOfWires),
  m_ReadoutEnd(ReadoutEnd),
  m_RadiusEndCapCM(RadiusEndCapCM),
  m_Phi0EastInDegrees(Phi0EastInDegrees),
  m_Phi0WestInDegrees(Phi0WestInDegrees),
  m_TwistCakeInDegrees(TwistCakeInDegrees),
  m_CakeNumber_Axial(CakeNumber_Axial),
  m_ZStepPolycone_StereoLayer(ZStepPolycone_StereoLayer),
  m_ZBowingInch(ZBowingInch),
  m_ZBushing(ZBushing),
  m_ZPin(ZPin),
  m_Pin_WireRadiusInch(Pin_WireRadiusInch),
  m_SagXMicron(SagXMicron),
  m_SagYMicron(SagYMicron),
  m_FieldRadiusLessEndCapCM(FieldRadiusLessEndCapCM),
  m_FieldRadiusMoreEndCapCM(FieldRadiusMoreEndCapCM)
{
}

// ADRLayerConstants::ADRLayerConstants( const ADRLayerConstants& )
// {
// }

ADRLayerConstants::~ADRLayerConstants()
{
}

//
// assignment operators
//
// const ADRLayerConstants& ADRLayerConstants::operator=( const ADRLayerConstants& )
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











