#if !defined(ASISTOREPRO_ASISTORE_H)
#define ASISTOREPRO_ASISTORE_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiStore
// 
// Description: Class that holds layers of Si detector
//
// Author:      Alex Undrus
// 
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "FrameAccess/FAItem.h"
#include "DataHandler/Stream.h"
#include "DetectorGeometry/DGDetectorStore.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"
#include "ASiStorePro/ASiSensorAddress.h"
#include "ASiStorePro/ASiLayer.h"

// forward declarations (replace these with Class declarations)

class DGDetectorStore;
class ASiLadder;
class ASiHybrid;
class ASiSensorInfo;
class ASiStrip;
#include "STLUtility/fwd_vector.h"

#include <vector>


class ASiStore
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      enum{ RPhiSide = 0, ZSide = 1}; 
      enum{ FirstHybrid = 1, SecondHybrid = 2};
      // Constructors and destructor
      ASiStore();
      ASiStore(const  FAItem< DGDetectorStore >& iSiStore );
      const ASiStore& operator=( const ASiStore& );

      virtual ~ASiStore();

      // member functions
  
      SmallCount layerForSensor(SmallCount NSensor) const;
      SmallCount ladderInLayerForSensor(SmallCount NSensor) const;
      SmallCount ladderForSensor(SmallCount NSensor) const;
      SmallCount ladderForLayer(SmallCount NLayer) const;
      SmallCount sensorForHybrid(SmallCount NHybrid) const;
      SmallCount hybridInLadderForSensor(SmallCount NSensor) const;
      SmallCount inHybridForSensor(SmallCount NSensor) const;

      SmallCount chipForLAddress( Count LAddress ) const;  
      SmallCount sensorForLAddress( Count LAddress ) const; 
      SmallCount localStripForLAddress( Count LAddress ) const; 
      SmallCount sideForLAddress( Count LAddress ) const; 
      SmallCount layerForLAddress( Count LAddress ) const; 

      SmallCount clamshellForHybrid( SmallCount hybrid ) const; 

      HepPoint3D coorSensorInSi(SmallCount NSensor) const;

      // const member functions

      const ASiLayer& layer( SmallCount iLayer ) const;
      const ASiLadder& ladder( SmallCount iLayer, 
			       SmallCount iLadderInLayer ) const;
      const ASiLadder& ladder( SmallCount iLadder ) const;
      const ASiHybrid& hybrid( SmallCount iLayer, SmallCount iLadderInLayer,
			       SmallCount iHybridInLadder ) const;
      const ASiHybrid& hybrid( SmallCount iHybrid ) const;
      const ASiSensorInfo& sensor( SmallCount iLayer, 
				   SmallCount iLadderInLayer, 
				   SmallCount iHybridInLadder, 
				   SmallCount iSensorInHybrid ) const;
      const ASiSensorInfo& sensor( SmallCount iLayer, 
				   SmallCount iLadderInLayer, 
				   SmallCount iSensorInLadder ) const;
      const ASiSensorInfo& sensor( SmallCount iSensor ) const;
      ASiStrip strip( Count lAddress ) const;

      const STL_VECTOR( const ASiLayer* ) v_layers() const;

      SmallCount numberOfLaddersForLayer( SmallCount NLayer ) const;  
      SmallCount numberOfSensorsInLadderForLayer( SmallCount NLayer ) const;
      // static member functions
      static SmallCount numberOfLaddersTotal(); 
      static float siPitchPhi(); 
      static float siPitchZ(); 
      static float siThickness(); 
      static float siNormalCharge(); 
   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ASiStore( const ASiStore& );

      // assignment operator(s)

      // private member functions

      // private const member functions

      // data members

      ASiLayer* m_layers;
      FAItem< DGDetectorStore > m_SiStore;
      STL_VECTOR( const ASiLadder* ) m_ladders;
      STL_VECTOR( const ASiHybrid* ) m_hybrids;
      STL_VECTOR( const ASiSensorInfo* ) m_sensors;

//      unsigned int m_aLayer[kNumberOfAxialLayers]; 
//      DABoolean m_isULayer[kNumberOfLayers];
      
      // static data members

};

// inline function definitions


//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ASiStorePro/Template/ASiStore.cc"
//#endif

#endif /* ASISTOREPRO_ASISTORE_H */
