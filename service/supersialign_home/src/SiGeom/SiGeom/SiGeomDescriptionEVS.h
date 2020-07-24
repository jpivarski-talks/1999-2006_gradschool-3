#if !defined(SIGEOM_SIGEOMDESCRIPTIONEVS_H)
#define SIGEOM_SIGEOMDESCRIPTIONEVS_H
// -*- C++ -*-
//
// Package:     SiGeom
// Module:      SiGeomDescriptionEVS
// 
// Description: Fake namespace to hold SiGeom extra vol specs 4-charidentifiers
//
// Usage:
//    indentifier naming method: <longname identifier>4
//
// Author:      Aren Jansen
// Created:     Mon Jun 14 10:34:43 EDT 1999
// $Id: SiGeomDescriptionEVS.h,v 1.4 2000/02/03 14:45:42 pg Exp $
//
// Revision history
//
// $Log: SiGeomDescriptionEVS.h,v $
// Revision 1.4  2000/02/03 14:45:42  pg
//   Assigned unique names for all ladders and their contents.
// So, now we have 61 separate volumes to hold ladders, with sensors
// inside getting unique names as well (61 of them). This is forced
// by GEANT requirements.
//
// Revision 1.3  2000/01/06 02:55:10  undrus
// Vees become parts of ladders; Names of Vees get the layer number
//
// Revision 1.2  1999/11/16 20:25:44  pg
// Modifications for use in Cleog3
//
// Revision 1.1  1999/06/29 19:42:18  lkg
// Add Si3 Geant3 extra volume spec's information and associated proxy
//

// system include files

// user include files

// forward declarations

class SiGeomDescriptionEVS
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      static const char* const kVolumeNameSi4;
      static const char* const kSiBBName4;
      static const char* const kNameLabFrame4;
      static const char* const kLayerName4[4];
      static const char* const kLayerBBName4[4];
      static const char* const kLadderName4[61];
      static const char* const kLadderBBName4[4];
      static const char* const kSensorName4[61];
      static const char* const kCylName4;
      static const char* const kVeeName4[4];
      static const char* const kVeeBBName4[4];
      static const char* const kVeeSideName4[4];

      static const char* const kSecondaryID4; 

      // ---------- Constructors and destructor ----------------
      SiGeomDescriptionEVS();
      virtual ~SiGeomDescriptionEVS();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      SiGeomDescriptionEVS( const SiGeomDescriptionEVS& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const SiGeomDescriptionEVS& operator=( const SiGeomDescriptionEVS& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* SIGEOM_SIGEOMDESCRIPTIONEVS_H */
