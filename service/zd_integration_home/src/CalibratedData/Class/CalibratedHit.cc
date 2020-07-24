// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CalibratedHit
// 
// Description: Calibrated hit base class code.
//
// Implementation: See CalibratedHit.h
//
// Author:      Jon Thaler
// Created:     Tue Dec 16 10:48:21 EST 1997
//              Thu Apr 2 1998 replace Calib* with Calibrated*
//
// Revision history
//
// Revision  3/13/98 Inga Karliner
//   remove all RawHit information
//   take out device from Constructor
//-------------------+
// Environment flags |
//-------------------+
#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "C++Std/iostream.h"


//--------------------+
// user include files |
//--------------------+
#include "CalibratedData/CalibratedHit.h"

//-------------------------------+
// constants, enums and typedefs |
//-------------------------------+
static const char* const kFacilityString = "CalibratedData.CalibratedHit";

//-----------------------------+
// constructors and destructor |
//-----------------------------+
//CalibratedHit::CalibratedHit(const Identifier id)
CalibratedHit::CalibratedHit(Identifier id)
         : m_id        (id)
{}
CalibratedHit::~CalibratedHit()
{
}



// const member functions
// member functions
CalibratedHit::Identifier
CalibratedHit::identifier() const
{
   return ( m_id ) ;
}

DABoolean 
CalibratedHit::operator == (const CalibratedHit& rhs) const
{
   return ( m_id == rhs.m_id );
}

DABoolean 
CalibratedHit::operator != (const CalibratedHit& rhs) const
{
   return !((*this) == rhs);
}

//-------+
// print |
//-------+
void CalibratedHit::printCH(ostream& os) const
{ os << "CalibratedHit: ID = " << m_id
     << " Device = " << DeviceName[deviceType()] << endl; }

//--------------+
// Device names |
//--------------+
string CalibratedHit::DeviceName[CalibratedHit::NumberofDeviceTypes] =
{ "BeamSpot", "SVR", "SVZ", "DR", "RICH", "CC", "Muon", "Cathode", "ZD"};








