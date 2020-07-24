#if !defined(IRGEOM_REQMAGNETGEOMETRYEVS_H)
#define IRGEOM_REQMAGNETGEOMETRYEVS_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      REQMagnetGeometryEVS
// 
// Description: Create G3iExtraVolSpecsStore for IRGeom REQ magnet
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 14:56:02 EDT 1999
// $Id: REQMagnetGeometryEVS.h,v 1.1.1.1 1999/07/22 14:38:41 lkg Exp $
//
// Revision history
//
// $Log: REQMagnetGeometryEVS.h,v $
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

class REQMagnetGeometryEVS
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      REQMagnetGeometryEVS(const Record& iRecord);
      virtual ~REQMagnetGeometryEVS();

      // ---------- member functions ---------------------------
      void operator()(G3iExtraVolSpecsStore& store);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      REQMagnetGeometryEVS( const REQMagnetGeometryEVS& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const REQMagnetGeometryEVS& operator=( const REQMagnetGeometryEVS& ); // stop default

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
//# include "IRGeom/Template/REQMagnetGeometryEVS.cc"
//#endif

#endif /* IRGEOM_REQMAGNETGEOMETRYEVS_H */
