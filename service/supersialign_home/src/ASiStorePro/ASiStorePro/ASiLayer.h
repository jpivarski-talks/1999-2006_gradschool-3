#if !defined(ASISTOREPRO_ASILAYER_H)
#define ASISTOREPRO_ASILAYER_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiLayer
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
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"
#include "STLUtility/fwd_vector.h"

#include <vector>

// forward declarations (replace these with Class declarations)

class DGDetectorStore;
class ASiStore;
class ASiSensorAddress;
class ASiLadder;
class ASiHybrid;
class ASiSensorInfo;

class ASiLayer
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      ASiLayer();
      ASiLayer(const FAItem<DGDetectorStore >& iStore, ASiStore* iStorePro, SmallCount c_index );
      const ASiLayer& operator=( const ASiLayer& );
      virtual ~ASiLayer();

      // member functions
      void setNumberOfLayer( SmallCount iIndex );

      // const member functions

      SmallCount number() const;
      SmallCount sensorEast() const; 
      SmallCount sensorWest() const; 
      SmallCount ladder() const; 
      SmallCount numberOfLadders() const;
      SmallCount numberOfSensorsInLadder() const;
      float halfLengthXOfLadder() const; 
      float halfLengthYOfLadder() const; 
      float halfLengthZOfLadder() const; 
  
      const ASiLadder& ladder( SmallCount iLadderInLayer ) const;
      const ASiHybrid& hybrid( SmallCount iLadderInLayer, SmallCount iHybridInLadder ) const; 
      const ASiSensorInfo& sensor( SmallCount iLadderInLayer, SmallCount iHybridInLadder, SmallCount iSensorInHybrid ) const;

      const STL_VECTOR( const ASiLadder* ) v_ladders() const;
      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ASiLayer( const ASiLayer& );

      // assignment operator(s)

      // private member functions

      // private const member functions

      // data members

      ASiLadder* m_ladders;
      FAItem < DGDetectorStore > m_SiStore;
      ASiStore* m_Store;
      SmallCount m_index;
      
//      unsigned int m_aLayer[kNumberOfAxialLayers]; 
//      DABoolean m_isULayer[kNumberOfLayers];
      
      // static data members

};

// inline function definitions


//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ASiStorePro/Template/ASiLayer.cc"
//#endif

#endif /* ASISTOREPRO_ASILAYER_H */












