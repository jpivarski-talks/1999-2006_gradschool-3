#if !defined(IRGEOM_MASKMAINGEOMETRYEVS_H)
#define IRGEOM_MASKMAINGEOMETRYEVS_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskMainGeometryEVS
// 
// Description: Create G3iExtraVolSpecsStore for IRGeom Main 
//              Tungsten Mask only
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 14:55:51 EDT 1999
// $Id: MaskMainGeometryEVS.h,v 1.1.1.1 1999/07/22 14:38:41 lkg Exp $
//
// Revision history
//
// $Log: MaskMainGeometryEVS.h,v $
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

class MaskMainGeometryEVS
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MaskMainGeometryEVS(const Record& iRecord);
      virtual ~MaskMainGeometryEVS();

      // ---------- member functions ---------------------------
      void operator()(G3iExtraVolSpecsStore& store);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MaskMainGeometryEVS( const MaskMainGeometryEVS& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MaskMainGeometryEVS& operator=( const MaskMainGeometryEVS& ); // stop default

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
//# include "IRGeom/Template/MaskMainGeometryEVS.cc"
//#endif

#endif /* IRGEOM_MASKMAINGEOMETRYEVS_H */
