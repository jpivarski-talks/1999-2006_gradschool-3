#if !defined(ASISTOREPRO_ASIHYBRID_H)
#define ASISTOREPRO_ASIHYBRID_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiHybrid
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

// forward declarations (replace these with Class declarations)

class DGDetectorStore;
class ASiSensorAddress;
class ASiStore;
class ASiLayer;
class ASiLadder;
class ASiSensorInfo;
#include "STLUtility/fwd_vector.h"

#include <vector>



class ASiHybrid
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor

      ASiHybrid(); 
      ASiHybrid( const FAItem<DGDetectorStore >& iStore, ASiStore* iStorePro, SmallCount clay_index, SmallCount clad_index, SmallCount c_index ); 
////      ASiHybrid( SmallCount clay_index, SmallCount clad_index, SmallCount c_index ); 
      const ASiHybrid& operator=( const ASiHybrid& );
      virtual ~ASiHybrid();

      // member functions

      void setNumberOfHybrid( SmallCount iIndex, SmallCount iIndexLayer, SmallCount iIndexLadder );

      // const member functions

      SmallCount number() const;
      SmallCount sensor() const;
      SmallCount gang() const;
      SmallCount ladderInLayer() const;
      SmallCount ladder() const;
      SmallCount layer() const;

      const ASiSensorInfo& sensor( SmallCount iSensor ) const;
      const ASiLadder& ladderOwner() const;
      const ASiLayer& layerOwner() const;

      const STL_VECTOR( const ASiSensorInfo* ) v_sensors() const;

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ASiHybrid( const ASiHybrid& );

      // assignment operator(s)

      // private member functions

      // private const member functions

      // data members

      ASiSensorInfo* m_sensors;
      FAItem < DGDetectorStore > m_SiStore;
      ASiStore* m_Store;
      SmallCount m_index;
      SmallCount m_hybrid;
      SmallCount m_layer;
      SmallCount m_ladder;

//      unsigned int m_aLayer[kNumberOfAxialLayers]; 
//      DABoolean m_isULayer[kNumberOfLayers];
      
      // static data members

};

// inline function definitions


//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ASiStorePro/Template/ASiHybrid.cc"
//#endif

#endif /* ASISTOREPRO_ASIHYBRID_H */












