#if !defined(ASISTOREPRO_ASISENSORINFO_H)
#define ASISTOREPRO_ASISENSORINFO_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiSensorInfo
// 
// Description: Class that holds layers of Si detector
//
// Author:      Alex Undrus
// 
// system include files

// user include files
#include "FrameAccess/FAItem.h"
#include "DataHandler/Stream.h"
#include "ASiStorePro/ASiStrip.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"
#include "CLHEP/Geometry/Transform3D.h"

// forward declarations (replace these with Class declarations)

class ASiStore;
class DGDetectorStore;
class ASiLayer;
class ASiLadder;
class ASiHybrid;

class ASiSensorInfo
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor

      ASiSensorInfo(); 
      ASiSensorInfo( const FAItem<DGDetectorStore >& iStore, ASiStore* iStorePro, SmallCount clay_index, SmallCount clad_index, SmallCount chyb_index,  SmallCount c_index) ; 
      const ASiSensorInfo& operator=( const ASiSensorInfo& );
      virtual ~ASiSensorInfo();

      // member functions

      void setNumberOfSensor( SmallCount iIndex, SmallCount iIndexLayer, SmallCount iIndexLadder, SmallCount iIndexHybrid );

      void setStrip( SmallCount iSide, SmallCount iLocalStrip );

      SmallCount number() const ;
      SmallCount layer() const ; 
      SmallCount ladderInLayer() const;
      SmallCount ladder() const ; 
      SmallCount sensorInLadder() const;
      SmallCount hybrid() const ; 
      SmallCount gang() const;

      DABoolean isWest() const;
      DABoolean isEast() const;

      Count lAddress(SmallCount NSide, SmallCount NLocalStrip = 0) const;

      HepPoint3D coorInLadder() const;
      HepTransform3D transToSi() const;
      HepPoint3D coorInSi(HepPoint3D SPoint = HepPoint3D(0.,0.,0.)) const;

      HepVector3D unitVectorZInSi() const;
      HepVector3D unitVectorXInSi() const;
      HepVector3D unitVectorYInSi() const;

//      const ASiStrip& strip() const;
      ASiStrip strip(SmallCount NSide, SmallCount NLocalStrip) const;
 
      float pitchZ() const;
      float pitchRphi() const;
      float halfLengthX() const; 
      float halfLengthY() const; 
      float halfLengthZ() const; 

      float rstrip(float coord) const;
      float zstrip(float coord) const;
  
      const ASiLadder& ladderOwner() const;
      const ASiLayer& layerOwner() const;
      const ASiHybrid& hybridOwner() const;
      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ASiSensorInfo( const ASiSensorInfo& );

      // assignment operator(s)

      // private member functions

      // private const member functions

      // data members

      FAItem < DGDetectorStore > m_SiStore;
      ASiStore* m_Store;
      ASiStrip m_strip;
      SmallCount m_index;
      SmallCount m_hybrid;
      SmallCount m_layer;
      SmallCount m_ladder;
      SmallCount m_sensor;

      //Added on 5/8/00 for caching
      HepTransform3D m_tHepSLab;

      // static data members

};

// inline function definitions

inline SmallCount ASiSensorInfo::number() const { return m_sensor;} 
inline SmallCount ASiSensorInfo::layer() const { return m_layer;} 
inline SmallCount ASiSensorInfo::ladder() const { return m_ladder;} 
inline SmallCount ASiSensorInfo::hybrid() const { return m_hybrid;} 

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ASiStorePro/Template/ASiSensorInfo.cc"
//#endif

#endif /* ASISTOREPRO_ASISENSORINFO_H */












