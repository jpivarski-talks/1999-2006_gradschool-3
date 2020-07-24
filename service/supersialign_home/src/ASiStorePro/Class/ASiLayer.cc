// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiLayer
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

#include "ASiStorePro/ASiSensorAddress.h"
#include "ASiStorePro/ASiLayer.h"
#include "ASiStorePro/ASiLadder.h"
#include "ASiStorePro/ASiHybrid.h"
#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiSensorInfo.h"

#include "DetectorGeometry/DGDetectorStore.h"
#include "DetectorGeometry/DGLabVolume.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "DetectorGeometry/DGConstVolumeType.h"
#include "DetectorGeometry/DGVolumeBox.h"
#include "DetectorGeometry/DGPath.h"
#include "DetectorGeometry/DGVolumeParent.h"

#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Transform3D.h"
// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ASiStorePro.ASiLayer" ;

//
// static data member definitions
//

//
// constructors and destructor
//

ASiLayer::ASiLayer()
{
}

ASiLayer::ASiLayer( const FAItem<DGDetectorStore >& iStore, 
		    ASiStore* iStorePro, SmallCount iLayer )
   : m_SiStore( iStore ), m_Store( iStorePro ), m_index( iLayer )
{
   SmallCount n_Ladders = ASiSensorAddress::kAdNLaddersInLayer[m_index-1];
   
   m_ladders = new ASiLadder[n_Ladders];

   for( SmallCount kLadder(0); kLadder < n_Ladders; kLadder++ ) {
      m_ladders[kLadder] = ASiLadder( m_SiStore, m_Store, 
				      m_index, kLadder + 1 );
   }
}

// ASiLayer::ASiLayer(const ASiLayer& ) 
// {
// }


ASiLayer::~ASiLayer()
{
//   cout <<  " CALLING DESTRUCTOR ASiLayer " << endl;
   delete [] m_ladders;  
}

//
// assignment operators
//

 const ASiLayer& ASiLayer::operator=( const ASiLayer& param )
{
   if( this != &param )
  {
     m_SiStore = param.m_SiStore ;
     m_Store = param.m_Store ;
     m_index = param.m_index ;
     SmallCount n_Ladders = ASiSensorAddress::kAdNLaddersInLayer[m_index-1];
     m_ladders = new ASiLadder[n_Ladders];
// This is BAD - copying pointers!!
     for( SmallCount kLadder(0); kLadder < n_Ladders; kLadder++ ) {
	m_ladders[kLadder] = param.m_ladders[kLadder];
     }
  }

  return *this;
}


//
// member functions
//

void 
ASiLayer::setNumberOfLayer( SmallCount iIndex )
{
   m_index = iIndex;
}


//
// const member functions
//

SmallCount 
ASiLayer::number() const
{
   return m_index;
}

SmallCount 
ASiLayer::sensorEast() const 
{   
   SmallCount iladder = ladder();
   SmallCount sensor = ASiSensorAddress::ggetSensorForHybrid
      (iladder + ASiSensorAddress::kAdNLadders - 1);
   return sensor;
}

SmallCount 
ASiLayer::sensorWest() const 
{   
   SmallCount iladder = ladder();
   SmallCount sensor = ASiSensorAddress::ggetSensorForHybrid(iladder - 1);
   return sensor;
}

SmallCount 
ASiLayer::ladder() const
{
   SmallCount  iLadder = 0;
   
   for(int ly = 0; ly < (m_index - 1); ly++)  {  
      iLadder = iLadder + ASiSensorAddress::kAdNLaddersInLayer[ly];
   }

   return iLadder + 1;
}

SmallCount 
ASiLayer::numberOfLadders() const
{
   return ASiSensorAddress::kAdNLaddersInLayer[m_index - 1];
}

SmallCount 
ASiLayer::numberOfSensorsInLadder() const
{
   return ASiSensorAddress::kAdNSensorsInLadder[m_index - 1];
}

float 
ASiLayer::halfLengthXOfLadder() const
{

// This is stupid - they are all the same!!!
  SmallCount NLayer = m_index - 1;
  DGPath pathToLadder = m_SiStore->findPathTo( 
     SiGeomDescription::kLadderName[NLayer], 1 );
  const DGConstLabVolumePtr spLadder = m_SiStore->labVolume( pathToLadder );
  DGConstVolumePtr pVolume = spLadder -> volume();
  DGConstVolumeType< DGVolumeParent > pParent = pVolume;
   if( ! pParent.isValid() ){
     report( WARNING, kFacilityString )
    <<  "Could not convert pointer to DGVolume into pointer to DGVolumeParent "
    << endl;
    return 0;
   }

  DGConstVolumeType< DGVolumeBox > pBox =
                                          (*pParent).boundingVolume();
  float halfLengthX = (*pBox).halfLengthX();
  return halfLengthX;
}

float 
ASiLayer::halfLengthYOfLadder() const
{    
      return (*m_Store).siThickness()/2.;  
}

float 
ASiLayer::halfLengthZOfLadder() const 
{
   SmallCount NLayer = m_index - 1;
   DGPath pathToLadder = m_SiStore->findPathTo( 
      SiGeomDescription::kLadderName[NLayer], 1 );
   const DGConstLabVolumePtr spLadder = m_SiStore->labVolume( pathToLadder );
   DGConstVolumePtr pVolume = spLadder -> volume();
   DGConstVolumeType< DGVolumeParent > pParent = pVolume;
   if( ! pParent.isValid() ){
      report( WARNING, kFacilityString )
	 <<  "Could not convert DGVolume pointer into DGVolumeParent pointer "
	 << endl;
      return 0;
   }

   DGConstVolumeType< DGVolumeBox > pBox = (*pParent).boundingVolume();
   float halfLengthZ = (*pBox).halfLengthZ();
   return halfLengthZ;
}

const ASiLadder& 
ASiLayer::ladder( SmallCount iLadder ) const
{
   assert( ( 1 <= iLadder ) && ( ASiSensorAddress::kAdNLadders >= iLadder ) );

   return m_ladders[iLadder - 1 ];
}

const ASiHybrid& 
ASiLayer::hybrid( SmallCount iLadderInLayer, SmallCount iHybridInLadder ) const
{
   assert( ( 1 <= iLadderInLayer ) && 
	   ( ASiSensorAddress::kAdNLadders >= iLadderInLayer ) );
  assert( ( 1 <= iHybridInLadder ) && 
	  ( ASiSensorAddress::kAdNHybrids >= iHybridInLadder ) );

  return m_ladders[iLadderInLayer - 1 ].hybrid( iHybridInLadder );
}

const ASiSensorInfo& 
ASiLayer::sensor( SmallCount iLadderInLayer, SmallCount iHybridInLadder,
		  SmallCount iSensorInHybrid ) const
{
  assert( ( 1 <= iLadderInLayer ) && 
	  ( ASiSensorAddress::kAdNLadders >= iLadderInLayer ) );
  assert( ( 1 <= iHybridInLadder ) && 
	  ( ASiSensorAddress::kAdNHybrids >= iHybridInLadder ) );
  assert( ( 1 <= iSensorInHybrid ) && 
	  ( ASiSensorAddress::kAdNSensors>=iSensorInHybrid ) );

  return m_ladders[iLadderInLayer - 1 ].hybrid( iHybridInLadder ).sensor( iSensorInHybrid );
}

const STL_VECTOR( const ASiLadder* ) 
ASiLayer::v_ladders() const
{
// Check memory management- looks suspicious....

   STL_VECTOR( const ASiLadder* ) temp ;
   SmallCount n_Max = ASiSensorAddress::kAdNLaddersInLayer[m_index-1];
   
   for( SmallCount kIndex(1); kIndex != n_Max + 1; ++kIndex ) {
      temp.push_back(&ladder(kIndex)) ;
   }
   return temp;
}
//
// static member functions
//

 









