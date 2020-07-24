#if !defined(PACKAGE_SIGEOMDESCRIPTION_H)
#define PACKAGE_SIGEOMDESCRIPTION_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      SiGeomDescription
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Alexander Undrus
// Created:     Fri May 22 20:52:33 EDT 1998 
// $Id: SiGeomDescription.h,v 1.14 2002/03/04 21:18:07 bkh Exp $
//
// Revision history
//
// $Log: SiGeomDescription.h,v $
// Revision 1.14  2002/03/04 21:18:07  bkh
// Make parent volumes enclose children completely; revamp of ladder object
//
// Revision 1.13  2000/05/09 13:24:30  pg
// Added constants to SiGeomDescription
//
// Revision 1.12  2000/02/03 14:45:42  pg
//   Assigned unique names for all ladders and their contents.
// So, now we have 61 separate volumes to hold ladders, with sensors
// inside getting unique names as well (61 of them). This is forced
// by GEANT requirements.
//
// Revision 1.11  2000/01/08 00:40:54  undrus
// use hierarchical alignment constants; possibility to override the global
// transformation constants with the internal member data remains temporarily
//
// Revision 1.10  2000/01/06 02:55:10  undrus
// Vees become parts of ladders; Names of Vees get the layer number
//
// Revision 1.9  2000/01/06 01:39:01  undrus
// Added Layer level in volume hierarchy
//
// Revision 1.8  1999/11/16 20:25:43  pg
// Modifications for use in Cleog3
//
// Revision 1.7  1999/07/25 00:58:57  undrus
//  added SiAlignedGeometryProxy.  SiGeom  produces both base and aligned geometry
//
// Revision 1.6  1999/03/25 20:33:38  cleo3
//  added passive elements : Vees and Cylinder
//
// Revision 1.5  1998/12/01 03:15:50  cleo3
// added some constants to SiGeomDescription
//
// Revision 1.4  1998/11/03 02:32:25  cleo3
//  implemented Chris's DummyFriend
//
// Revision 1.3  1998/11/02 21:35:51  cleo3
//  corrected bug introduced by previous submission
//
// Revision 1.2  1998/11/02 21:26:30  cleo3
//  added default constructor to get rid of cxx warning
//
// Revision 1.1.1.1  1998/08/18 07:42:04  cleo3
// imported SiGeom sources
//

// system include files

#include "CLHEP/Geometry/Point3D.h"

// user include files

// forward declarations
class DummyFriend;

class SiGeomDescription
{
      // ---------- friend classes and functions ---------------
      friend class DummyFriend;

   public:
      // ---------- constants, enums and typedefs --------------
      enum { kNSensors = 447, kNLadders = 61, kNLayers = 4 };
      enum { kNHybrids = kNLadders*2 };
      enum { kNMaxLaddersInLayer = 26 };
      
      enum { kNFirstLayer = 1 };
      enum { kNFirstLadder = 1 };
      enum { kNFirstSensor = 1 };
      
      enum { kNchannelSensor = 512 };
      enum { kNchannelChip = 128 };
      enum { kNChip = kNHybrids*8 }; // =976
      enum { kNchannelTotal = kNChip*kNchannelChip }; // =124928
      
      static const float kMotherRadius;
      static const float kMotherSemiHalfLength;
      static const float kCylRadius;
      static const float kCylThickness;
      static const float kCylSemiHalfLength;
      static const float kVeeSide;
      static const float kVeeThickness;
      static const float kSensorThickness;

      static const float kSensorPitchX;
      static const float kSensorPitchZ;

      static const float kSensorActiveX;
      static const float kSensorActiveZ;

      static const float kSiBandGap;
      static const float kElectronsPerADC;
      static const float kADCRange;

      static const float kSensorWidth;
      static const float kZStep;
      static const int kMaxPhiStep;

      static const double kVeeBBHalfHeight ;
      static const double kLadderBBHalfHeight ;
      static const double kyLadderOffset ;


   static const int kNZStep [kNLayers];
   static const int kNZStepEast [kNLayers];
   static const HepVector3D PointLayer[kNLayers]; 
   static const int NPhiStep[kNLayers];
   static const HepVector3D RStep[kNLayers];
   static const float LayerRadMin[kNLayers];
   static const float LayerRadMax[kNLayers];
 
   static const int kNSensorsInLadderWest[kNLayers];
   static const int kNSensorsInLadderEast[kNLayers];
   static const int kNSensorsInLadder[kNLayers];
   static const int kNLaddersInLayer[kNLayers];
//   static const float kRStep[kNLayers];

      static const char* const kVolumeNameSi;
      static const char* const kPlacedSiName; 
      static const char* const kSiBBName;
      static const char* const kNameLabFrame;
      static const char* const kLayerName[kNLayers];
      static const char* const kLayerBBName[kNLayers];
      static const char* const kLadderName[61];
      static const char* const kLadderBBName[kNLayers];
      static const char* const kSensorName[61];
      static const char* const kCylName;
      static const char* const kVeeName[kNLayers];
      static const char* const kVeeBBName[kNLayers];
      static const char* const kVeeSideName[kNLayers];

      static const char* const kSecondaryID;
      
      // ---------- Constructors and destructor ----------------

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      SiGeomDescription( const SiGeomDescription& ); // stop default
      SiGeomDescription( void );                      
      virtual ~SiGeomDescription();                   

      // ---------- assignment operator(s) ---------------------
      const SiGeomDescription& operator=( const SiGeomDescription& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/SiGeomDescription.cc"
//#endif

#endif /* PACKAGE_SIGEOMDESCRIPTION_H */





