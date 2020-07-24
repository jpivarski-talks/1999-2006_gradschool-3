// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      TRSubdetectorLists
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Dec 19 17:44:59 EST 2000
// $Id: TRSubdetectorLists.cc,v 1.2 2001/01/05 05:27:32 lyon Exp $
//
// Revision history
//
// $Log: TRSubdetectorLists.cc,v $
// Revision 1.2  2001/01/05 05:27:32  lyon
// Added LayerMap and other changes to quality objects
//
// Revision 1.1  2000/12/22 06:54:11  wsun
// First submission.
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
#include "TrackRoot/TRSubdetectorLists.h"

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

static const char* const kFacilityString = "TrackRoot.TRSubdetectorLists" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: TRSubdetectorLists.cc,v 1.2 2001/01/05 05:27:32 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

DABoolean
TRSubdetectorLists::m_subdetectors[ TRSubdetectorLists::kNumberOfDetectors ]
[ TRSubdetectorLists::kNumberOfHitTypes ] =
{
   // CLEO II
   { true,    // PT
     true,    // VD
     false,   // SVR
     false,   // SVZ
     true,    // DR2
     false,    // DR axial
     false,    // DR stereo
     true,    // VD cathode
     true,    // DR cathode
     true },  // Entire
   // CLEO II.V
   { false,   // PT
     true,    // VD
     true,    // SVR
     true,    // SVZ
     true,    // DR2
     false,    // DR axial
     false,    // DR stereo
     true,     // VD Cathode
     true,    // DR cathode
     true },  // Entire
   // CLEO III
   { false,   // PT
     false,   // VD
     true,    // SVR
     true,    // SVZ
     false,   // DR2
     true,    // DR axial
     true,    // DR stereo
     false,   // VD cathode
     true,    // DR cathode
     true }   // Entire
} ;

int
TRSubdetectorLists::m_layerMapOffsets[ TRSubdetectorLists::kNumberOfDetectors ]
[ TRSubdetectorLists::kNumberOfHitTypes ] =
{
   // --- NOTE -- a "1" means bit 0 
   // CLEO II
   { 1,    // PT (layers 1-6)
     7,    // VD (layers 7-16)
     0,   // SVR (Not there)
     0,   // SVZ (Not there)
     17,  // DR2 
     0,    // DR axial (not there)
     0,    // DR stereo (not there)
     68,   // VD cathode
     70,    // DR cathode
     1 },  // Entire (starts at one)
   // CLEO II.V
   { 0,   // PT (Not there)
     7,    // VD (layers 7-16)
     1,    // SVR (layers 1-3)
     4,    // SVZ (layers 4-6)
     17,   // DR2 
     0,    // DR axial (not there)
     0,    // DR stereo (not there)
     68,   // VD cathode
     70,    // DR cathode [VDinner=68 VDouter=69 DRinner=70 DRouter=71]
     1 },  // Entire (starts at one)
   // CLEO III
   { 0,   // PT
     0,   // VD
     1,    // SVR (layers 1-4)
     5,    // SVZ (layers 5-8)
     0,    // DR2 (not there)
     9,    // DR axial (layers 9-24 [16 layers]
     25,   // DR stereo (layers 25-55 [31 layers]
     0,    // VD Cathode
     56,    // DR cathode (1)
     1 }   // Entire (starts at one)
} ;

int
TRSubdetectorLists::m_layerMapNLayers[ TRSubdetectorLists::kNumberOfDetectors ]
[ TRSubdetectorLists::kNumberOfHitTypes ] =
{
   // CLEO II
   { 6,    // PT (layers 1-6)
     10,    // VD (layers 7-16)
     0,   // SVR (Not there)
     0,   // SVZ (Not there)
     51,  // DR2
     0,    // DR III axial (not there)
     0,    // DR III  stereo (not there)
     2,    // VD Cathode
     2,    // DR cathode
     71 },  // Entire (starts at one)
   // CLEO II.V
   { 0,   // PT (Not there)
     10,    // VD (layers 7-16)
     3,    // SVR (layers 1-3)
     3,    // SVZ (layers 4-6)
     51,   // DR2
     0,    // DR III axial 
     0,    // DR III stereo
     2,    // VD cathode
     2,    // DR cathode
     71 },  // Entire (starts at one)
   // CLEO III
   { 0,   // PT (not there)
     0,   // VD (not there)
     4,    // SVR (layers 1-4)
     4,    // SVZ (layers 5-8)
     0,    // DR2 (not there)
     16,    // DR axial (layers 9-24 [16 layers]
     31,   // DR stereo (layers 25-55 [31 layers]
     0,    // VD cathode
     1,    // DR cathode (1)
     56 }   // Entire (starts at one)
} ;

int
TRSubdetectorLists::m_numberOfTypes[ TRSubdetectorLists::kNumberOfDetectors ] =
{ 6,    // CLEO II
  7,    // CLEO II.V
  6 } ; // CLEO III

int
TRSubdetectorLists::m_layerMapSizes[ TRSubdetectorLists::kNumberOfDetectors ] =
{   3,   // CLEO II needs three UINT32 words to hold layer map
    3,   // CLEO II.V needs three UINT32 words to hold layer map
    2 }; // CLEO III needs two UINT32 words to hold layer map

char*
TRSubdetectorLists::m_subdetectorNames[TRSubdetectorLists::kNumberOfHitTypes] =
{
  "PT", "VD", "SVR", "SVZ", "DR2", "DR3Axial", "DR3Stereo", "VDCathode",
  "DRCathode", "EntireDetector"
};

//
// constructors and destructor
//
TRSubdetectorLists::TRSubdetectorLists()
{
}

// TRSubdetectorLists::TRSubdetectorLists( const TRSubdetectorLists& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

TRSubdetectorLists::~TRSubdetectorLists()
{
}

//
// assignment operators
//
// const TRSubdetectorLists& TRSubdetectorLists::operator=( const TRSubdetectorLists& rhs )
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
