#if !defined(IRGEOM_BEAMPIPEGEOMETRYEVS_H)
#define IRGEOM_BEAMPIPEGEOMETRYEVS_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      BeamPipeGeometryEVS
// 
// Description: Create G3iExtraVolSpecsStore for Beam Pipe only
//
// Usage:
//    <usage>
//
// Author:      Aren Jansen
// Created:     Tue Jun 15 18:02:45 EDT 1999
// $Id: BeamPipeGeometryEVS.h,v 1.1.1.1 1999/07/22 14:38:40 lkg Exp $
//
// Revision history
//
// $Log: BeamPipeGeometryEVS.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:40  lkg
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

class BeamPipeGeometryEVS
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      BeamPipeGeometryEVS(const Record& iRecord);
      virtual ~BeamPipeGeometryEVS();

      // ---------- member functions ---------------------------
      void operator()(G3iExtraVolSpecsStore& store);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      BeamPipeGeometryEVS( const BeamPipeGeometryEVS& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const BeamPipeGeometryEVS& operator=( const BeamPipeGeometryEVS& ); // stop default

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
//# include "IRGeom/Template/BeamPipeGeometryEVS.cc"
//#endif

#endif /* IRGEOM_BEAMPIPEGEOMETRYEVS_H */
