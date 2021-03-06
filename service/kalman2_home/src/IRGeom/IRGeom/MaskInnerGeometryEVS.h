#if !defined(IRGEOM_MASKINNERGEOMETRYEVS_H)
#define IRGEOM_MASKINNERGEOMETRYEVS_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskInnerGeometryEVS
// 
// Description: Create G3iExtraVolSpecsStore for IRGeom Inner 
//              Tungsten Mask only
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 14:55:41 EDT 1999
// $Id: MaskInnerGeometryEVS.h,v 1.1.1.1 1999/07/22 14:38:41 lkg Exp $
//
// Revision history
//
// $Log: MaskInnerGeometryEVS.h,v $
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

class MaskInnerGeometryEVS
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MaskInnerGeometryEVS(const Record& iRecord);
      virtual ~MaskInnerGeometryEVS();

      // ---------- member functions ---------------------------
      void operator()(G3iExtraVolSpecsStore& store);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MaskInnerGeometryEVS( const MaskInnerGeometryEVS& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MaskInnerGeometryEVS& operator=( const MaskInnerGeometryEVS& ); // stop default

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
//# include "IRGeom/Template/MaskInnerGeometryEVS.cc"
//#endif

#endif /* IRGEOM_MASKINNERGEOMETRYEVS_H */
