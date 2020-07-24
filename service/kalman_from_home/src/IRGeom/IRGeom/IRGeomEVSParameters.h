#if !defined(IRGEOM_IRGEOMEVSPARAMETERS_H)
#define IRGEOM_IRGEOMEVSPARAMETERS_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      IRGeomEVSParameters
// 
// Description: Provides various names for the Interaction Region ExtraVolSpecs
//
// Usage:
//    naming method: <longname identifier>4
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 10:12:32 EDT 1999
// $Id: IRGeomEVSParameters.h,v 1.3 2000/03/06 18:38:57 lkg Exp $
//
// Revision history
//
// $Log: IRGeomEVSParameters.h,v $
// Revision 1.3  2000/03/06 18:38:57  lkg
// Break Cu pipe into two sections to avoid GEANT limit on volume parameters
//
// Revision 1.2  2000/02/02 15:26:33  lkg
// Minor volume name change
//
// Revision 1.1.1.1  1999/07/22 14:38:40  lkg
// First release
//

// system include files

// user include files
#include "IRGeom/IRGeomParameters.h"

// forward declarations

class IRGeomEVSParameters
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // Lab Volume Name
      static const char* const kLabName4;

      // Interaction region mother volume name
      static const char* const kIRMotherVolName4;

      // Names for the Beam Pipe of the interaction region
      static const char* const kAuLiningVolName4;
      static const char* const kOuterBePipeVolName4;
      static const char* const kInnerBePipeVolName4;
      //      static const char* const kCuPipeVolName4;
      static const char* const kCuPipeVolName4_1;
      static const char* const kCuPipeVolName4_2;
      static const char* const kCoolantPipeVolName4;
      static const char* const kAlCapVolName4;

      // Names for the Inner Tungsten Mask of the interaction region
      static const char* const kInnerTungstenMaskVolName4;

      // Names for the Main Tungsten Mask of the interaction region
      static const char* const kMainTungstenMaskVolName4;
  
      // Names for the phase 2-3 (Hybrid Pipe) Tungsten Mask
      static const char* const kHybridTungstenMaskVol1Name4;
      static const char* const kHybridTungstenMaskVol2Name4;
      
      // Names for the REQ magnet
      static const char* const kREQMagnetVolName4;
      
      // Names for the Stainless Steel Tube
      static const char* const kStainlessTubeVolName4;    

      // Names for the Copper Support Structure
      static const char* const kCuMainStructVolName4;
      static const char* const
             kCuRadialBoxVolName4[IRGeomParameters::kNumBoxes4];

      static const char* const 
             kBeOHybridL1VolName4[IRGeomParameters::kNumBoxes1];
      static const char* const 
             kBeOHybridL2VolName4[IRGeomParameters::kNumBoxes2];
      static const char* const 
             kBeOHybridL3VolName4[IRGeomParameters::kNumBoxes3];
      static const char* const 
             kBeOHybridL4Set1VolName4[IRGeomParameters::kNumBoxes4];
      static const char* const 
             kBeOHybridL4Set2VolName4[IRGeomParameters::kNumBoxes4];      
      static const char* const kBeOBoardVolName4;                        
      static const char* const kBeOCuPadVolName4;

      static const char* const kAlInnerConeVolName4;
      static const char* const kAlOuterConeVolName4;  
      static const char* const kCFiberConeVolName4;
      static const char* const 
             kAlRadialBoxVolName14[IRGeomParameters::kAlNumBoxes1];
      static const char* const 
             kAlRadialBoxVolName24[IRGeomParameters::kAlNumBoxes2];
      static const char* const kCuInnerConeVolName4;
      static const char* const kCuInnerLipVolName4;
      static const char* const kAl12NoseConeVolName4;


      // Many volumes will appear on both East and West ends of
      // the detector
      enum { kWestCopy = 1,
	     kEastCopy = 2 };

      // ---------- Constructors and destructor ----------------
      IRGeomEVSParameters();
      virtual ~IRGeomEVSParameters();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      IRGeomEVSParameters( const IRGeomEVSParameters& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const IRGeomEVSParameters& operator=( const IRGeomEVSParameters& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "IRGeom/Template/IRGeomEVSParameters.cc"
//#endif

#endif /* IRGEOM_IRGEOMEVSPARAMETERS_H */
