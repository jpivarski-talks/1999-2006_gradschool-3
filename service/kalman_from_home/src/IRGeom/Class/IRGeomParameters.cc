// -*- C++ -*-
//
// Package:     IRGeom
// Module:      IRGeomParameters
// 
// Description: Hold IRGeom longname volume identifiers
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lawrence Gibbons
// Created:     Mon May 11 20:32:13 EDT 1998
// $Id: IRGeomParameters.cc,v 1.2 2000/03/06 18:38:53 lkg Exp $
//
// Revision history
//
// $Log: IRGeomParameters.cc,v $
// Revision 1.2  2000/03/06 18:38:53  lkg
// Break Cu pipe into two sections to avoid GEANT limit on volume parameters
//
// Revision 1.1.1.1  1999/07/22 14:38:42  lkg
// First release
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "IRGeom/IRGeomParameters.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "IRGeom.IRGeomParameters" ;

//
// static data member definitions
//
// Lab mother volume name
const char* const
IRGeomParameters::kCLEO                 = "lab";

// Interaction region mother volume name
const char* const
IRGeomParameters::kIRMotherVolName      = "Interaction Region";

// Names for the beam pipe of the interaction region
const char* const
IRGeomParameters::kOuterBePipeVolName    = "Outer Beryllium Beam Pipe";
const char* const
IRGeomParameters::kInnerBePipeVolName    = "Inner Beryllium Beam Pipe";
const char* const
IRGeomParameters::kCuPipeVolName         = "Copper Pipe";
const char* const
IRGeomParameters::kCuPipeVolName1        = "Copper Pipe (small z)";
const char* const
IRGeomParameters::kCuPipeVolName2        = "Copper Pipe (large z)";
const char* const
IRGeomParameters::kAuLiningVolName       = "Gold Beam Pipe Lining";
const char* const
IRGeomParameters::kCoolantPipeVolName    = "Coolant Pipe";
const char* const
IRGeomParameters::kAlCapVolName          = "Aluminum Cap";

// Names for the Inner Tungsten Mask of the interaction region
const char* const 
IRGeomParameters::kInnerTungstenMaskVolName   = "Inner Tungsten Mask";

// Names for the Main Tungsten Mask of the interaction region
const char* const 
IRGeomParameters::kMainTungstenMaskVolName    = "Main Tungsten Mask";
  
// Names for the phase 2-3 (Hybrid Pipe) Tungsten Mask
const char* const 
IRGeomParameters::kHybridTungstenMaskVol1Name 
          = "Hybrid Tungsten Mask Cone";
const char* const 
IRGeomParameters::kHybridTungstenMaskVol2Name 
          = "Hybrid Tungsten Mask Tube";
      
// Names for the REQ magnet
const char* const 
IRGeomParameters::kREQMagnetVolName           = "REQ Magnet";
      
// Names for the Stainless Steel Tube
const char* const 
IRGeomParameters::kStainlessTubeVolName       = "Stainless Steel Tube";

// Names for the Copper Support Structure
const char* const 
IRGeomParameters::kCuMainStructVolName = "Copper Support Main Structure";

const char* const 
IRGeomParameters::kCuRadialBoxVolName[] = {
   "Copper Radial Box 1",
   "Copper Radial Box 2",
   "Copper Radial Box 3",
   "Copper Radial Box 4",
   "Copper Radial Box 5",
   "Copper Radial Box 6",
   "Copper Radial Box 7",
   "Copper Radial Box 8",
   "Copper Radial Box 9",
   "Copper Radial Box 10",
   "Copper Radial Box 11",
   "Copper Radial Box 12",
   "Copper Radial Box 13" };

const char* const 
IRGeomParameters::kBeOHybridL1VolName[] = {
   "Layer 1 BeO Hybrid 1",
   "Layer 1 BeO Hybrid 2",
   "Layer 1 BeO Hybrid 3",
   "Layer 1 BeO Hybrid 4",
   "Layer 1 BeO Hybrid 5",
   "Layer 1 BeO Hybrid 6",
   "Layer 1 BeO Hybrid 7"  };

const char* const 
IRGeomParameters::kBeOHybridL2VolName[] = {
   "Layer 2 BeO Hybrid 1",
   "Layer 2 BeO Hybrid 2",
   "Layer 2 BeO Hybrid 3",
   "Layer 2 BeO Hybrid 4",
   "Layer 2 BeO Hybrid 5",
   "Layer 2 BeO Hybrid 6",
   "Layer 2 BeO Hybrid 7",
   "Layer 2 BeO Hybrid 8",
   "Layer 2 BeO Hybrid 9",
   "Layer 2 BeO Hybrid 10" };

const char* const 
IRGeomParameters::kBeOHybridL3VolName[] = {
   "Layer 3 BeO Hybrid 1",
   "Layer 3 BeO Hybrid 2",
   "Layer 3 BeO Hybrid 3",
   "Layer 3 BeO Hybrid 4",
   "Layer 3 BeO Hybrid 5",
   "Layer 3 BeO Hybrid 6",
   "Layer 3 BeO Hybrid 7",
   "Layer 3 BeO Hybrid 8",
   "Layer 3 BeO Hybrid 9",
   "Layer 3 BeO Hybrid 10",
   "Layer 3 BeO Hybrid 11",
   "Layer 3 BeO Hybrid 12",
   "Layer 3 BeO Hybrid 13",
   "Layer 3 BeO Hybrid 14",
   "Layer 3 BeO Hybrid 15",
   "Layer 3 BeO Hybrid 16",
   "Layer 3 BeO Hybrid 17",
   "Layer 3 BeO Hybrid 18" };

const char* const 
IRGeomParameters::kBeOHybridL4Set1VolName[] = {
   "Layer 4 BeO Hybrid Set One 1",
   "Layer 4 BeO Hybrid Set One 2",
   "Layer 4 BeO Hybrid Set One 3",
   "Layer 4 BeO Hybrid Set One 4",
   "Layer 4 BeO Hybrid Set One 5",
   "Layer 4 BeO Hybrid Set One 6",
   "Layer 4 BeO Hybrid Set One 7",
   "Layer 4 BeO Hybrid Set One 8",
   "Layer 4 BeO Hybrid Set One 9",
   "Layer 4 BeO Hybrid Set One 10",
   "Layer 4 BeO Hybrid Set One 11",
   "Layer 4 BeO Hybrid Set One 12",
   "Layer 4 BeO Hybrid Set One 13" };

const char* const 
IRGeomParameters::kBeOHybridL4Set2VolName[] = {
   "Layer 4 BeO Hybrid Set Two 1",
   "Layer 4 BeO Hybrid Set Two 2",
   "Layer 4 BeO Hybrid Set Two 3",
   "Layer 4 BEO Hybrid Set Two 4",
   "Layer 4 BeO Hybrid Set Two 5",
   "Layer 4 BeO Hybrid Set Two 6",
   "Layer 4 BeO Hybrid Set Two 7",
   "Layer 4 BeO Hybrid Set Two 8",
   "Layer 4 BeO Hybrid Set Two 9",
   "Layer 4 BeO Hybrid Set Two 10",
   "Layer 4 BeO Hybrid Set Two 11",
   "Layer 4 BeO Hybrid Set Two 12",
   "Layer 4 BeO Hybrid Set Two 13" };

const char* const 
IRGeomParameters::kBeOBoardVolName = "BeO Circuit Board";

const char* const 
IRGeomParameters::kBeOCuPadVolName = "BeO Hybrid Copper Mounting Pads";

const char* const 
IRGeomParameters::kAlInnerConeVolName = "Aluminum Inner Cone";

const char* const 
IRGeomParameters::kAlOuterConeVolName = "Aluminum Outer Cone";

const char* const 
IRGeomParameters::kCFiberConeVolName = "Carbon Fiber Cone";


const char* const 
IRGeomParameters::kAlRadialBoxVolName1[] = {
   "Aluminum Outer Radial Box 1", 
   "Aluminum Outer Radial Box 2",
   "Aluminum Outer Radial Box 3",
   "Aluminum Outer Radial Box 4",
   "Aluminum Outer Radial Box 5",
   "Aluminum Outer Radial Box 6",
   "Aluminum Outer Radial Box 7",
   "Aluminum Outer Radial Box 8",
   "Aluminum Outer Radial Box 9",
   "Aluminum Outer Radial Box 10",
   "Aluminum Outer Radial Box 11",
   "Aluminum Outer Radial Box 12",
   "Aluminum Outer Radial Box 13",
   "Aluminum Outer Radial Box 14",
   "Aluminum Outer Radial Box 15",
   "Aluminum Outer Radial Box 16",
   "Aluminum Outer Radial Box 17",
   "Aluminum Outer Radial Box 18" };

const char* const 
IRGeomParameters::kAlRadialBoxVolName2[] = {
   "Aluminum Inner Radial Box 1",
   "Aluminum Inner Radial Box 2",
   "Aluminum Inner Radial Box 3",
   "Aluminum Inner Radial Box 4",
   "Aluminum Inner Radial Box 5",
   "Aluminum Inner Radial Box 6",
   "Aluminum Inner Radial Box 7",
   "Aluminum Inner Radial Box 8" };

const char* const 
IRGeomParameters::kCuInnerConeVolName = "Copper Inner Cone";

const char* const 
IRGeomParameters::kCuInnerLipVolName = "Copper Inner Lip";

const char* const 
IRGeomParameters::kAl12NoseConeVolName = "Aluminum Nose Cone";

const char* const IRGeomParameters::kUsageTag = "IR3";
const char* const IRGeomParameters::kSecondaryID = "Interaction Region";
