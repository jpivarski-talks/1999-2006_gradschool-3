#if !defined(IRGEOM_IRGEOMPARAMETERS_H)
#define IRGEOM_IRGEOMPARAMETERS_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      IRGeomParameters
// 
// Description: Provides various names for the Interaction Region geometry
//
// Usage:
//    The following hierarchy is used to describe the Interaction Region,
//    where I use CDJ's convention of every line representing a
//    DGPlacedVolume with the first constant the DGVolume's name and
//    the second its CopyNumber
//    
//       kCLEO                                     IRGeometryProxy
//          kIRMotherVolName                       IRGeometryProxy
//             kAuLiningVolName                    BeamPipeGeometry
//             kOuterBePipeVolName                        -
//             kInnerBePipeVolName                        - 
//             kCuPipeVolName(W/E)                        -
//              -- or for BP23 --
//             kCuPipeVolName1(W/E)                       -
//             kCuPipeVolName2(W/E)                       -
//              -- ----------- --
//             kAlCapVolName(W/E)                         -
//             kCoolantPipeVolName                        -
//             kInnerTungstenMaskVolName(W/E)     MaskInnerGeometry
//             kMainTungstenMask(W/E)             MaskMainGeometry
//             kHybridTungstenMaskVol1Name(W/E)   MaskHybridGeometry
//             kHybridTungstenMaskVol2Name(W/E)           -
//             kREQMagnetVolName(W/E)             REQMagnetGeometry
//             kStainlessTubeVolName(W/E)         StainlessTubeGeometry
//             kCuMainStructVolName(W/E)          CuSupportGeometry
//             kCuRadialBoxVolName[kNumBoxes4](W/E)       -
//             kBeOHybridL1VolName[kNumBoxes1](W/E)       -
//                kBeOBoardVolName                        -
//                kBeOCuPadVolName(Top)                   -
//                kBeOCuPadVolName(Bottom)                -
//             kBeOHybridL2VolName[kNumBoxes2](W/E)       -
//                kBeOBoardVolName                        -
//                kBeOCuPadVolName(Top)                   -
//                kBeOCuPadVolName(Bottom)                -
//             kBeOHybridL3VolName[kNumBoxes3](W/E)       -
//                kBeOBoardVolName                        -
//                kBeOCuPadVolName(Top)                   -
//                kBeOCuPadVolName(Bottom)                -
//             kBeOHybridL4Set1VolName[kNumBoxes4](W/E)   -
//                kBeOBoardVolName                        -
//                kBeOCuPadVolName(Top)                   -
//                kBeOCuPadVolName(Bottom)                -
//             kBeOHybridL4Set2VolName[kNumBoxes4](W/E)   -
//                kBeOBoardVolName                        -
//                kBeOCuPadVolName(Top)                   -
//                kBeOCuPadVolName(Bottom)                -
//             kCFiberConeVolName(W/E)                    -
//             kAlInnerConeVolName(W/E)                   -
//             kAlRadialBoxVolName2[kAlNumBoxes2](W/E)    -       
//             kAlOuterConeVolName(W/E)                   -
//             kAlRadialBoxVolName1[kAlNumBoxes1](W/E)    -       
//             kCuInnerConeVolName(W/E)                   -
//             kCuInnerLipVolName(W/E)             -
//             kAl12NoseConeVolName(W/E)             -
//
//    The secondary ID used to retrieve the DGDetectorStore is
//       kSecondaryID = "Interaction_Region"
//
// Author:      Lawrence Gibbons
// Created:     Sun May 10 17:43:09 EDT 1998
// $Id: IRGeomParameters.h,v 1.4 2002/05/31 17:20:04 bkh Exp $
//
// Revision history
//
// $Log: IRGeomParameters.h,v $
// Revision 1.4  2002/05/31 17:20:04  bkh
// Repair so does not overlap DR or CC endcap and also
// make it one sided only and replicate that side for negative z
//
// Revision 1.3  2000/03/06 18:38:58  lkg
// Break Cu pipe into two sections to avoid GEANT limit on volume parameters
//
// Revision 1.2  1999/07/23 22:22:09  lkg
// Change "static const int" variables to enum's to accommodate SunOs/CC
//
// Revision 1.1.1.1  1999/07/22 14:38:40  lkg
// First release
//

// system include files

// user include files

// forward declarations

class IRGeomParameters
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // Lab Volume Name
      static const char* const kCLEO;

      // Interaction region mother volume name
      static const char* const kIRMotherVolName;

      // Names for the Beam Pipe of the interaction region
      static const char* const kAuLiningVolName;
      static const char* const kOuterBePipeVolName;
      static const char* const kInnerBePipeVolName;
      static const char* const kCuPipeVolName;
      static const char* const kCuPipeVolName1;
      static const char* const kCuPipeVolName2;
      static const char* const kCoolantPipeVolName;
      static const char* const kAlCapVolName;

      // Names for the Inner Tungsten Mask of the interaction region
      static const char* const kInnerTungstenMaskVolName;

      // Names for the Main Tungsten Mask of the interaction region	
      static const char* const kMainTungstenMaskVolName;
  
      // Names for the phase 2-3 (Hybrid Pipe) Tungsten Mask
      static const char* const kHybridTungstenMaskVol1Name;
      static const char* const kHybridTungstenMaskVol2Name;
      
      // Names for the REQ magnet
      static const char* const kREQMagnetVolName;
      
      // Names for the Stainless Steel Tube
      static const char* const kStainlessTubeVolName;

      // Names for the Copper/Carbon Support Structure
      enum numBoxes {
	 kNumBoxes1 = 7,
	 kNumBoxes2 = 10,
	 kNumBoxes3 = 18,
	 kNumBoxes4 = 13 // Layer 4 #Ladders/2
      };

      static const char* const kCuMainStructVolName;
      static const char* const kCuRadialBoxVolName[kNumBoxes4];

      static const char* const kBeOHybridL1VolName[kNumBoxes1];
      static const char* const kBeOHybridL2VolName[kNumBoxes2];
      static const char* const kBeOHybridL3VolName[kNumBoxes3];
      static const char* const kBeOHybridL4Set1VolName[kNumBoxes4];
      static const char* const kBeOHybridL4Set2VolName[kNumBoxes4];      
      static const char* const kBeOBoardVolName;                        
      static const char* const kBeOCuPadVolName;                  
      
      enum alNumBoxes {
	 kAlNumBoxes1 = 18,
	 kAlNumBoxes2 = 8
      };
      static const char* const kCFiberConeVolName;
      static const char* const kAlInnerConeVolName;
      static const char* const kAlOuterConeVolName;      
      static const char* const kAlRadialBoxVolName1[kAlNumBoxes1];
      static const char* const kAlRadialBoxVolName2[kAlNumBoxes2];
      static const char* const kCuInnerConeVolName;
      static const char* const kCuInnerLipVolName;
      static const char* const kAl12NoseConeVolName;
      
      static const char* const kUsageTag;
      static const char* const kSecondaryID;


      // ---------- Constructors and destructor ----------------
      IRGeomParameters();
      virtual ~IRGeomParameters();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      IRGeomParameters( const IRGeomParameters& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const IRGeomParameters& operator=( const IRGeomParameters& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "IRGeom/Template/IRGeomParameters.cc"
//#endif

#endif /* IRGEOM_IRGEOMPARAMETERS_H */



