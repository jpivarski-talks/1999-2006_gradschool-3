// -*- C++ -*-
//
// Package:     <package>
// Module:      SiGeomDescription
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Alexander Undrus
// Created:     Fri May 22 21:07:49 EDT 1998
// $Id: SiGeomDescription.cc,v 1.14 2002/05/31 17:22:39 bkh Exp $
//
// Revision history
//
// $Log: SiGeomDescription.cc,v $
// Revision 1.14  2002/05/31 17:22:39  bkh
// Fix so does not erase IRGeom components
//
// Revision 1.13  2002/03/04 21:18:00  bkh
// Make parent volumes enclose children completely; revamp of ladder object
//
// Revision 1.12  2000/10/18 20:44:45  llh14
// Fixed Layer 2 Radius
//
// Revision 1.11  2000/09/26 20:36:18  llh14
// Changed Z-Gap spacing
//
// Revision 1.10  2000/05/09 13:24:28  pg
// Added constants to SiGeomDescription
//
// Revision 1.9  2000/02/03 14:45:30  pg
//   Assigned unique names for all ladders and their contents.
// So, now we have 61 separate volumes to hold ladders, with sensors
// inside getting unique names as well (61 of them). This is forced
// by GEANT requirements.
//
// Revision 1.8  2000/01/06 02:54:59  undrus
// Vees become parts of ladders; Names of Vees get the layer number
//
// Revision 1.7  2000/01/06 01:38:50  undrus
// Added Layer level in volume hierarchy
//
// Revision 1.6  1999/11/16 20:25:38  pg
// Modifications for use in Cleog3
//
// Revision 1.5  1999/07/27 22:29:53  lkg
// correct length of CarbonFiber cyl.
//
// Revision 1.4  1999/07/25 00:58:55  undrus
//  added SiAlignedGeometryProxy.  SiGeom  produces both base and aligned geometry
//
// Revision 1.3  1999/03/25 20:33:31  cleo3
//  added passive elements : Vees and Cylinder
//
// Revision 1.2  1998/12/01 03:15:49  cleo3
// added some constants to SiGeomDescription
//
// Revision 1.1.1.1  1998/08/18 07:42:04  cleo3
// imported SiGeom sources
//

#include "Experiment/Experiment.h"
#include "Experiment/report.h"
#include "Experiment/units.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "SiGeom/SiGeomDescription.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "SiGeom.SiGeomDescription" ;

//
// static data member definitions
//

const char* const 
SiGeomDescription::kVolumeNameSi = "Si";
const char* const 
SiGeomDescription::kPlacedSiName = "Si";
const char* const 
SiGeomDescription::kSiBBName = "Si Bounding Box";
const char* const 
SiGeomDescription::kNameLabFrame = "lab";
// BB = Bounding Box
const char* const 
SiGeomDescription::kLayerBBName[] = {"Si Layer 1 BB", "Si Layer 2 BB",
				     "Si Layer 3 BB", "Si Layer 4 BB"};
const char* const 
SiGeomDescription::kLayerName[] = {"Si Layer 1","Si Layer 2",
				   "Si Layer 3","Si Layer 4"};
const char* const 
SiGeomDescription::kLadderBBName[] = {"Si Ladder 1 BB","Si Ladder 2 BB",
				      "Si Ladder 3 BB","Si Ladder 4 BB"};
const char* const 
SiGeomDescription::kLadderName[]={"Si Ladder 1","Si Ladder 2","Si Ladder 3",
				  "Si Ladder 4","Si Ladder 5","Si Ladder 6",
				  "Si Ladder 7","Si Ladder 8","Si Ladder 9",
				  "Si Ladder 10","Si Ladder 11","Si Ladder 12",
				  "Si Ladder 13","Si Ladder 14","Si Ladder 15",
				  "Si Ladder 16","Si Ladder 17","Si Ladder 18",
				  "Si Ladder 19","Si Ladder 20","Si Ladder 21",
				  "Si Ladder 22","Si Ladder 23","Si Ladder 24",
				  "Si Ladder 25","Si Ladder 26","Si Ladder 27",
				  "Si Ladder 28","Si Ladder 29","Si Ladder 30",
				  "Si Ladder 31","Si Ladder 32","Si Ladder 33",
				  "Si Ladder 34","Si Ladder 35","Si Ladder 36",
				  "Si Ladder 37","Si Ladder 38","Si Ladder 39",
				  "Si Ladder 40","Si Ladder 41","Si Ladder 42",
				  "Si Ladder 43","Si Ladder 44","Si Ladder 45",
				  "Si Ladder 46","Si Ladder 47","Si Ladder 48",
				  "Si Ladder 49","Si Ladder 50","Si Ladder 51",
				  "Si Ladder 52","Si Ladder 53","Si Ladder 54",
				  "Si Ladder 55","Si Ladder 56","Si Ladder 57",
				  "Si Ladder 58","Si Ladder 59","Si Ladder 60",
				  "Si Ladder 61"};
const char* const 
SiGeomDescription::kSensorName[]={"Si Sensor 1","Si Sensor 2","Si Sensor 3",
				  "Si Sensor 4","Si Sensor 5","Si Sensor 6",
				  "Si Sensor 7","Si Sensor 8","Si Sensor 9",
				  "Si Sensor 10","Si Sensor 11","Si Sensor 12",
				  "Si Sensor 13","Si Sensor 14","Si Sensor 15",
				  "Si Sensor 16","Si Sensor 17","Si Sensor 18",
				  "Si Sensor 19","Si Sensor 20","Si Sensor 21",
				  "Si Sensor 22","Si Sensor 23","Si Sensor 24",
				  "Si Sensor 25","Si Sensor 26","Si Sensor 27",
				  "Si Sensor 28","Si Sensor 29","Si Sensor 30",
				  "Si Sensor 31","Si Sensor 32","Si Sensor 33",
				  "Si Sensor 34","Si Sensor 35","Si Sensor 36",
				  "Si Sensor 37","Si Sensor 38","Si Sensor 39",
				  "Si Sensor 40","Si Sensor 41","Si Sensor 42",
				  "Si Sensor 43","Si Sensor 44","Si Sensor 45",
				  "Si Sensor 46","Si Sensor 47","Si Sensor 48",
				  "Si Sensor 49","Si Sensor 50","Si Sensor 51",
				  "Si Sensor 52","Si Sensor 53","Si Sensor 54",
				  "Si Sensor 55","Si Sensor 56","Si Sensor 57",
				  "Si Sensor 58","Si Sensor 59","Si Sensor 60",
				  "Si Sensor 61"};
const char* const 
SiGeomDescription::kCylName = "Si support cylinder";
const char* const 
SiGeomDescription::kVeeBBName[] = { "Si Vee 1 BB","Si Vee 2 BB",
				    "Si Vee 3 BB","Si Vee 4 BB"};
const char* const 
SiGeomDescription::kVeeName[] = {"Si Vee 1","Si Vee 2","Si Vee 3","Si Vee 4"};
const char* const 
SiGeomDescription::kVeeSideName[] ={ "Si Vee Side 1","Si Vee Side 2",
				     "Si Vee Side 3","Si Vee Side 4"};

const char* const 
SiGeomDescription::kSecondaryID = "Si";

const float SiGeomDescription::kMotherRadius = 11.*k_cm;
const float SiGeomDescription::kMotherSemiHalfLength = 27.*k_cm;
const float SiGeomDescription::kCylRadius = 8.128*k_cm;
const float SiGeomDescription::kCylThickness = 0.05*k_cm;
const float SiGeomDescription::kCylSemiHalfLength = 17.09/2.0 * k_in;
// old   const float SiGeomDescription::kCylSemiHalfLength = 19.*k_cm;

const float SiGeomDescription::kVeeSide = 0.635*k_cm;
const float SiGeomDescription::kVeeThickness = 0.025*k_cm;

const float SiGeomDescription::kSensorThickness = 0.03*k_cm;
const float SiGeomDescription::kSensorWidth = 2.705*k_cm;

const float SiGeomDescription::kSensorPitchX = 0.000050;
const float SiGeomDescription::kSensorPitchZ = 0.000100;

const float SiGeomDescription::kSensorActiveX = 255.5*kSensorPitchX ;
const float SiGeomDescription::kSensorActiveZ = 255.5*kSensorPitchZ ;

const float SiGeomDescription::kSiBandGap = 3.8e-9; // 3.8 eV per pair
const float SiGeomDescription::kElectronsPerADC = 400.; // SVX chip gain
const float SiGeomDescription::kADCRange = 255.; // SVX chip gain

//Changing kZstep from 5.31 to 5.33 cm
const float SiGeomDescription::kZStep = 5.33*k_cm;
const int   SiGeomDescription::kMaxPhiStep = 26;

const int SiGeomDescription::kNZStep [SiGeomDescription::kNLayers] = 
{3, 4, 7, 10};
const int SiGeomDescription::kNZStepEast [SiGeomDescription::kNLayers] = 
{2, 2, 4, 5};

const HepVector3D SiGeomDescription::PointLayer[SiGeomDescription::kNLayers] = 
{
   HepVector3D(-0.43*k_cm, 2.515*k_cm, 0.*k_cm),
   HepVector3D(-0.52*k_cm, 3.775*k_cm, 0.*k_cm) , 
   HepVector3D( 0.*k_cm,   7.015*k_cm,  0.*k_cm), 
   HepVector3D( 0.*k_cm,   10.115*k_cm, 0.*k_cm)
}; 

const int SiGeomDescription::NPhiStep[SiGeomDescription::kNLayers] = 
{7, 10, 18, 26};

const HepVector3D SiGeomDescription::RStep[SiGeomDescription::kNLayers] = 
{
   HepVector3D(0.*k_cm,0.*k_cm,0.*k_cm),
   HepVector3D(0.*k_cm,0.*k_cm,0.*k_cm), 
   HepVector3D(0.*k_cm,0.27*k_cm,0.*k_cm), 
   HepVector3D(0.*k_cm,0.26*k_cm,0.*k_cm)
};

const float SiGeomDescription::LayerRadMin[SiGeomDescription::kNLayers]=
       {2.515*k_cm-0.2*k_cm, 3.765*k_cm-0.2*k_cm, 
        6.9*k_cm, 9.45*k_cm};
const float SiGeomDescription::LayerRadMax[SiGeomDescription::kNLayers]=
       {2.515*k_cm+1.*k_cm, 3.765*k_cm+1.*k_cm, 
        7.015*k_cm+1.*k_cm, 10.8*k_cm};

const int SiGeomDescription::kNSensorsInLadderWest[SiGeomDescription::kNLayers] = { 1,2,3,5 };
const int SiGeomDescription::kNSensorsInLadderEast[SiGeomDescription::kNLayers] = { 2,2,4,5 };
const int SiGeomDescription::kNSensorsInLadder[SiGeomDescription::kNLayers]
= { 3,4,7,10 };
const int SiGeomDescription::kNLaddersInLayer[SiGeomDescription::kNLayers]
= { 7,10,18,26 };


const double SiGeomDescription::kVeeBBHalfHeight   
( (0.84*kVeeSide+kVeeThickness/2)/2 ) ;
const double SiGeomDescription::kLadderBBHalfHeight 
( kVeeBBHalfHeight + 1.5*kSensorThickness ) ;

const double SiGeomDescription::kyLadderOffset
( -kLadderBBHalfHeight + kSensorThickness ) ;



//const float SiGeomDescription::kRStep[SiGeomDescription::kNLayers] 
//= { 0., 0., 0.0027, 0.0026 };


//
// constructors and destructor
//
SiGeomDescription::SiGeomDescription()
{
}

// SiGeomDescription::SiGeomDescription( const SiGeomDescription& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

SiGeomDescription::~SiGeomDescription()
{
}

//
// assignment operators
//
// const SiGeomDescription& SiGeomDescription::operator=( const SiGeomDescription& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

//
// static member functions
//
