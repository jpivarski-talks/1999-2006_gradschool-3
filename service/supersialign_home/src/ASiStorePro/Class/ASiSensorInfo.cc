// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiSensorInfo
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
#include "SiGeom/SiGeomDescription.h"
#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiLayer.h"
#include "ASiStorePro/ASiLadder.h"
#include "ASiStorePro/ASiHybrid.h"
#include "ASiStorePro/ASiSensorInfo.h"
#include "ASiStorePro/ASiSensorAddress.h"

#include "DetectorGeometry/DGDetectorStore.h"
#include "DetectorGeometry/DGVolumePtr.h"
#include "DetectorGeometry/DGLabVolume.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "DetectorGeometry/DGConstPlacedVolumePtr.h"
#include "DetectorGeometry/DGConstVolumePtr.h"
#include "DetectorGeometry/DGConstVolumeType.h"
#include "DetectorGeometry/DGVolumeBox.h"
#include "DetectorGeometry/DGPlacedVolume.h"
#include "DetectorGeometry/DGPath.h"
#include "DetectorGeometry/DGVolumeParent.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DetectorGeometry/DGTransform.h"

#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"
#include "CLHEP/Geometry/Transform3D.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ASiStorePro.ASiSensorInfo" ;

//
// static data member definitions
//

//
// constructors and destructor
//
ASiSensorInfo::ASiSensorInfo()

{
}

ASiSensorInfo::ASiSensorInfo( const FAItem<DGDetectorStore >& iStore,
			      ASiStore* iStorePro, 
			      SmallCount clay_index,
			      SmallCount clad_index,
			      SmallCount chyb_index,
			      SmallCount c_index ) 
   : m_SiStore( iStore ),
     m_Store( iStorePro ),
     m_index( c_index ),
     m_layer( clay_index ),
     m_ladder( clad_index ),
     m_hybrid( chyb_index ),
     m_sensor( ASiSensorAddress::ggetSensorForHybrid(chyb_index-1) 
	       + c_index -1)
     
{
//moved these outside body of constructor 5/8/00
//m_index = c_index;
//m_layer = clay_index;
//m_ladder = clad_index;
//m_hybrid = chyb_index;
//SmallCount FirstSensor = ASiSensorAddress::ggetSensorForHybrid(m_hybrid-1);
//m_sensor = FirstSensor + m_index - 1;


//   FAItem< ASiSensorStore > SiSensor;

//   cout << " ASSInfo::ASSInfo " << m_index << " * " << m_layer << " * " << m_ladder << " * " << m_hybrid << " * " << m_sensor << " * " << this->number() << endl;
   ASiStrip temp(m_SiStore, m_layer, m_ladder, m_hybrid, m_sensor, this);
   m_strip = temp;

// Code added 5/8/00 to cache m_tHepSLab since it is used mutilple times 
   DGPath pathToLadder = m_SiStore->findPathTo(
      SiGeomDescription::kLadderName[m_ladder-1], 1);
   
   pathToLadder.addNodeToEnd( DGPath::Node(
      SiGeomDescription::kSensorName[m_ladder-1], m_sensor ) );

   const DGConstLabVolumePtr spSLab = m_SiStore->labVolume( pathToLadder );
   m_tHepSLab = spSLab->localToLabAligned();

}

//ASiSensorInfo::ASiSensorInfo(const ASiSensorInfo& param) 
//{
//   ASiStrip temp(param.m_SiStore, param.m_layer, param.m_ladder, param.m_hybrid,param.m_sensor, this);
//     m_strip = temp;
//     m_SiStore = param.m_SiStore ;
//     m_Store = param.m_Store ;
//     m_index = param.m_index;
//     m_hybrid = param.m_hybrid;
//     m_layer = param.m_layer;
//     m_ladder = param.m_ladder;
//     m_sensor = param.m_sensor;
//}

ASiSensorInfo::~ASiSensorInfo()
{
}

//
// assignment operators
//
const ASiSensorInfo& ASiSensorInfo::operator=( const ASiSensorInfo& param )
{
  if( this != &param )
  {
   ASiStrip temp(param.m_SiStore, param.m_layer, param.m_ladder, param.m_hybrid,param.m_sensor, this);
     m_strip = temp;
     m_SiStore = param.m_SiStore ;
     m_Store = param.m_Store ;
     m_index = param.m_index;
     m_hybrid = param.m_hybrid;
     m_layer = param.m_layer;
     m_ladder = param.m_ladder;
     m_sensor = param.m_sensor;
//Added 5/8/00 for caching
     m_tHepSLab = param.m_tHepSLab;
  }

  return *this;
}

// member functions

void 
ASiSensorInfo::setNumberOfSensor( SmallCount iIndex, SmallCount iIndexLayer, SmallCount iIndexLadder, SmallCount iIndexHybrid )
{
   m_index = iIndex;

   m_layer = iIndexLayer;

   m_ladder = iIndexLadder;

   m_hybrid = iIndexHybrid;

   SmallCount FirstSensor = ASiSensorAddress::ggetSensorForHybrid(m_hybrid-1);

   m_sensor = FirstSensor + m_index - 1;

}

void 
ASiSensorInfo::setStrip( SmallCount iSide, SmallCount iStrip )
{
   m_strip.reinit( iSide, iStrip );
}


SmallCount 
ASiSensorInfo::ladderInLayer() const
{
   return ASiSensorAddress::ggetLadderInLayerForSensor(m_sensor)+1;
}


SmallCount 
ASiSensorInfo::sensorInLadder() const
{
   return  ASiSensorAddress::ggetInLadderForSensor(m_sensor)+1;
}


SmallCount 
ASiSensorInfo::gang() const
{
   return  ASiSensorAddress::ggetGangForSensor(m_sensor);
}

DABoolean 
ASiSensorInfo::isWest() const
{
   return (m_hybrid <= ASiSensorAddress::kAdNLadders);
}

DABoolean 
ASiSensorInfo::isEast() const
{
   return (m_hybrid > ASiSensorAddress::kAdNLadders);
}


Count 
ASiSensorInfo::lAddress(SmallCount NSide, SmallCount NLocalStrip ) const
{
//   cout << " inputs for ASiSensorInfo::lAddress " << NSide << " ** " << NLocalStrip << " ** " << m_sensor  << endl;
  if ( NLocalStrip >=  ASiSensorAddress::kAdNchannelSensor )
  {
     report( EMERGENCY, kFacilityString )
	<< " input NLocalStrip for getLAddressForLocalStrip is " 
	<< NLocalStrip
	<< " must be 0 - (kAdNchannelSensor-1) " << endl;
     assert( false );
     exit(1);
  }    

  Count lafirst = ASiSensorAddress::ggetLogicalAddressForSensor(NSide,
								m_sensor);
  return lafirst + NLocalStrip;
}
           
//

HepPoint3D 
ASiSensorInfo::coorInLadder() const
{

//Not cached, this one is different - 5/8/00 
  DGPath pathToLadder = m_SiStore->findPathTo(
     SiGeomDescription::kLadderName[m_ladder-1],1);
  
  pathToLadder.addNodeToEnd( 
     DGPath::Node(SiGeomDescription::kSensorName[m_ladder-1],m_sensor) );

  const DGConstPlacedVolumePtr spSLab = m_SiStore->placedVolume(pathToLadder);
  HepTransform3D tHepSensor = spSLab->localAlignedToParent();
    
  HepPoint3D  PointTest(0.,0.,0.);
  return tHepSensor*PointTest;
}

HepTransform3D 
ASiSensorInfo::transToSi() const
{
//   DGPath pathToLadder = m_SiStore->findPathTo(
//      SiGeomDescription::kLadderName[m_ladder-1], 1);
   
//   pathToLadder.addNodeToEnd( DGPath::Node(
//      SiGeomDescription::kSensorName[m_ladder-1], m_sensor ) );

//   const DGConstLabVolumePtr spSLab = m_SiStore->labVolume( pathToLadder );
//   HepTransform3D tHepSLab = spSLab->localToLabAligned();

   return m_tHepSLab;
   
}

HepPoint3D
ASiSensorInfo::coorInSi(HepPoint3D SPoint) const
{
   return transToSi()*SPoint;
}

HepVector3D 
ASiSensorInfo::unitVectorZInSi() const
{
//  DGPath pathToLadder = m_SiStore->findPathTo(
//     SiGeomDescription::kLadderName[m_ladder-1], 1);
  
//  pathToLadder.addNodeToEnd( DGPath::Node(
//     SiGeomDescription::kSensorName[m_ladder-1],m_sensor ) );
  
//  const DGConstLabVolumePtr spSLab = m_SiStore->labVolume( pathToLadder );
//  HepTransform3D tHepSLab = spSLab->localToLabAligned();
  
  HepRotation rotation = m_tHepSLab.getRotation();
  
  HepPoint3D unitZ(0.,0.,1.);
  
  return rotation*unitZ;
}

HepVector3D 
ASiSensorInfo::unitVectorYInSi() const
{   
//   DGPath pathToLadder = m_SiStore->findPathTo(
//      SiGeomDescription::kLadderName[m_ladder-1], 1);
   
//   pathToLadder.addNodeToEnd( DGPath::Node( 
//      SiGeomDescription::kSensorName[m_ladder-1], m_sensor ) );
   
//   const DGConstLabVolumePtr spSLab = m_SiStore->labVolume( pathToLadder );
//   HepTransform3D tHepSLab = spSLab->localToLabAligned();
   
   HepRotation rotation = m_tHepSLab.getRotation();
   
   HepPoint3D unitY(0.,1.,0.);
   
   return rotation*unitY;
}

HepVector3D 
ASiSensorInfo::unitVectorXInSi() const
{
//   DGPath pathToLadder = m_SiStore->findPathTo( 
//      SiGeomDescription::kLadderName[m_ladder-1], 1);

//   pathToLadder.addNodeToEnd( DGPath::Node( 
//      SiGeomDescription::kSensorName[m_ladder-1],m_sensor ) );

//   const DGConstLabVolumePtr spSLab = m_SiStore->labVolume( pathToLadder );
//   HepTransform3D tHepSLab = spSLab->localToLabAligned();

   HepRotation rotation = m_tHepSLab.getRotation();

   HepPoint3D unitX(1.,0.,0.);

   return rotation*unitX;
}

float 
ASiSensorInfo::pitchZ() const
{
   return SiGeomDescription::kSensorPitchZ;
}

float 
ASiSensorInfo::pitchRphi() const
{
   return SiGeomDescription::kSensorPitchX;
}

float 
ASiSensorInfo::halfLengthX() const 
{
   return SiGeomDescription::kSensorWidth/2.;
}

float 
ASiSensorInfo::halfLengthY() const 
{
   return (*m_Store).siThickness()/2.;
}

float 
ASiSensorInfo::halfLengthZ() const
{
   return SiGeomDescription::kZStep/2.; 
}

float 
ASiSensorInfo::rstrip(float coord ) const
{
// do not look outside active area.
   assert(abs(coord)<= SiGeomDescription::kSensorActiveX);
// Active are extends +- half pitch beyound the first/last strips
   return (coord + SiGeomDescription::kSensorActiveX)/pitchRphi() + 0.5;
}

float 
ASiSensorInfo::zstrip(float coord ) const
{
// Do not look outside active area
   assert(abs(coord)<= SiGeomDescription::kSensorActiveZ);
// Active are extends +- half pitch beyound the first/last strips
   return (coord + SiGeomDescription::kSensorActiveZ)/pitchZ() + 0.5;
}
//const ASiStrip& 
//ASiSensorInfo::strip() const
//{
//  return m_strip[0];
//}

ASiStrip 
ASiSensorInfo::strip(SmallCount NSide, SmallCount NLocalStrip) const
{
  const_cast<ASiSensorInfo*>(this)->m_strip.reinit(NSide,NLocalStrip);
  return m_strip;
}

const ASiLayer&
ASiSensorInfo::layerOwner() const
{
return (*m_Store).layer( m_layer );
}

const ASiLadder&
ASiSensorInfo::ladderOwner() const
{
return (*m_Store).ladder( m_ladder );
}

const ASiHybrid&
ASiSensorInfo::hybridOwner() const
{
return (*m_Store).hybrid( m_hybrid );
}

//
// static member functions
//









