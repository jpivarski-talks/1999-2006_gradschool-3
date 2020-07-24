// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiStrip
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Alex Undrus 
// 

#include "Experiment/Experiment.h"
// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiStrip.h"
#include "ASiStorePro/ASiSensorInfo.h"
#include "ASiStorePro/ASiSensorAddress.h"

#include "DetectorGeometry/DGDetectorStore.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ASiStorePro.ASiStrip" ;

//
// static data member definitions
//

//
// constructors and destructor
//
ASiStrip::ASiStrip()

{
}

ASiStrip::ASiStrip( const FAItem<DGDetectorStore >& iStore, 
		    SmallCount layer, SmallCount ladder, 
		    SmallCount hybrid, SmallCount sensor, 
		    ASiSensorInfo* master )
   : m_SiStore( iStore ), m_layer(layer), m_ladder(ladder),
     m_hybrid(hybrid), m_sensor(sensor), m_master( master ) 
{
   setLocalStrip( 0 );
   setSide( ASiStore::RPhiSide );
}

ASiStrip::ASiStrip(const ASiStrip& param) :
   m_SiStore( param.m_SiStore ), m_hybrid( param.m_hybrid ),
   m_layer( param.m_layer ), m_ladder( param.m_ladder ),
   m_sensor( param.m_sensor ),  m_side( param.m_side ),
   m_localStrip( param.m_localStrip ), m_master( param.m_master )
{
}

ASiStrip::~ASiStrip()
{
}

//
// assignment operators
//

 const ASiStrip& ASiStrip::operator=( const ASiStrip& param )
{
  if( this != &param )
  {
     m_SiStore = param.m_SiStore ;
     m_hybrid = param.m_hybrid;
     m_layer = param.m_layer;
     m_ladder = param.m_ladder;
     m_sensor = param.m_sensor;
     m_side = param.m_side;
     m_localStrip = param.m_localStrip;
     m_master = param.m_master;
     
  }
  
  return *this;
}

//
// member functions
//

void 
ASiStrip::setLocalStrip( SmallCount iStrip )
{
// Unsafe!!! what about negative?
   assert( (iStrip <= ASiSensorAddress::kAdNchannelSensor) );
   m_localStrip = iStrip;
}

void
ASiStrip::setSide( SmallCount iSide )
{
   assert( ( iSide == ASiStore::RPhiSide ) || (iSide ==ASiStore::ZSide ) );
   m_side = iSide;
}

void
ASiStrip::reinit( SmallCount iSide, SmallCount iStrip )
{
   assert( 1 >= iSide );
   assert( ASiSensorAddress::kAdNchannelSensor>=iStrip );

   m_localStrip = iStrip;
   m_side = iSide;
}

Count 
ASiStrip::lAddress() const
{
  Count lafirst = ASiSensorAddress::ggetLogicalAddressForSensor(m_side, m_sensor);
  Count ret = lafirst + m_localStrip - 1;
  return ret;
}

SmallCount 
ASiStrip::layer() const
{
   return m_layer;
}

SmallCount 
ASiStrip::ladderInLayer() const
{
   SmallCount ret = m_master->ladderInLayer();
   return ret;
}

SmallCount 
ASiStrip::localStrip() const
{
   return m_localStrip;
}


SmallCount 
ASiStrip::ladder() const
{
   return m_ladder;
}

SmallCount 
ASiStrip::sensorInLadder() const
{
   SmallCount ret = m_master->sensorInLadder();
   return ret;
}

SmallCount 
ASiStrip::hybrid() const
{
   return m_hybrid;
}

SmallCount 
ASiStrip::sensor() const
{
   return m_sensor;
}

SmallCount 
ASiStrip::gang() const
{
   SmallCount ret = m_master->gang();
   return ret;
}

SmallCount 
ASiStrip::side() const
{
   return m_side;
}

DABoolean 
ASiStrip::isRphi() const
{
   if(m_side == 0) { return true; }
   else { return false; }
}

DABoolean 
ASiStrip::isZ() const
{
   if(m_side == 1) { return true; }
   else { return false; }
}

SmallCount 
ASiStrip::chip() const
{
// VERY UGLY. Needs rewriting

  Count LAddress = lAddress();
  SmallCount chip,hybrid;
  Count firstchannel;
  if(LAddress > ASiSensorAddress::kAdZoffset) {
//Z-side
     hybrid = (LAddress - ASiSensorAddress::kAdZoffset - 1)/
	ASiSensorAddress::kAdNchannelSensor + 1;
     firstchannel = (hybrid-1)*ASiSensorAddress::kAdNchannelSensor + 1;
     chip = (LAddress - ASiSensorAddress::kAdZoffset - firstchannel)/
	ASiSensorAddress::kAdNchannelChip + 1 + 4 + (hybrid-1)*8;
  }
  else {
     hybrid = (LAddress - 1)/ASiSensorAddress::kAdNchannelSensor + 1;
     firstchannel = (hybrid-1)*ASiSensorAddress::kAdNchannelSensor + 1;
     chip = (LAddress - firstchannel)/ASiSensorAddress::kAdNchannelChip + 1
	+ (hybrid-1)*8;
  }

  return chip;
}
  
float ASiStrip::pitch() const
{
   float ret(0.);
   if(isRphi()) { ret = m_master->pitchRphi(); }
   if(isZ()) { ret = m_master->pitchZ(); }
   assert( 0. < ret ); 
   return ret;
}

float ASiStrip::localCoor() const
{
   float ret=( m_localStrip - ASiSensorAddress::kAdNchannelSensor/2.)*pitch();
   return ret; 
}

//
// const member functions
//

//
// static member functions
//









