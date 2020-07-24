#if !defined(IRGEOM_MASKHYBRIDGEOMETRYEVS_H)
#define IRGEOM_MASKHYBRIDGEOMETRYEVS_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskHybridGeometryEVS
// 
// Description: Create G3iExtraVolumeSpecsStore for Hybrid pipe (phase 2-3)
//              Tungsten Mask only
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 14:55:34 EDT 1999
// $Id: MaskHybridGeometryEVS.h,v 1.1.1.1 1999/07/22 14:38:41 lkg Exp $
//
// Revision history
//
// $Log: MaskHybridGeometryEVS.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:41  lkg
// First release
//

// system include files

// user include files
#include "DataHandler/Record.h"

#include "Geant3Interface/G3iTrackingMedium.h"
#include "Geant3Interface/G3iSensitiveVolume.h"
#include "Geant3Interface/G3iExtraVolSpecs.h"
#include "Geant3Interface/G3iExtraVolSpecsStore.h"
#include "Geant3Interface/G3iTrackingMediaStore.h"
#include "Geant3Interface/G3iTrackingMediaIds.h"

// forward declarations

class MaskHybridGeometryEVS
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MaskHybridGeometryEVS(const Record& iRecord);
      virtual ~MaskHybridGeometryEVS();

      // ---------- member functions ---------------------------
      void operator()(G3iExtraVolSpecsStore& store);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MaskHybridGeometryEVS( const MaskHybridGeometryEVS& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MaskHybridGeometryEVS& operator=( const MaskHybridGeometryEVS& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const Record& m_Record;								       
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "IRGeom/Template/MaskHybridGeometryEVS.cc"
//#endif

#endif /* IRGEOM_MASKHYBRIDGEOMETRYEVS_H */
