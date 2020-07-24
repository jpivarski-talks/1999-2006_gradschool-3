// -*- C++ -*-
//
// Package:     IRGeom
// Module:      IRGeomEVSParameters
// 
// Description: IRGeom EVS 4-char identifier definition
//
// Implementation:
//     naming method: <longname identifier>4
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 10:17:50 EDT 1999
// $Id: IRGeomEVSParameters.cc,v 1.3 2000/03/06 18:38:53 lkg Exp $
//
// Revision history
//
// $Log: IRGeomEVSParameters.cc,v $
// Revision 1.3  2000/03/06 18:38:53  lkg
// Break Cu pipe into two sections to avoid GEANT limit on volume parameters
//
// Revision 1.2  2000/02/02 15:11:07  lkg
// Add missing commas in initialization of kBeOHybridL2VolName4 array (which
// caused dereferencing of a null pointer).
//
// Change names for kCuRadialBoxVolName4 to avoid clashes w/ other subdetectors
//
// Revision 1.1.1.1  1999/07/22 14:38:42  lkg
// First release
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
#include "IRGeom/IRGeomEVSParameters.h"

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

static const char* const kFacilityString = "IRGeom.IRGeomEVSParameters" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: IRGeomEVSParameters.cc,v 1.3 2000/03/06 18:38:53 lkg Exp $";
static const char* const kTagString = "$Name: v01_01_02 $";

//
// static data member definitions
//

// Lab mother volume name
const char* const
IRGeomEVSParameters::kLabName4                 = "lab";

// Interaction region mother volume name
const char* const
IRGeomEVSParameters::kIRMotherVolName4      = "IR  ";

// Names for the beam pipe of the interaction region
const char* const
IRGeomEVSParameters::kOuterBePipeVolName4   = "OBBP";
const char* const
IRGeomEVSParameters::kInnerBePipeVolName4   = "IBBP";
const char* const
IRGeomEVSParameters::kCuPipeVolName4_1      = "CPI1";
const char* const
IRGeomEVSParameters::kCuPipeVolName4_2      = "CPI2";
const char* const
IRGeomEVSParameters::kAuLiningVolName4      = "GBPL";
const char* const
IRGeomEVSParameters::kCoolantPipeVolName4   = "COOL";
const char* const
IRGeomEVSParameters::kAlCapVolName4         = "ACAP";

// Names for the Inner Tungsten Mask of the interaction region
const char* const 
IRGeomEVSParameters::kInnerTungstenMaskVolName4   = "ITM ";

// Names for the Main Tungsten Mask of the interaction region
const char* const 
IRGeomEVSParameters::kMainTungstenMaskVolName4    = "MTM ";
  
// Names for the phase 2-3 (Hybrid Pipe) Tungsten Mask
const char* const 
IRGeomEVSParameters::kHybridTungstenMaskVol1Name4 = "HTM1";
const char* const 
IRGeomEVSParameters::kHybridTungstenMaskVol2Name4 = "HTM2";
      
// Names for the REQ magnet
const char* const 
IRGeomEVSParameters::kREQMagnetVolName4           = "REQM";
      
// Names for the Stainless Steel Tube
const char* const 
IRGeomEVSParameters::kStainlessTubeVolName4       = "SSTU";

// Names for the Copper Support Structure
const char* const 
IRGeomEVSParameters::kCuMainStructVolName4 = "CSMS";

const char* const 
IRGeomEVSParameters::kCuRadialBoxVolName4[] = {
   "CRB1",
   "CRB2",
   "CRB3",
   "CRB4",
   "CRB5",
   "CRB6",
   "CRB7",
   "CRB8",
   "CRB9",
   "CRBA",
   "CRBB",
   "CRBC",
   "CRBD" };


const char* const 
IRGeomEVSParameters::kBeOHybridL1VolName4[] = {
   "L1H1",
   "L1H2",
   "L1H3",
   "L1H4",
   "L1H5",
   "L1H6",
   "L1H7" };

const char* const 
IRGeomEVSParameters::kBeOHybridL2VolName4[] = {
   "L2H1",
   "L2H2",
   "L2H3",
   "L2H4",
   "L2H5",
   "L2H6",
   "L2H7",
   "L2H8",
   "L2H9",
   "L2H0" };

const char* const 
IRGeomEVSParameters::kBeOHybridL3VolName4[] = {
   "3H1 ",
   "3H2 ",
   "3H3 ",
   "3H4 ",
   "3H5 ",
   "3H6 ",
   "3H7 ",
   "3H8 ",
   "3H9 ",
   "3H10",
   "3H11",
   "3H12",
   "3H13",
   "3H14",
   "3H15",
   "3H16",
   "3H17",
   "3H18" };

const char* const 
IRGeomEVSParameters::kBeOHybridL4Set1VolName4[] = {
   "411 ",
   "412 ",
   "413 ",
   "414 ",
   "415 ",
   "416 ",
   "417 ",
   "418 ",
   "419 ",
   "4110",
   "4111",
   "4112",
   "4113" };

const char* const 
IRGeomEVSParameters::kBeOHybridL4Set2VolName4[] = {
   "421 ",
   "422 ",
   "423 ",
   "424 ",
   "425 ",
   "426 ",
   "427 ",
   "428 ",
   "429 ",
   "4210",
   "4211",
   "4212",
   "4213" };

const char* const 
IRGeomEVSParameters::kBeOBoardVolName4 = "BHCB";

const char* const 
IRGeomEVSParameters::kBeOCuPadVolName4 = "BHMP";



const char* const 
IRGeomEVSParameters::kCFiberConeVolName4 = "CFSC";

const char* const 
IRGeomEVSParameters::kAlInnerConeVolName4 = "ALIC";

const char* const 
IRGeomEVSParameters::kAlOuterConeVolName4 = "ALOC";

const char* const 
IRGeomEVSParameters::kAlRadialBoxVolName14[] = {
   "OB1 ",
   "OB2 ",
   "OB3 ",
   "OB4 ",
   "OB5 ",
   "OB6 ",
   "OB7 ",
   "OB8 ",
   "OB9 ",
   "OB10",
   "OB11",
   "OB12",
   "OB13",
   "OB14",
   "OB15",
   "OB16",
   "OB17",
   "OB18" };

const char* const 
IRGeomEVSParameters::kAlRadialBoxVolName24[] = {
   "IB1 ",
   "IB2 ",
   "IB3 ",
   "IB4 ",
   "IB5 ",
   "IB6 ",
   "IB7 ",
   "IB8 " };

const char* const 
IRGeomEVSParameters::kCuInnerConeVolName4 = "CUIC";

const char* const 
IRGeomEVSParameters::kCuInnerLipVolName4 = "CUIL";
			
const char* const 
IRGeomEVSParameters::kAl12NoseConeVolName4 = "ALNC";


