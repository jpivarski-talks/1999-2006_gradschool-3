#if !defined(ASISTOREPRO_ASILADDER_H)
#define ASISTOREPRO_ASILADDER_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiLadder
// 
// Description: Class that holds ladders of Si detector
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

#include "DetectorGeometry/DGConstLabVolumePtr.h"

#include <vector>

// forward declarations (replace these with Class declarations)

class DGDetectorStore;
class ASiSensorAddress;
class ASiHybrid;
class ASiLayer;
class ASiStore;
class ASiSensorInfo;

class ASiLadder
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor

      ASiLadder(); 
      ASiLadder(const FAItem<DGDetectorStore >& iStore, ASiStore* iStorePro, SmallCount cl_index, SmallCount c_index); 
      const ASiLadder& operator=( const ASiLadder& );
      virtual ~ASiLadder();

      // member functions

      void setNumberOfLadder( SmallCount iIndex, SmallCount iIndexLayer );

      // const member functions
      SmallCount number() const;
      SmallCount layer() const;

      HepTransform3D transToSi() const;
      HepPoint3D coorInSi( HepPoint3D SPoint = HepPoint3D(0.,0.,0.)) const;

      HepVector3D unitVectorZInSi() const;
      HepVector3D unitVectorXInSi() const;
      HepVector3D unitVectorYInSi() const;

      HepVector3D rPhiPointer() const;
      HepVector3D zPointer() const;

      float phiInSi() const;
      float phiRotationInSi() const;
      float radiusInSi () const;
      float halfLengthX() const; 
      float halfLengthY() const; 
      float halfLengthZ() const; 

      const ASiHybrid& hybrid( SmallCount iHybridInLadder ) const;
      const ASiSensorInfo& sensor( SmallCount iHybridInLadder, SmallCount iSensorInHybrid ) const;
      const ASiSensorInfo& sensor( SmallCount iSensorInLadder ) const;
      const ASiLayer& layerOwner() const;

      const STL_VECTOR( const ASiHybrid* ) v_hybrids() const;

     // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ASiLadder( const ASiLadder& );

      // assignment operator(s)

      // private member functions

      // private const member functions

      // data members

      ASiHybrid* m_hybrids;
      FAItem < DGDetectorStore > m_SiStore;
      ASiStore* m_Store;
      SmallCount m_index;
      SmallCount m_ladder;
      SmallCount m_layer;

      // added 5/3/00
      DGConstLabVolumePtr m_spSLab;

      // static data members

};

// inline function definitions


//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ASiStorePro/Template/ASiLadder.cc"
//#endif

#endif /* ASISTOREPRO_ASILADDER_H */












