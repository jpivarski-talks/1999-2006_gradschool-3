// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiStore
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
#include "DetectorGeometry/DGLabVolume.h"
#include "DetectorGeometry/DGConstLabVolumePtr.h"
#include "DetectorGeometry/DGPath.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Transform3D.h"
// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ASiStorePro.ASiStore" ;

//
// static data member definitions
//

//
// constructors and destructor
//
ASiStore::ASiStore( const FAItem<DGDetectorStore >& iStore ) 
   : m_SiStore( iStore )
{

   m_layers = new ASiLayer[ASiSensorAddress::kAdNLayers];

   for( int kLayer = 0; kLayer < ASiSensorAddress::kAdNLayers; kLayer++ ) {
      m_layers[kLayer] = ASiLayer( m_SiStore, this, kLayer + 1 );
   }
   for(int iLadder = 1; iLadder <= ASiSensorAddress::kAdNLadders; iLadder++)
   {
      SmallCount sensor = ASiSensorAddress::ggetSensorForHybrid(iLadder-1);
      m_ladders.push_back(&m_layers[layerForSensor(sensor) - 1]
         .ladder(ladderInLayerForSensor(sensor)));
   }
   for(int iHybrid = 1; iHybrid <= ASiSensorAddress::kAdNHybrids; iHybrid++)
   {
      SmallCount isensor = ASiSensorAddress::ggetSensorForHybrid(iHybrid-1);
      m_hybrids.push_back(&m_layers[layerForSensor(isensor) - 1]
         .ladder(ladderInLayerForSensor(isensor))
         .hybrid(hybridInLadderForSensor(isensor)));
   }
   for(int iSensor = 1; iSensor <= ASiSensorAddress::kAdNSensors; iSensor++)
   {
      m_sensors.push_back(&m_layers[layerForSensor(iSensor) - 1]
         .ladder(ladderInLayerForSensor(iSensor))
         .hybrid(hybridInLadderForSensor(iSensor))
         .sensor(inHybridForSensor(iSensor)));
   }
}

// ASiStore::ASiStore(const ASiStore& ) 
// {
// }


ASiStore::~ASiStore()
{
//   cout << " CALLING DESTRUCTOR ASiStore " << endl;
   delete [] m_layers;  
} 

//
// assignment operators
//
const ASiStore& ASiStore::operator=( const ASiStore& param)
{
   if( this != &param )
   {
      m_SiStore = param.m_SiStore ;
      
      SmallCount n_Max = ASiSensorAddress::kAdNLayers;
      m_layers = new ASiLayer[n_Max];
      
      for( SmallCount kIndex(0); kIndex < n_Max; kIndex++ ) {
	 m_layers[kIndex] = param.m_layers[kIndex];
      }
   }
   
   return *this;
}

//
// member functions
//

SmallCount 
ASiStore::layerForSensor(SmallCount i_sensor) const
{
 return ASiSensorAddress::ggetLayerForSensor(i_sensor)+1;
}

SmallCount 
ASiStore::ladderForSensor(SmallCount i_sensor) const
{
   SmallCount hybrid = ASiSensorAddress::ggetHybridForSensor(i_sensor);
   SmallCount ladder = hybrid - 
      (hybrid/ASiSensorAddress::kAdNLadders)*ASiSensorAddress::kAdNLadders;
   return ladder+1;
}

SmallCount 
ASiStore::ladderForLayer(SmallCount i_layer) const
{
   SmallCount  iLadder = 0;
   
   for(int ly = 0; ly < (i_layer - 1); ly++) {  
      iLadder = iLadder + ASiSensorAddress::kAdNLaddersInLayer[ly];
   }

   return iLadder + 1;
}


SmallCount 
ASiStore::ladderInLayerForSensor(SmallCount i_sensor) const
{
   return ASiSensorAddress::ggetLadderInLayerForSensor(i_sensor) + 1;
}

SmallCount 
ASiStore::sensorForHybrid(SmallCount i_hybrid) const
{
   return ASiSensorAddress::ggetSensorForHybrid(i_hybrid-1);
}

SmallCount 
ASiStore::hybridInLadderForSensor(SmallCount i_sensor) const
{
   SmallCount hybrid = ASiSensorAddress::ggetHybridForSensor(i_sensor) + 1;
   return hybrid/(ASiSensorAddress::kAdNLadders+1) + 1;
}

SmallCount 
ASiStore::inHybridForSensor(SmallCount i_sensor) const
{

//  SmallCount gang = ASiSensorAddress::ggetGangForSensor(i_sensor);
   SmallCount lad = ladderForSensor(i_sensor);
   SmallCount inl = ASiSensorAddress::ggetInLadderForSensor(i_sensor) + 1;
   SmallCount hybr = ASiStore::hybridInLadderForSensor(i_sensor);
//  SmallCount lay = ASiSensorAddress::ggetLayerForSensor(i_sensor) + 1 ;

//  cout << " --- " << gang
//<< " --- " << lad << " --- " << inl << " --- " << hybr << " --- " << lay
//       << endl; 
//  cout << " == " << ASiSensorAddress::kAdNSensorsInLadder[ lay - 1 ] <<
// " == " <<     ASiSensorAddress::kAdNSensorsInLadderWest[ lad - 1 ] << endl;
   SmallCount ret;
   if ( hybr == 2 ) 
   {
      ret = inl - ASiSensorAddress::kAdNSensorsInLadderWest[ lad - 1 ];
   }
   else if ( hybr == 1 )
   {
      ret =  inl ;
   }
   else
   { 
//     cout << " ASiStore::InHybridForSensor : illegal number of hybrid : "
//          << hybr << endl;
     assert( 1 == hybr );
   } 
//  cout << "ASiStore::inHybridForSensor " << " * " <<  gang << " * " << 
//     inl << " * " << hybr<< " * " << lay << " * " << ret << endl;

  return ret;
}

HepPoint3D 
ASiStore::coorSensorInSi(SmallCount i_sensor) const
{
   SmallCount ladder = ladderForSensor(i_sensor);

   DGPath pathToLadder = m_SiStore->findPathTo( 
      SiGeomDescription::kLadderName[ladder-1], 1);

   pathToLadder.addNodeToEnd( DGPath::Node(
      SiGeomDescription::kSensorName[ladder-1], i_sensor) );

   const DGConstLabVolumePtr spSLab = m_SiStore->labVolume( pathToLadder );
   HepTransform3D tHepSLab = spSLab ->localToLabAligned();

   HepPoint3D PointTest(0.,0.,0.);
   return tHepSLab*PointTest;
}

SmallCount 
ASiStore::chipForLAddress( Count LAddress ) const  
{
  if ( LAddress >  ASiSensorAddress::kAdNchannelTotal || LAddress <= 0 )
  {
    report( EMERGENCY, kFacilityString )
       << " arg of chipForLAddress is " <<  LAddress
       << " must be 1 - kAdNchannelTotal " << endl;
    assert( false );
    exit(1);
  }
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
     chip = (LAddress - firstchannel)/ASiSensorAddress::kAdNchannelChip + 1 +
	(hybrid-1)*8;
  }
   return chip;
}
      
SmallCount 
ASiStore::sensorForLAddress( Count LAddress ) const 
{
  if ( LAddress >  ASiSensorAddress::kAdNchannelTotal || LAddress <= 0 )
  {
     report( EMERGENCY, kFacilityString )
	<< " arg of sensorForLAddress is " <<  LAddress
	<< " must be 1 - kAdNchannelTotal " << endl;
     assert( false );
     exit(1);
  }    
  SmallCount chip = chipForLAddress(LAddress);
  SmallCount hybrid = (chip-1)/8;
  return ASiSensorAddress::ggetSensorForHybrid(hybrid);
}
      
SmallCount 
ASiStore::localStripForLAddress( Count LAddress ) const 
{
   if ( LAddress >  ASiSensorAddress::kAdNchannelTotal || LAddress <= 0 )
   {
      report( EMERGENCY, kFacilityString )
	 << " arg of localStripForLAddress is " <<  LAddress
	 << " must be 1 - kAdNchannelTotal " << endl;
      assert( false );
      exit(1);
   }    
   SmallCount ret = LAddress - 
      ((LAddress-1)/ASiSensorAddress::kAdNchannelSensor)*
      ASiSensorAddress::kAdNchannelSensor; 
   return ret;
}
      
SmallCount 
ASiStore::sideForLAddress( Count LAddress ) const 
{
   if ( LAddress >  ASiSensorAddress::kAdNchannelTotal || LAddress <= 0 )
   {
      report( EMERGENCY, kFacilityString )
	 << " arg of localStripForLAddress is " <<  LAddress
	 << " must be 1 - kAdNchannelTotal " << endl;
      assert( false );
      exit(1);
   }    
   SmallCount side = 
      (LAddress > ASiSensorAddress::kAdZoffset) ? ZSide : RPhiSide;
   return side;
}
      
SmallCount 
ASiStore::layerForLAddress( Count LAddress ) const 
{
   if ( LAddress >  ASiSensorAddress::kAdNchannelTotal || LAddress <= 0 )
   {
      report( EMERGENCY, kFacilityString )
	 << " arg of layerForLAddress is " <<  LAddress
	 << " must be 1 - kAdNchannelTotal " << endl;
      assert( false );
      exit(1);
   }    
   
   SmallCount sensor = sensorForLAddress(LAddress);

   return layerForSensor(sensor);
}

//
// const member functions
//

const ASiLayer& 
ASiStore::layer( SmallCount iLayer ) const
{
   assert( ( 1 <= iLayer ) && ( ASiSensorAddress::kAdNLayers >= iLayer ) );
   
   return m_layers[iLayer - 1 ];
}

const ASiLadder& 
ASiStore::ladder( SmallCount iLayer, SmallCount iLadderInLayer ) const
{
   assert(( 1 <= iLayer ) && ( ASiSensorAddress::kAdNLayers >= iLayer ) );
   assert(( 1 <= iLadderInLayer ) && 
	  ( ASiSensorAddress::kAdNLaddersInLayer[iLayer-1] >= iLadderInLayer));
   
   return m_layers[iLayer - 1].ladder( iLadderInLayer );
}

const ASiLadder& 
ASiStore::ladder( SmallCount iLadder ) const
{
   assert( ( 1 <= iLadder ) && ( ASiSensorAddress::kAdNLadders >= iLadder ) );
   
   return *(m_ladders[iLadder-1]);
}

const ASiHybrid& 
ASiStore::hybrid( SmallCount iLayer, SmallCount iLadderInLayer,
		  SmallCount iHybridInLadder ) const 
{
// Check input parameters
   assert( ( 1 <= iLayer ) && ( ASiSensorAddress::kAdNLayers >= iLayer ) );
   assert(( 1 <= iLadderInLayer ) && 
	  ( ASiSensorAddress::kAdNLaddersInLayer[iLayer-1] >= iLadderInLayer));
   assert( ( FirstHybrid <= iHybridInLadder ) &&
	   ( SecondHybrid >= iHybridInLadder ) );
   
   return m_layers[iLayer - 1].ladder( iLadderInLayer ).hybrid( iHybridInLadder );
}

const ASiHybrid& 
ASiStore::hybrid( SmallCount iHybrid ) const 
{
   assert( ( 1 <= iHybrid ) && ( ASiSensorAddress::kAdNHybrids >= iHybrid ) );

   return *(m_hybrids[iHybrid-1]);
}

const ASiSensorInfo& 
ASiStore::sensor(SmallCount iLayer, SmallCount iLadderInLayer, 
		 SmallCount iHybridInLadder, SmallCount iSensorInHybrid) const 
{
   assert(( 1 <= iLayer ) && ( ASiSensorAddress::kAdNLayers >= iLayer ) );
   assert(( 1 <= iLadderInLayer ) && 
	  ( ASiSensorAddress::kAdNLaddersInLayer[iLayer-1] >= iLadderInLayer));
   assert(( FirstHybrid <= iHybridInLadder ) && 
	  ( SecondHybrid >= iHybridInLadder ) );
   assert(( 1 <= iSensorInHybrid ) && 
	  (ASiSensorAddress::kAdNSensorsInLadder[iLayer-1] >= iSensorInHybrid));
   
   return m_layers[iLayer - 1].ladder( iLadderInLayer ).hybrid( iHybridInLadder ).sensor( iSensorInHybrid );
}

const ASiSensorInfo& 
ASiStore::sensor( SmallCount iLayer, SmallCount iLadderInLayer,
		  SmallCount iSensorInLadder ) const 
{
// Check input parameters
   assert(( 1 <= iLayer ) && ( ASiSensorAddress::kAdNLayers >= iLayer ) );
   assert(( 1 <= iLadderInLayer ) && 
	  ( ASiSensorAddress::kAdNLaddersInLayer[iLayer-1] >= iLadderInLayer));
   assert( ( 1 <= iSensorInLadder ) && 
	   ( ASiSensorAddress::kAdNSensorsInLadder[iLayer - 1] >= iSensorInLadder ) );
   
   return m_layers[iLayer - 1].ladder( iLadderInLayer ).sensor( iSensorInLadder );
}

const ASiSensorInfo& 
ASiStore::sensor( SmallCount iSensor ) const 
{
   assert( ( 1 <= iSensor ) && ( ASiSensorAddress::kAdNSensors>=iSensor ) );
   
   return *(m_sensors[iSensor-1]);
}

ASiStrip 
ASiStore::strip( Count lAddress ) const 
{
  assert( ( 1 <= lAddress ) && 
	  ( ASiSensorAddress::kAdNchannelTotal>=lAddress ) );

  SmallCount iSensor = sensorForLAddress( lAddress );
  SmallCount iLocalStrip = localStripForLAddress( lAddress );
  SmallCount iSide =(lAddress>ASiSensorAddress::kAdZoffset) ? ZSide : RPhiSide;

  return sensor(iSensor).strip(iSide,iLocalStrip);
}

const STL_VECTOR( const ASiLayer* ) 
ASiStore::v_layers() const
{
      STL_VECTOR( const ASiLayer* ) temp ;
      SmallCount n_Max = ASiSensorAddress::kAdNLayers;

      for( SmallCount kIndex(1); kIndex != n_Max+1; ++kIndex ) {
	 temp.push_back(&layer(kIndex)) ;
      }
      return temp;
}

SmallCount 
ASiStore::numberOfLaddersTotal() 
{return ASiSensorAddress::kAdNLadders;} 

float 
ASiStore::siPitchPhi() {return 50.E-6;} // R-phi side pitch

float 
ASiStore::siPitchZ() {return 100.E-6;} // Z-side pitch

float 
ASiStore::siThickness() {return 300.E-6;} // Detector thickness

float 
ASiStore::siNormalCharge() 
{
// return charge in 1000 of electrons
   return siThickness()*ASiSensorAddress::kAdChargePerM/1000.;   
} 

SmallCount 
ASiStore::numberOfLaddersForLayer( SmallCount NLayer ) const
{
   return ASiSensorAddress::kAdNLaddersInLayer[NLayer - 1];  
}

SmallCount 
ASiStore::numberOfSensorsInLadderForLayer( SmallCount NLayer ) const
{
   return ASiSensorAddress::kAdNSensorsInLadder[NLayer - 1];  
}

SmallCount 
ASiStore::clamshellForHybrid( SmallCount hybrid) const
{
   SmallCount map[61] = {1,1,2,2,2,2,1,
			 1,1,1,2,2,2,2,2,1,1};
   SmallCount ladder = (hybrid-1)%61+1; // 
   return map[ladder-1];
}

//
// static member functions
//









