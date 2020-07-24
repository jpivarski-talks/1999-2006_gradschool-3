#if !defined(ASISTOREPRO_ASISTRIP_H)
#define ASISTOREPRO_ASISTRIP_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiStrip
// 
// Description: Class that holds layers of Si detector
//
// Author:      Alex Undrus
// 
// system include files

// user include files
#include "FrameAccess/FAItem.h"
#include "DataHandler/Stream.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Geometry/Vector3D.h"

// forward declarations (replace these with Class declarations)

class DGDetectorStore;
class ASiSensorAddress;
class ASiSensorInfo;

class ASiStrip
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor

      ASiStrip(); 
      ASiStrip( const FAItem<DGDetectorStore >& iStore, SmallCount clay_index, SmallCount clad_index, SmallCount chyb_index,  SmallCount csen_index, ASiSensorInfo* c_master) ; 
      const ASiStrip& operator=( const ASiStrip& );
      virtual ~ASiStrip();
      ASiStrip( const ASiStrip& );

      // member functions

      void setLocalStrip( SmallCount iStrip );
      void setSide( SmallCount iSide );
      void reinit( SmallCount iSide, SmallCount iStrip );

      SmallCount layer() const;
      SmallCount ladderInLayer() const;
      SmallCount ladder() const;
      SmallCount sensorInLadder() const;
      SmallCount hybrid() const;
      SmallCount sensor() const;
      SmallCount gang() const;
      SmallCount side() const;
      DABoolean isRphi() const;
      DABoolean isZ() const;
      Count lAddress() const;
      SmallCount localStrip() const;
      SmallCount chip() const;
      float pitch() const;
      float localCoor() const;
  
      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions
 
   private:
      // Constructors and destructor

      // assignment operator(s)

      // private member functions

      // private const member functions

      // data members

      FAItem < DGDetectorStore > m_SiStore;
      ASiSensorInfo* m_master; 
      SmallCount      m_side;
      SmallCount      m_localStrip;
      SmallCount m_hybrid;
      SmallCount m_layer;
      SmallCount m_ladder;
      SmallCount m_sensor;

      // static data members

};

// inline function definitions


//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ASiStorePro/Template/ASiStrip.cc"
//#endif

#endif /* ASISTOREPRO_ASISTRIP_H */












