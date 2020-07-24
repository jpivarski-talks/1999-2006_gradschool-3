// -*- C++ -*-
//
// Package:     SiGeom
// Module:      SiGeomDescriptionEVS
// 
// Description: Fake namespace to hold SiGeomEVS 4-char identifiers
//
// Implementation:
//     Identifier naming method: <longname identifier>4
//
// Author:      Aren Jansen
// Created:     Mon Jun 14 10:34:47 EDT 1999
// $Id: SiGeomDescriptionEVS.cc,v 1.6 2000/02/03 14:45:30 pg Exp $
//
// Revision history
//
// $Log: SiGeomDescriptionEVS.cc,v $
// Revision 1.6  2000/02/03 14:45:30  pg
//   Assigned unique names for all ladders and their contents.
// So, now we have 61 separate volumes to hold ladders, with sensors
// inside getting unique names as well (61 of them). This is forced
// by GEANT requirements.
//
// Revision 1.5  2000/01/14 02:53:42  undrus
// fixed bug in calculation of east sensors; names in EVS description reduced
// to 4 symbols
//
// Revision 1.4  2000/01/06 02:55:00  undrus
// Vees become parts of ladders; Names of Vees get the layer number
//
// Revision 1.3  1999/11/16 20:25:38  pg
// Modifications for use in Cleog3
//
// Revision 1.2  1999/09/02 18:36:04  pg
// Modified ExtraVolSpecs proxy for Si to be placed inside coil, as prescribed by Brian
//
// Revision 1.1  1999/06/29 19:42:04  lkg
// Add Si3 Geant3 extra volume spec's information and associated proxy
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "SiGeom/SiGeomDescriptionEVS.h"
#include "CoilGeom/CoilAirName.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "SiGeom.SiGeomDescriptionEVS" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: SiGeomDescriptionEVS.cc,v 1.6 2000/02/03 14:45:30 pg Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//
const char* const 
SiGeomDescriptionEVS::kVolumeNameSi4 = "SI  ";
const char* const 
SiGeomDescriptionEVS::kSiBBName4 = "SIBB";
const char* const 
SiGeomDescriptionEVS::kNameLabFrame4 = kCoilAirName4;
const char* const 
SiGeomDescriptionEVS::kLayerName4[] = {"SLY1","SLY2", "SLY3", "SLY4"};
// Bounding Box for layers
const char* const 
SiGeomDescriptionEVS::kLayerBBName4[] = {"SBL1", "SBL2", "SBL3", "SBL4"};
const char* const 
// Ladders
SiGeomDescriptionEVS::kLadderName4[] = {"SD01","SD02", "SD03", "SD04",
					"SD05","SD06", "SD07", "SD08",
					"SD09","SD10", "SD11", "SD12",
					"SD13","SD14", "SD15", "SD16",
					"SD17","SD18", "SD19", "SD20",
					"SD21","SD22", "SD23", "SD24",
					"SD25","SD26", "SD27", "SD28",
					"SD29","SD30", "SD31", "SD32",
					"SD33","SD34", "SD35", "SD36",
					"SD37","SD38", "SD39", "SD40",
					"SD41","SD42", "SD43", "SD44",
					"SD45","SD46", "SD47", "SD48",
					"SD49","SD50", "SD51", "SD52",
					"SD53","SD54", "SD55", "SD56",
					"SD57","SD58", "SD59", "SD60",
					"SD61"};
// Bounding Box for ladders
const char* const 
SiGeomDescriptionEVS::kLadderBBName4[] = {"SBD1", "SBD2", "SBD3", "SBD4"};

const char* const 
SiGeomDescriptionEVS::kSensorName4[] = {"SN01","SN02", "SN03", "SN04",
					"SN05","SN06", "SN07", "SN08",
					"SN09","SN10", "SN11", "SN12",
					"SN13","SN14", "SN15", "SN16",
					"SN17","SN18", "SN19", "SN20",
					"SN21","SN22", "SN23", "SN24",
					"SN25","SN26", "SN27", "SN28",
					"SN29","SN30", "SN31", "SN32",
					"SN33","SN34", "SN35", "SN36",
					"SN37","SN38", "SN39", "SN40",
					"SN41","SN42", "SN43", "SN44",
					"SN45","SN46", "SN47", "SN48",
					"SN49","SN50", "SN51", "SN52",
					"SN53","SN54", "SN55", "SN56",
					"SN57","SN58", "SN59", "SN60",
					"SN61"};

const char* const 
SiGeomDescriptionEVS::kCylName4 = "SCYL";
const char* const 
SiGeomDescriptionEVS::kVeeBBName4[] = {"SBV1","SBV2","SBV3","SBV4"};
const char* const 
SiGeomDescriptionEVS::kVeeName4[] = {"SVE1","SVE2","SVE3","SVE4"};
const char* const 
SiGeomDescriptionEVS::kVeeSideName4[] ={"SVS1","SVS2","SVS3","SVS4"};

const char* const 
SiGeomDescriptionEVS::kSecondaryID4 = "SI  ";

//
// constructors and destructor
//
SiGeomDescriptionEVS::SiGeomDescriptionEVS()
{
}

// SiGeomDescriptionEVS::SiGeomDescriptionEVS( const SiGeomDescriptionEVS& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

SiGeomDescriptionEVS::~SiGeomDescriptionEVS()
{
}

//
// assignment operators
//
// const SiGeomDescriptionEVS& SiGeomDescriptionEVS::operator=( const SiGeomDescriptionEVS& rhs )
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
