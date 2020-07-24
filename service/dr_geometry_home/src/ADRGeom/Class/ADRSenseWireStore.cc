// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRSenseWireStore
// 
// Description: Makes and holds sense wire layers.
//
// Implementation:
//     
//
// Author:      Inga Karliner. Start with DRWireExample by Chris D. Jones 
// Revision history
// Revision 10/05/98 Inga Karliner (from Adam Lyon)
//     add cellPhiWidth = cell width per layer
// Revision 10/06/98 Inga Karliner
//     add layer.isAxial, .isStereo, .isStereoU, .isStereoV
// Revision 10/07/98 Inga Karliner
//     wire_in_layer = 0,NumberOfWires-1 
//               was = 1,NumberOfWires  
// Revision 10/28/98 Inga Karliner
//    store array m_wires[] for each layer
//    incorporate mods and change of names in ActiveElements
//     wire maker  AEWireLayerCylindrical::wireMk() replaces wire maker wire()
//     wire reader AEWireLayerCylindrical::wire()   accesses m_wires in layer
//     wire maker  ADRSenseWireStore::wireMk() replaces wire maker wire() 
//     wire reader ADRSenseWireStore::wire()   accesses m_wires in right layer 
// Revision 3/10/99 Inga Karliner
//     Add Constants: 
//               sagx =0, sagy=  sagy(layer) from DPP table at 0.4 yrs
//               DPP tables show sag for 0.4 years, and for 
//               calculated extrapolation for saturated 
//               and unsaturated case after 4 years.
//                                    sag (microns)
//                                   0.4yrs   4 yrs(Sat/Unsat)
//                   lyr 1            22.8     19.7/ 15.8 
//                      16           107.9    104.4/ 95.9
//                      17           128.0    125.3/118.3
//                      47           148.5    147.9/146.3

//    Add Constants: 
//              Bowing of endplates:
//              DPP tables show bowing for 0.4 years, and for 
//              calculated extrapolation for saturated 
//              and unsaturated case after 4 years. 
//    Add Constants: 
//              Field wires radii DPP dimensions_8_95.txt, Table 6 
//              define radial extent of sense wire layers
//    Add Function layerNumber(const HepPoint3D&)
//
//To Do:  Long and short bushings: all are short except
//             lyr 1+2 (axial); layer 17 (stereo)  (DPP 2/11/99)
// 
// Revision 3/11/99 I.Karliner 
//     AEWireInfo& wireObjectClosest
//          ( DABoolean& success, HepPoint3D& iPoint, unsigned int ilayer)
//          returns closest wire object for 3D point within the layer 
//          defined by radial extent, zmin, zmax
//          returns success = false if point not in the layer, 
//                               or if distance algorithm fails 
// Revision 04/05/99 Inga Karliner
//     readoutEnd =-1 for East, +1 for West 
// Revision 06/02/99 Inga Karliner
// Add:setRadiusNominal sets average layer radius  at z=0 NO SAG 
//     setPhi0Nominal   sets phi(wire0)            at z=0 NO SAG
//     radiusNominal(layer) = average layer radius at z=0 NO SAG
//     phi0Nominal(layer)    phi(wire0)            at z=0 NO SAG
//     -see layer access functions in ActiveElements/AEWireLayerCylindrical
// ToDo in ActiveElements/AEWireLayerCylindrical: 
// setZMin, setZMax, setTanStereo = average for each layer, 
// to include alignment effects 
// Revision 06/28/99 Inga Karliner
//    comment out the dump to ADRSenseWireStore.ump
// Revision 8/5/99 Inga Karliner
//    move calculations of secondary layer quantities to layer object: 
//    change pLayer->setRadius(radius0)   to pLayer->setRadius()
//           pLayer->setPhi0(phi0)        to pLayer->setPhi0()
//           pLayer->setPhi0Nominal(phi0) to pLayer->setPhi0Nominal()
//           pLayer->setTanStereoAngle(a);to pLayer->setTanStereoAngle();
//           pLayer->setZMin(ZMin);       to pLayer->setZMin();
//           pLayer->setZMax(ZMax);       to pLayer->setZMax();
// Revision 08/31/99 Inga Karliner
//     Replace polycone Rlist with DRGeomParam arrays
//         following the change in DRGeom 
// Revision 09/14/99 Inga Karliner
//     Add
//        setLayer(...)             sets    layer 
//        updateSenseWireStore(..)  updates m_layers 
//        layerNC(ilayer)           returns ref to layer for updates (private)
//     Remove obsolete functions setEast/WestEndPlate()
//     Initiatize m_layers
// 9/27/99  Cleanup
//    add data member 
//        array of pointers to the layer constants  m_PWireSurveyConstants;
//    add access functions wireSurveyConstants()
// 6/27/00 I.Karliner correct entries in Layerdata
//layer      RadiusCM             AngleDegE
//         West End Cap
//15                            .0004->-0.0004
//19        39.78   -> 39.79
//33        59.43   -> 59.42
//38        66.42   -> 66.41
//44        74.80   -> 74.79

// 7/4/00 I.Karliner: 
//   replace bowF with Bowi
//   replace radii in cm with inches from the blueprints and from
//   DPP dimensions_8_95.txt, Table 7 inches
//   replace radii precision 100 microns with precision 25 microns 
//   from DPP table 14 in  the preliminary file dimensions_tables.txt
// 7/7/00 I.Karliner 
//   replace radii in cm with inches from the blueprints and from
//   DPP dimensions_8_95.txt, Table 7 inches up to 0.001", convert
//   to cm.
//   use z-offsets from DPP dimensions_8_95.txt, Table 14
//   separate wire/pin pull into tangential and radial.
//   Agreement with DPP; 
//    radiusEC 2 10^-6m, Ycm 8 10-6m, tanscd 5 10^-6 
//    why not complete? roundoffs? 
// 7/9/00 I.Karliner fix spelling (radii) 
// 7/12/00 I.Karliner Fix z layer 40. 1157.085->1157.065
//    take out spurious +0.000002 in EC radius
//------------------------------------------------------------------------ 
// 9/11/00 I.Karliner Major Change:
//    Install Dan Peterson's wire end positions rather than calculate here:
//  
//    Old: (r, phi) sense hole positions, corrected for 
//           wedding cake twist 
//           wire pull to the side (pin, wire diameters, and stereo angle)            
//          z at the EndCap corrected for bowing, bushing length
//
//    New:  read in ideal geometry's( r,phi,z)  from Dan's file, Table 14
//          with corrections for the wire pull to the side, bowing, bushing length
//          and place them w/r to the endplate pieces. 
//          See the comment preceding layerDataDPP
// 9/19/00 I.Karliner
//         Add the closest field wire functions
// 9/27/00 I.Karliner
//         Add wire direction to the the closest field wire functions 
// 10/12/00 I.Karliner
//         Fix bug: loop over layerNC(itLayer) itLayer=1,47 (was 0,46) 
// 10/30/00 I.Karliner
//         In pointClosestFieldWireInFieldPlane & pointClosestFieldWireInSensePlane
//         -shorten the error msg when radius falls outside the allowed range
//         -provide full message including allowed range with severity=DEBUG
// 10/31/00 I.Karliner
//         Add fieldRadiusNominalDown(layer) and fieldRadiusNominalUp(layer)
// 11/03/00 I.Karliner
//         pointClosestFieldWireInSensePlane: 
//         Allow search for the closest field wire in the sense plane from the entire
//         layer (was limited to radia of the hyperboloid +/- 200 microns)
// 1/31/01 I.Karliner
//        report level INFO -> VERIFY when success reported      
// 2/01/01 I.Karliner
//        -read Table14 from constants dr3.adrgeomlayersends and overwrite  layerDataDPP
//        with the data derived from these constants.  These data serve as the input to
//        local coordinates of the wire layers: r, phi0E, phi0W, z.
//        -read LayerData from the constants dr3.adrgeomlayer. These allow to adjust sagx,y (meters)
//        Other quantities are not used at present, either because they are hardwired
//        like the readout end, or because Dan Peterson's Table 14 replaced the algorithm
//        that this function used to perform. 
// 4/05/01 I.Karliner add 
//            delete [] m_PLayerConstants;
//            delete [] m_PLayersEndsConstants;
//-----------------------------------------------------------------------
#include "Experiment/Experiment.h"
// system include files
#include "C++Std/iomanip.h" 
#include "C++Std/fstream.h"
#include <stdlib.h>   // for ::exit
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/units.h"
#include "ADRGeom/ADRSenseWireStore.h"
#include "ADRGeom/ADRWireAddress.h"

#include "ActiveElements/AEWireInfo.h"
#include "ActiveElements/AEWireLayerCylindrical.h"
#include "DetectorGeometry/DGVolumePolyCone.h"
#include "DRGeom/DRGeometryParams.h"
#include "DetectorGeometry/DGLabVolume.h"
// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ADRGeom.ADRSenseWireStore" ;

enum  LayerEndPlates { kEastPlate, kWestPlate };

//
// static data member definitions
//

//
// constructors and destructor
//
ADRSenseWireStore::ADRSenseWireStore(
                    const DGConstLabVolumePtr&  iEastWeddingCake1,
		    const DGConstLabVolumePtr&  iWestWeddingCake1,
		    const DGConstLabVolumePtr&  iEastWeddingCake2,
		    const DGConstLabVolumePtr&  iWestWeddingCake2,
		    const DGConstLabVolumePtr&  iEastWeddingCake3,
		    const DGConstLabVolumePtr&  iWestWeddingCake3,
		    const DGConstLabVolumePtr&  iEastWeddingCake4,
		    const DGConstLabVolumePtr&  iWestWeddingCake4,
		    const DGConstLabVolumePtr&  iEastWeddingCake5,
		    const DGConstLabVolumePtr&  iWestWeddingCake5,
		    const DGConstLabVolumePtr&  iEastWeddingCake6,
		    const DGConstLabVolumePtr&  iWestWeddingCake6,
		    const DGConstLabVolumePtr&  iEastWeddingCake7,
		    const DGConstLabVolumePtr&  iWestWeddingCake7,
		    const DGConstLabVolumePtr&  iEastWeddingCake8,
		    const DGConstLabVolumePtr&  iWestWeddingCake8,
		    const DGConstLabVolumePtr& iEastConicalEndPlate,
		    const DGConstLabVolumePtr& iWestConicalEndPlate,
		    ADRWireSurveyConstants** pWireSurveyConstants,
		    ADRLayerConstants** pLayerConstants,
		    ADRGeomLayersEndsConstants** pGeomLayersEnds)
  :m_layers(0),
   m_PWireSurveyConstants(pWireSurveyConstants),
   m_PLayerConstants(pLayerConstants),
   m_PLayersEndsConstants(pGeomLayersEnds)
{
   report( DEBUG, kFacilityString )
     <<"print out ADRLayerConstants in ADRSenseWireStore"<<endl
     <<"         -------------------- not used --------------------------------   USED    USED    "<< endl
     <<"         ---    replaced by results of calculations in DPP Table 14 ---                   "<< endl
     << "Lyr Nwr REnd RECcm PEDeg   PWDeg Twst Cake Zstp ZBow ZBsh ZPin PnWrRad Sgx Sgy  RFMn RFMx"<< endl;
   int ix;
   for( ix=1; ix < kNumberOfLayers+1  ; ix++)  
     {
       report( DEBUG, kFacilityString )
	 <<setw(2)<<layerConstants(ix).dRLayerNumber()<<" "
	 <<setw(3)<<layerConstants(ix).numberOfWires()<<" "
	 <<setw(3)<<layerConstants(ix).readoutEnd()<<" "
	 <<setw(5)<<layerConstants(ix).radiusEndCapCM()<<" "
	 <<setw(7)<<layerConstants(ix).phi0EastInDegrees()<<" "
	 <<setw(7)<<layerConstants(ix).phi0WestInDegrees()<<" "
	 <<setw(5)<<layerConstants(ix).twistCakeInDegrees()<<" "
	 <<setw(2)<<layerConstants(ix).cakeNumber_Axial()<<" "
	 <<setw(3)<<layerConstants(ix).zStepPolycone_StereoLayer()<<" "
	 <<setw(2)<<layerConstants(ix).zBowingInch()<<" "
	 <<setw(2)<<layerConstants(ix).zBushing()<<" "
	 <<setw(2)<<layerConstants(ix).zPin()<<" "
	 <<setw(7)<<layerConstants(ix).pin_WireRadiusInch()<<" "
	 <<setw(2)<<layerConstants(ix).sagXMicron()<<" "
	 <<setw(9)<<layerConstants(ix).sagYMicron()<<" "
	 <<setw(5)<<layerConstants(ix).fieldRadiusLessEndCapCM()<<" "
	 <<setw(2)<<layerConstants(ix).fieldRadiusMoreEndCapCM()<<endl;
     }
   report( DEBUG, kFacilityString )
     <<"print out ADRLayersEndsConstants in ADRSenseWireStore"<<endl
     << " Lyr RECmm    XWmm      YWmm    ZWmm     XEmm       YEmm phi0E phi0W"<< endl;
   for( ix=1; ix < kNumberOfLayers+1  ; ix++)  
     {
       report( DEBUG, kFacilityString )
	 <<setw(2)<<layersEndsConstants(ix).dRLayerNumber()<<"."
	 <<setw(8)<<layersEndsConstants(ix).radius0MM()<<" " 	
	 <<setw(8)<<layersEndsConstants(ix).x0_WestMM()<<" "	
	 <<setw(8)<<layersEndsConstants(ix).y0_WestMM()<<" "
	 <<setw(8)<<layersEndsConstants(ix).z0_WestMM()<<" "
	 <<setw(8)<<layersEndsConstants(ix).x0_EastMM()<<" "
	 <<setw(8)<<layersEndsConstants(ix).y0_EastMM()<<" "
	 <<setw(8)<<-atan2(layersEndsConstants(ix).y0_EastMM(),layersEndsConstants(ix).x0_EastMM())<<" "
	 <<setw(8)<< atan2(layersEndsConstants(ix).y0_WestMM(),layersEndsConstants(ix).x0_WestMM())<<" "
	 <<endl;
     }


  // constants
//---------------------------------------------+
// U,V Layers: access using uLayers() vLayers()|
//---------------------------------------------+
  unsigned int ulyrs[]  
    = {17,18,19,20,   25,26,27,28,  33,34,35,36,  41,42,43,44 };       
  unsigned int vlyrs[] 
    = { 21,22,23,24,  29,30,31,32,  37,38,39,40,  45,46,47};
  unsigned int ktLayer; 
  unsigned int mLayer ;

  for( ktLayer=0; ktLayer < kNumberOfAxialLayers; ++ktLayer ) 
    {  m_aLayer[ktLayer] = ktLayer+1 ;      }   
  for( ktLayer=0;  ktLayer < kNumberOfULayers; ++ktLayer ) 
    {  m_uLayer[ktLayer] = ulyrs[ktLayer];  }    
  for( ktLayer=0;  ktLayer < kNumberOfVLayers; ++ktLayer ) 
    {  m_vLayer[ktLayer] = vlyrs[ktLayer];  }

  for(ktLayer=0; ktLayer < kNumberOfLayers ; ++ktLayer)
    { 
      m_isULayer[ktLayer]=(false);
      m_isVLayer[ktLayer]=(false);
    }
      //-------------------------------------------------------------+
      // shift layer from ulyrs down by one because 
      // CDJ ActiveElements:: AEWireLayerCylindrical 
      //     enters layers as 0:46, but retrieves (1:47)
      //-------------------------------------------------------------+
  for( ktLayer=0;  ktLayer < kNumberOfULayers; ++ktLayer ) 
    { 
      mLayer =  m_uLayer[ktLayer]-1;
      m_isULayer[mLayer] = (true);
    }
  for( ktLayer=0;  ktLayer < kNumberOfVLayers; ++ktLayer ) 
    {  
      mLayer =  m_vLayer[ktLayer]-1;
      m_isVLayer[mLayer] = (true);
    }
//Stereo Layers (17-47):  
//The conical endplate steps holding stereo sense wires in lyrs 17-47
// have z of the following BigStep (see DRGeomPerfectProxy etc):  
  // 9-11-00 OBSOLETE:
      int polyconeZOffsetIndex[]= 
      {                                3,   7,  11,  15, 
	19,  23,  27,  31,  35,  39,  43,  47,  55,  63,  
	71,  79,  87,  95, 103, 111, 119, 127, 135, 143,
       151, 159, 167, 175, 183, 191, 191 
      };
//The data for each layer
  enum LayerData { kRadiusInCM = 0,
		   kNumberOfWires,
		   kAngleInRadiansE,                   
		   kAngleInRadiansW,
		   //		   kAngleInDegreesE,                   
		   //		   kAngleInDegreesW,
		   kzOffsetEast,
		   kzOffsetWest,
		   pin_wire_sgn,
                   ReadoutEnd
                 };
  enum LayerDataDPP { kRadiusInCMDPP = 0,
		      kAngleInRadiansEDPP,                   
		      kAngleInRadiansWDPP,
                      kZWestInMMDPP
  };

  /* 9/07/2000  Major change
     How we enter the data for the wire-end of wire 0 in all layers:

     These data is to be read from the data file basedr3.adrgeomlayersends
     after it's installed in BDL etc. 
     Until this happens, all relevant the files are in /home/inga/cleo3/Notes/
     and these numbers are hardwired.

     The data can be produced by DPP in two stages:
     A1. dr3_hole_data.f produces dimensions_8_95.txt. Table 14 in the output
        needs to be printed to the file table14tmp1.txt 
     A2. read14.f reads table14tmp1.txt file and makes a data base ascii file
        dr3.adrgeomlayersends. 
     or in one stage where dr3_hole_data.f produces dr3.adrgeomlayersends
                    	*/

  double layerDataDPP[][4] =
  {
    // radiusCM, phi0ERad, phi0WRad, zmm
  13.2105,  -.001559,  -.001559,    365.4550,
  14.5898,  -.050646,  0.047531,    365.4550,
  16.0122,  -.001643,  -.001643,    436.6640,
  17.3914,  -.045279,  0.041987,    436.6640,
  18.8417,  -.001709,  -.001709,    507.8730,
  20.2235,  -.037411,  0.033992,    507.8730,
  21.6332,  -.001761,  -.001761,    579.0820,
  23.0124,  -.034484,  0.030966,    579.0820,
  24.4475,  -.001796,  -.001796,    650.2900,
  25.8293,  -.029847,  0.026252,    650.2900,
  27.2542,  -.001827,  -.001827,    721.4990,
  28.6360,  -.026372,  0.022715,    721.4990,
  30.0355,  -.001851,  -.001851,    792.7080,
  31.4173,  -.024951,  0.021248,    792.7080,
  32.8371,  -.001870,  -.001870,    904.5570,
  34.2163,  -.022541,  0.018796,    904.5570,
  36.9974,  -.062616,  -.062616,   1101.3800,
  38.3919,  -.080139,  -.045233,   1102.6110,
  39.7863,  -.062824,  -.062824,   1105.0730,
  41.1782,  -.080420,  -.045513,   1107.5350,
  42.6108,  0.058556,  0.058556,   1111.2280,
  44.0053,  0.043220,  0.074020,   1112.4590,
  45.3972,  0.058751,  0.058751,   1114.9230,
  46.7917,  0.043479,  0.074278,   1117.3870,
  48.2192,  -.055147,  -.055147,   1119.8521,
  49.6111,  -.069047,  -.041489,   1122.3180,
  51.0055,  -.055391,  -.055391,   1124.7880,
  52.3975,  -.069290,  -.041732,   1127.2581,
  53.8224,  0.052312,  0.052312,   1129.7280,
  55.2143,  0.039960,  0.064895,   1132.2050,
  56.6088,  0.052542,  0.052542,   1134.6820,
  58.0007,  0.040190,  0.065124,   1137.1600,
  59.4231,  -.049875,  -.049875,   1139.6400,
  60.8150,  -.061365,  -.038600,   1142.1230,
  62.2095,  -.050092,  -.050092,   1144.6090,
  63.6014,  -.061583,  -.038819,   1147.0980,
  65.0213,  0.047749,  0.047749,   1149.5890,
  66.4132,  0.037381,  0.058326,   1152.0800,
  67.8051,  0.047959,  0.047959,   1154.5720,
  69.1996,  0.037590,  0.058534,   1157.0649,
  70.6144,  -.045875,  -.045875,   1159.5570,
  72.0088,  -.055671,  -.036278,   1162.0510,
  73.4008,  -.046073,  -.046073,   1164.5450,
  74.7952,  -.055870,  -.036477,   1167.0389,
  76.2100,  0.044455,  0.044455,   1169.5310,
  77.6019,  0.035522,  0.053577,   1172.0190,
  78.9938,  0.044645,  0.044645,   1174.5070
    } ;

  // calculate layerDataDPP from layersEndsConstants and overwrite 

  for(  ix=0; ix < kNumberOfLayers  ; ix++)
    { 
      if(layerDataDPP[ix][kRadiusInCMDPP]!=0.1*layersEndsConstants(ix+1).radius0MM())
	{ 
	  report( VERIFY, kFacilityString )
	    <<"layer "<<ix+1<<" radius    DPP =/=file dif "
	    <<layerDataDPP[ix][kRadiusInCMDPP]<<" "<<0.1*layersEndsConstants(ix+1).radius0MM()
	    <<layerDataDPP[ix][kRadiusInCMDPP]-0.1*layersEndsConstants(ix+1).radius0MM()<<endl;
	}
      if(layerDataDPP[ix][kAngleInRadiansEDPP]!=
	 -atan2(layersEndsConstants(ix+1).y0_EastMM(),layersEndsConstants(ix+1).x0_EastMM()))
	{ 
	  report( VERIFY, kFacilityString )
	    <<"layer "<<ix+1<<" East phi0 DPP =/=file dif "
	    <<layerDataDPP[ix][kAngleInRadiansEDPP]<<" "
	    <<-atan2(layersEndsConstants(ix+1).y0_EastMM(),layersEndsConstants(ix+1).x0_EastMM())
	    <<layerDataDPP[ix][kAngleInRadiansEDPP]+
	     atan2(layersEndsConstants(ix+1).y0_EastMM(),layersEndsConstants(ix+1).x0_EastMM())<<endl;
	}
      if(layerDataDPP[ix][kAngleInRadiansWDPP]!=
	 atan2(layersEndsConstants(ix+1).y0_WestMM(),layersEndsConstants(ix+1).x0_WestMM()))
	{ 
	  report( VERIFY, kFacilityString )
	    <<"layer "<<ix+1<<" West phi0 DPP =/=file dif "
	    <<layerDataDPP[ix][kAngleInRadiansWDPP]<<" "
	    <<atan2(layersEndsConstants(ix+1).y0_WestMM(),layersEndsConstants(ix+1).x0_WestMM())
	    <<layerDataDPP[ix][kAngleInRadiansWDPP] -
	    atan2(layersEndsConstants(ix+1).y0_WestMM(),layersEndsConstants(ix+1).x0_WestMM())<<endl;
	}	

      layerDataDPP[ix][kRadiusInCMDPP] = 0.1*layersEndsConstants(ix+1).radius0MM() ;
      layerDataDPP[ix][kAngleInRadiansEDPP] =
	-atan2(layersEndsConstants(ix+1).y0_EastMM(),layersEndsConstants(ix+1).x0_EastMM()) ;
      layerDataDPP[ix][kAngleInRadiansWDPP] =
        atan2(layersEndsConstants(ix+1).y0_WestMM(),layersEndsConstants(ix+1).x0_WestMM()) ;
      layerDataDPP[ix][kZWestInMMDPP] = layersEndsConstants(ix+1).z0_WestMM();
    }

  //-----------------------------------------------------------------------
  // Angle in Degrees (phi0) is given in endplates LOCAL coordinates
  //   Internal coordinates of the endplates vs Lab:
  //   East phi = -Lab phi  West phi = +Lab phi
  //   (see also Mats Selen's geomini.f)
  //  
  //   Phi0 -related definitions (all angles in local coordinates)
  //   Twist = 0.12 deg Wedding cake East and West are twisted to fix
  //                    position of the axial wires
  //   pin_wire = 1/2((R_Bush=0.0067")-(R_Sense=0.0008"))*(180/pi)*/r(")
  //             = (0.0059)*180/2/pi/r(")
  //               pin_wire contribution decreases | PhiELab - PhiWLab |
  //   Phi0 offset: definition of 1st wire in each layer
  //      axial layers odd:  wire closest to phi=0 at z=0 
  //                         Table7, dimensions_8_95.txt
  //   Phi0W0 =              min abs value of (Phi0W0,Phi0W0-2*pi/Nwires )
  //   Phi0E0 =  Phi0W0 
  //      axial layers even: 
  //   Phi0E  = (CellWidth-Phi0W0) 
  //   Phi0W1 =  Phi0W0 - Twist  
  //   Phi0E1 =  Phi0E0 - Twist    
  //   Phi0W  =  Phi0W1  +/- (pin_wire)
  //   Phi0E  =  Phi0E1  +/- (pin_wire)  
  //   Initialize zoffset = pin_wire_sign = 0 
  //   zoffset        filled from appropriate DRGeom component 
  //   pin_wire_sign  filled acc to axial or stereo lyr
  //------------------------------------------------------ ----------------
  double  kReadoutEast = double(AEWireLayer::kReadoutEast); 
  double  kReadoutWest = double(AEWireLayer::kReadoutWest);

  double kintocm=2.54 ;

  double layerData[][8] = 
  { 
  //RadiusCM  #Wires    AngleDegE,W     zOff E,W  pin_wire_sgn, ReadoutEnd 
    /*  replace radius value in cm from Table 7 with radius value from Table 14       
     { 13.2095,   64.0,  -0.002, -0.002, 0.000 , 0.000, +1., kReadoutEast},
    { 14.5903,   64.0,  -2.811 , 2.814, 0.000 , 0.000, +1., kReadoutWest},
    { 16.0110,   72.0,  -0.001, -0.001, 0.000 , 0.000, +1., kReadoutEast},
    { 17.3916,   72.0,  -2.499,  2.501, 0.000 , 0.000, +1., kReadoutWest},
    { 18.8422,   88.0,  -0.0009,-0.0009,0.000 , 0.000, +1., kReadoutEast},
    { 20.2228,   88.0,  -2.0449, 2.046, 0.000 , 0.000, +1., kReadoutWest},
    { 21.6330,   96.0,  -0.001, -0.001, 0.000 , 0.000, +1., kReadoutEast},
    { 23.0135,   96.0,  -1.874,  1.876, 0.000 , 0.000, +1., kReadoutWest}, 
    { 24.4477,  112.0,  -0.0003,-0.0003,0.000 , 0.000, +1., kReadoutEast},
    { 25.8282,  112.0,  -1.6063, 1.608, 0.000 , 0.000, +1., kReadoutWest},
    { 27.2550,  128.0,  -0.0005,-0.0005,0.000 , 0.000, +1., kReadoutEast},
    { 28.6355,  128.0,  -1.4055, 1.407, 0.000 , 0.000, +1., kReadoutWest},
    { 30.0367,  136.00, -0.0001,-0.0001,0.000 , 0.000, +1., kReadoutEast},
    { 31.4171,  136.00, -1.323,  1.324, 0.000 , 0.000, +1., kReadoutWest},
    { 32.8362,  152.00, -0.0004,-0.0004,0.000 , 0.000, +1., kReadoutWest},
    { 34.2166,  152.00, -1.1844, 1.184, 0.000 , 0.000, +1., kReadoutWest},
    { 36.9984,  180.00, -3.599, -3.599, 0.000 , 0.000, +1., kReadoutWest},
    { 38.3919,  180.00, -4.603, -2.603, 0.000 , 0.000, +1., kReadoutWest}, 
    { 39.7855,  180.00, -3.610, -3.610, 0.000 , 0.000, +1., kReadoutWest},
    { 41.1792,  180.00, -4.618, -2.618, 0.000 , 0.000, +1., kReadoutWest},
    { 42.6111,  204.00,  3.365,  3.365, 0.000 , 0.000, -1., kReadoutEast},
    { 44.0045,  204.00,  2.486,  4.250, 0.000 , 0.000, -1., kReadoutEast},
    { 45.3979,  204.00,  3.376,  3.376, 0.000 , 0.000, -1., kReadoutEast},
    { 46.7913,  204.00,  2.501,  4.265, 0.000 , 0.000, -1., kReadoutEast},
    { 48.2187,  228.00, -3.169, -3.169, 0.000 , 0.000, +1., kReadoutEast},
    { 49.6120,  228.00, -3.965, -2.386, 0.000 , 0.000, +1., kReadoutEast},
    { 51.0053,  228.00, -3.182, -3.182, 0.000 , 0.000, +1., kReadoutEast},
    { 52.3986,  228.00, -3.978, -2.400, 0.000 , 0.000, +1., kReadoutEast},
    { 53.8223,  252.00,  3.005,  3.005, 0.000 , 0.000, -1., kReadoutWest},
    { 55.2155,  252.00,  2.298,  3.726, 0.000 , 0.000, -1., kReadoutWest},
    { 56.6087,  252.00,  3.018,  3.018, 0.000 , 0.000, -1., kReadoutWest},
    { 58.0018,  252.00,  2.311,  3.739, 0.000 , 0.000, -1., kReadoutWest},
    { 59.4227,  276.00, -2.865, -2.865, 0.000 , 0.000, +1., kReadoutWest},
    { 60.8157,  276.00, -3.523, -2.219, 0.000 , 0.000, +1., kReadoutWest},
    { 62.2088,  276.00, -2.877, -2.877, 0.000 , 0.000, +1., kReadoutWest},
    { 63.6019,  276.00, -3.535, -2.231, 0.000 , 0.000, +1., kReadoutWest},
    { 65.0202,  300.00,  2.742,  2.742, 0.000 , 0.000, -1., kReadoutEast},
    { 66.4132,  300.00,  2.148,  3.348, 0.000 , 0.000, -1., kReadoutEast},
    { 67.8062,  300.00,  2.754,  2.754, 0.000 , 0.000, -1., kReadoutEast},
    { 69.1992,  300.00,  2.160,  3.360, 0.000 , 0.000, -1., kReadoutEast},
    { 70.6155,  324.00, -2.634, -2.634, 0.000 , 0.000, +1., kReadoutEast},
    { 72.0084,  324.00, -3.195, -2.084, 0.000 , 0.000, +1., kReadoutEast},
    { 73.4014,  324.00, -2.645, -2.645, 0.000 , 0.000, +1., kReadoutEast},
    { 74.7943,  324.00, -3.207, -2.096, 0.000 , 0.000, +1., kReadoutEast},
    { 76.2092,  348.00,  2.553,  2.553, 0.000 , 0.000, -1., kReadoutWest},
    { 77.6021,  348.00,  2.040,  3.075, 0.000 , 0.000, -1., kReadoutWest},
    { 78.9950,  348.00,  2.563,  2.563, 0.000 , 0.000, -1., kReadoutWest}*/
    { kintocm*5.201,   64.0,  -0.002, -0.002, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*5.744,   64.0,  -2.811 , 2.814, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*6.304,   72.0,  -0.001, -0.001, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*6.847,   72.0,  -2.499,  2.501, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*7.418,   88.0,  -0.0009,-0.0009,0.000 , 0.000, +1., kReadoutEast},
    { kintocm*7.962,   88.0,  -2.0449, 2.046, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*8.517,   96.0,  -0.001, -0.001, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*9.060,   96.0,  -1.874,  1.876, 0.000 , 0.000, +1., kReadoutWest}, 
    { kintocm*9.625,  112.0,  -0.0003,-0.0003,0.000 , 0.000, +1., kReadoutEast},
    { kintocm*10.169, 112.0,  -1.6063, 1.608, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*10.730, 128.0,  -0.0005,-0.0005,0.000 , 0.000, +1., kReadoutEast},
    { kintocm*11.274, 128.0,  -1.4055, 1.407, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*11.825, 136.00, -0.0001,-0.0001,0.000 , 0.000, +1., kReadoutEast},
    { kintocm*12.369, 136.00, -1.323,  1.324, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*12.928, 152.00, -0.0004,-0.0004,0.000 , 0.000, +1., kReadoutWest},
    { kintocm*13.471, 152.00, -1.1844, 1.184, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*14.566, 180.00, -3.599, -3.599, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*15.115, 180.00, -4.603, -2.603, 0.000 , 0.000, +1., kReadoutWest}, 
    { kintocm*15.664, 180.00, -3.610, -3.610, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*16.212, 180.00, -4.618, -2.618, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*16.776, 204.00,  3.365,  3.365, 0.000 , 0.000, -1., kReadoutEast},
    { kintocm*17.325, 204.00,  2.486,  4.250, 0.000 , 0.000, -1., kReadoutEast},
    { kintocm*17.873, 204.00,  3.376,  3.376, 0.000 , 0.000, -1., kReadoutEast},
    { kintocm*18.422, 204.00,  2.501,  4.265, 0.000 , 0.000, -1., kReadoutEast},
    { kintocm*18.984, 228.00, -3.169, -3.169, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*19.532, 228.00, -3.965, -2.386, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*20.081, 228.00, -3.182, -3.182, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*20.629, 228.00, -3.978, -2.400, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*21.190, 252.00,  3.005,  3.005, 0.000 , 0.000, -1., kReadoutWest},
    { kintocm*21.738, 252.00,  2.298,  3.726, 0.000 , 0.000, -1., kReadoutWest},
    { kintocm*22.287, 252.00,  3.018,  3.018, 0.000 , 0.000, -1., kReadoutWest},
    { kintocm*22.835, 252.00,  2.311,  3.739, 0.000 , 0.000, -1., kReadoutWest},
    { kintocm*23.395, 276.00, -2.865, -2.865, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*23.943, 276.00, -3.523, -2.219, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*24.492, 276.00, -2.877, -2.877, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*25.040, 276.00, -3.535, -2.231, 0.000 , 0.000, +1., kReadoutWest},
    { kintocm*25.599, 300.00,  2.742,  2.742, 0.000 , 0.000, -1., kReadoutEast},
    { kintocm*26.147, 300.00,  2.148,  3.348, 0.000 , 0.000, -1., kReadoutEast},
    { kintocm*26.695, 300.00,  2.754,  2.754, 0.000 , 0.000, -1., kReadoutEast},
    { kintocm*27.244, 300.00,  2.160,  3.360, 0.000 , 0.000, -1., kReadoutEast},
    { kintocm*27.801, 324.00, -2.634, -2.634, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*28.350, 324.00, -3.195, -2.084, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*28.898, 324.00, -2.645, -2.645, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*29.447, 324.00, -3.207, -2.096, 0.000 , 0.000, +1., kReadoutEast},
    { kintocm*30.004, 348.00,  2.553,  2.553, 0.000 , 0.000, -1., kReadoutWest},
    { kintocm*30.552, 348.00,  2.040,  3.075, 0.000 , 0.000, -1., kReadoutWest},
    { kintocm*31.100, 348.00,  2.563,  2.563, 0.000 , 0.000, -1., kReadoutWest}
  };
  
  // 9-11-00 OBSOLETE: 
  double endPlateBowInch[] =
  {  
//  BOWf 
//  0.04822, 0.04739, 0.04653, 0.04571, 0.04483,  
//  0.04401, 0.04316, 0.04233, 0.04147, 0.04064,
    //
//  0.03978, 0.03895, 0.03811, 0.03728, 0.03643,
//  0.03560, 0.03449, 0.03446, 0.03443, 0.03440
    //
//  0.03435, 0.03431, 0.03421, 0.03404, 0.03387, 
//  0.03362, 0.03316, 0.03272, 0.03222, 0.03139,
    //
//  0.03054, 0.02968, 0.02863, 0.02748, 0.02617, 
//  0.02464, 0.02303, 0.02146, 0.01979, 0.01810,
    //
//  0.01638, 0.01465, 0.01288, 0.01112, 0.00944,
//  0.00802, 0.00660
    //
//replace with BOWi - also not used, use DPP Table 14 instead of calculating
    0.05862, 0.05562, 0.05437, 0.05317, 0.05190,
    0.05069, 0.04946, 0.04825, 0.04699, 0.04578,
    //
    0.04453, 0.04333, 0.04210, 0.04089, 0.03965,
    0.03844, 0.03683, 0.03680, 0.03677, 0.03674, 
    // 
    0.03669, 0.03664, 0.03654, 0.03636, 0.03387,
    0.03591, 0.03542, 0.03494, 0.03441, 0.03352,  
    //
    0.03261, 0.03169, 0.03057, 0.02935, 0.02795, 
    0.02632, 0.02460, 0.02291, 0.02114, 0.01933,
    //
    0.01749, 0.01564, 0.01376, 0.01187, 0.01008,
    0.00857, 00.0705
  } ;


  /* install corrected z offsets due to bushing/plate thickness 17
     Sample calculation of correct IDEAL z position for lyr
     NO-LOAD Location of big end plate reference point:  +43.400 inch
     Bowing due to wire load at layer 17:                - 0.037  
     inside surface endplate relative to ref. point:     + 0.053
     thickness of endplate at layer 17:                ( + 1.145
     - 0.053 )
     bushing standoff:                                   + 0.100 
     pin jacket length, end to bulge:                    - 1.281
     pin core inset:                                     + 0.022
     ___________
     43.349 inch
     = 1.101065 meter 

     Note: temporary correction: bowing not installed fully in Dan data
   
     */   


  report( VERIFY, kFacilityString ) 
    <<"+-----------------------------------+"<<endl
    <<"| ADRGeom                           |"<<endl 
    <<"| creating r,phi0 using DPP table 14|"<<endl
    <<"| from dr3.adrgeomlayersends        |"<<endl
    <<"+-----------------------------------+"<<endl;

	  
    for( ktLayer=0;  ktLayer < kNumberOfLayers; ++ktLayer ) 
      {
        layerData[ktLayer][kRadiusInCM] = layerDataDPP[ktLayer][kRadiusInCMDPP];

	layerData[ktLayer][kAngleInRadiansE] 
	  = layerDataDPP[ktLayer][kAngleInRadiansEDPP];

	layerData[ktLayer][kAngleInRadiansW] 
	  = layerDataDPP[ktLayer][kAngleInRadiansWDPP];
      }

  enum FieldRadiusCM{rad_small, rad_big};
  double FieldRadiusCM[][2]=
  {
    //rad_small, rad_big
    {12.50,  0.},
    {13.8827,  0.},
    {15.3050,  0.},
    {16.6870,  0.},
    {18.1388,  0.}, 
    {19.5203,  0.},
    {20.9313,  0.}, 
    {22.3126,  0.},
    {23.7474,  0.}, 
    {25.1284,  0.},
    {26.5558,  0.}, 
    {27.9367,  0.},
    {29.3383,  0.}, 
    {30.7192,  0.},
    {32.1385,  0.}, 
    {33.5193, 34.90},
    {36.295,  0.}, 
    {37.6887,  0.},
    {39.0825,  0.}, 
    {40.4763,  0.},
    {41.9087,  0.},
    {43.3022,  0.},
    {44.6958,  0.}, 
    {46.0894,  0.},
    {47.5170,  0.},
    {48.9104,  0.},
    {50.3038,  0.}, 
    {51.6972,  0.},
    {53.1212,  0.}, 
    {54.5145,  0.},
    {55.9077,  0.}, 
    {57.3010,  0.},
    {58.7220,  0.},
    {60.1152,  0.},
    {61.5083,  0.}, 
    {62.9015,  0.},
    {64.3200,  0.},
    {65.7130,  0.},
    {67.1061,  0.}, 
    {68.4992,  0.},
    {69.9156,  0.},
    {71.3086,  0.},
    {72.7016,  0.}, 
    {74.0946,  0.},
    {75.5096,  0.}, 
    {76.9025,  0.},
    {78.2955,  79.70}
  };


  for( ktLayer=0; ktLayer < (kNumberOfAxialLayers-1); ++ktLayer )
    { FieldRadiusCM[ktLayer][rad_big] =FieldRadiusCM[ktLayer+1][rad_small]; 
    }
  
  for( ktLayer=kNumberOfAxialLayers; ktLayer <kNumberOfLayers-1;++ktLayer )
    { FieldRadiusCM[ktLayer][rad_big] =FieldRadiusCM[ktLayer+1][rad_small]; 
    } 
     
/*  ofstream outf("ADRSenseWireStore.dump"); */
/*      outf<<"    set    field radius small, big rad extent-/+"<<endl;
      double radMin, radPlus;
      for( ktLayer=0; ktLayer <kNumberOfLayers; ++ktLayer ) 
      { 
      radMin = (layerData[ktLayer][kRadiusInCM]
      -FieldRadiusCM[ktLayer][rad_small]);
      radPlus = (FieldRadiusCM[ktLayer][rad_big]
      -layerData[ktLayer][kRadiusInCM]);
      outf<< setw(3)<< (ktLayer+1)<<"."
      << setw(10)<<FieldRadiusCM[ktLayer][rad_small] 
      << setw(10)<<FieldRadiusCM[ktLayer][rad_big]
      <<setw(10)<<radMin<<setw(10)<< radPlus <<endl;
      }
      */
  enum Wire_Sag{sagx, sagy} ;
  double wire_Sag[][2] = 
  { 
      //sagx  sagy  
    {0.0000, 0.0000228}, 
    {0.0000, 0.0000233}, 
    {0.0000, 0.0000323}, 
    {0.0000, 0.0000328}, 
    {0.0000, 0.0000445}, 
    //
    {0.0000, 0.0000451},
    {0.0000, 0.0000548}, 
    {0.0000, 0.0000554}, 
    {0.0000, 0.0000644}, 
    {0.0000, 0.0000650}, 
    //
    {0.0000, 0.0000782}, 
    {0.0000, 0.0000789}, 
    {0.0000, 0.0000881}, 
    {0.0000, 0.0000888}, 
    {0.0000, 0.0001073}, 
    //
    {0.0000, 0.0001079}, 
    {0.0000, 0.0001280}, 
    {0.0000, 0.0001285},
    {0.0000, 0.0001291}, 
    {0.0000, 0.0001297}, 
    //
    {0.0000, 0.0001304}, 
    {0.0000, 0.0001309}, 
    {0.0000, 0.0001315}, 
    {0.0000, 0.0001321},
    {0.0000, 0.0001327}, 
    //
    {0.0000, 0.0001333}, 
    {0.0000, 0.0001340}, 
    {0.0000, 0.0001346}, 
    {0.0000, 0.0001352}, 
    {0.0000, 0.0001359},
    //
    {0.0000, 0.0001366}, 
    {0.0000, 0.0001373}, 
    {0.0000, 0.0001380}, 
    {0.0000, 0.0001388}, 
    {0.0000, 0.0001395}, 
    //
    {0.0000, 0.0001403}, 
    {0.0000, 0.0001411}, 
    {0.0000, 0.0001419}, 
    {0.0000, 0.0001427}, 
    {0.0000, 0.0001435},
    // 
    {0.0000, 0.0001443}, 
    {0.0000, 0.0001451},
    {0.0000, 0.0001459}, 
    {0.0000, 0.0001467}, 
    {0.0000, 0.0001475}, 
    //
    {0.0000, 0.0001483}, 
    {0.0000, 0.0001485}
  } ;
 
  //overwrite wire_Sag with layerConstants.sag

  for( ktLayer=0; ktLayer < kNumberOfLayers; ++ktLayer )
	 { 
	   wire_Sag[ktLayer][sagx]  = layerConstants(ktLayer+1).sagXMicron();
	   wire_Sag[ktLayer][sagy]  = layerConstants(ktLayer+1).sagYMicron();
	 }
  //-------------------------------------------------+
  // set up CellId -> layer, wire in layer map       |
  // access with WireLayer wl(CellID) =(wire,layer)  |
  // CellId        = 1, 9796                         |
  // Layer         = 1,Nlayers(47)                   |
  // Wire In Layer = 1, kNumberOfWires               |
  //-------------------------------------------------+

  // First CellId in each layer:
  m_firstCellInLayer[0] = 1;  
  m_lastCellInLayer[kNumberOfLayers-1]  = kNumberOfWiresTotal;
  for (ktLayer=1; ktLayer <kNumberOfLayers; ++ktLayer)
    {  
      m_firstCellInLayer[ktLayer] = m_firstCellInLayer[ktLayer-1]
	                   + layerData[ktLayer-1][kNumberOfWires];
      m_lastCellInLayer[ktLayer-1] =m_firstCellInLayer[ktLayer]-1; 
    }

  // CellId -> layer
  for( ktLayer=0 ; ktLayer <kNumberOfLayers ;  ++ktLayer)
    { 
      for( unsigned int cellID=  m_firstCellInLayer[ktLayer]-1;
	      cellID < m_lastCellInLayer[ktLayer] ; ++cellID )
	{ m_cellToLayer[cellID] = ktLayer+1;
	}
    }

// ================= debug layer(CellId), First/Last CellID(Layer)
//  for (ktLayer=0; ktLayer <kNumberOfLayers; ++ktLayer)
//    { 
  //   report( INFO, kFacilityString ) 
  //<< "layer, First Last CellID "
//	 <<setw(3) << ktLayer+1
//	 <<setw(6) << m_firstCellInLayer[ktLayer]
//	 <<setw(6) << m_lastCellInLayer[ktLayer]<<endl;
//    }
// ================= end debug layer(CellId), First/Last CellID(Layer)


  /*
  +---------------------------------------------------------+
  |   Set up Phi0=phi of the 1st wire in each layer         |
  +---------------------------------------------------------+
  |                                                         |
  |   See comment  "9/11/2000  Major change"                | 
  |                   OBSOLETE:                             | 
  |  Input:  tabulated Phi0, r-pin, r-sense wire , Twist    | 
  |  Stereo Layers: pin-sensewire, to decrease |PhiE-PhiW|  |
  |  Axial  Layers: Twist and  pin-sensewire                |
  |  with (-.12)/(+.12) West/East Twist (Lab)               |
  |  in the Lab coordinates East phi > West Phi,            |
  |  (Lab)   East-> East - PinWire; West -> West + PinWire  |
  |  (Local) East-> East + PinWire; West -> West + PinWire  |  
  |  7/7/2000 corrections:                                  |
  |  separate the wire pull into tangential and radial      |
  |  radial=- PinWire*sin(abs(phiE-PhiW)/2)                 |
  |   tang  =  PinWire*cos((phiE-PhiW)/2)/LayerRadius       |
  |           = change in Phi0                              |
  |  remove the calculation of twist contribution to phi0   |
  |  and the wire pull correction to radius and phi0        |
  |  twist = -0.12 degrees                                  |
  |  pin diameter - wire diameter = 0.000295"               |
  |  const double Twist = - 0.12   ;                        |
  |  const double PinWire = 0.00295*2.54 ;                  |
  +---------------------------------------------------------+
    */


  //------------------------------------------------------------+
  // The data for DGConstLabVolumePtr& layerEndplates[layer][2] |
  //------------------------------------------------------------+
  m_layerEndplates[0][  kEastPlate ]  =  iEastWeddingCake1;
  m_layerEndplates[1][  kEastPlate ]  =  iEastWeddingCake1;
  m_layerEndplates[2][  kEastPlate ]  =  iEastWeddingCake2;
  m_layerEndplates[3][  kEastPlate ]  =  iEastWeddingCake2;
  m_layerEndplates[4][  kEastPlate ]  =  iEastWeddingCake3;
  m_layerEndplates[5][  kEastPlate ]  =  iEastWeddingCake3;
  m_layerEndplates[6][  kEastPlate ]  =  iEastWeddingCake4;
  m_layerEndplates[7][  kEastPlate ]  =  iEastWeddingCake4;
  m_layerEndplates[8][  kEastPlate ]  =  iEastWeddingCake5;
  m_layerEndplates[9][  kEastPlate ]  =  iEastWeddingCake5;
  m_layerEndplates[10][ kEastPlate ]  =  iEastWeddingCake6;
  m_layerEndplates[11][ kEastPlate ]  =  iEastWeddingCake6;
  m_layerEndplates[12][ kEastPlate ]  =  iEastWeddingCake7;
  m_layerEndplates[13][ kEastPlate ]  =  iEastWeddingCake7;
  m_layerEndplates[14][ kEastPlate ]  =  iEastWeddingCake8;
  m_layerEndplates[15][ kEastPlate ]  =  iEastWeddingCake8;

  m_layerEndplates[0][  kWestPlate ]  =  iWestWeddingCake1;
  m_layerEndplates[1][  kWestPlate ]  =  iWestWeddingCake1;
  m_layerEndplates[2][  kWestPlate ]  =  iWestWeddingCake2;
  m_layerEndplates[3][  kWestPlate ]  =  iWestWeddingCake2;
  m_layerEndplates[4][  kWestPlate ]  =  iWestWeddingCake3;
  m_layerEndplates[5][  kWestPlate ]  =  iWestWeddingCake3;
  m_layerEndplates[6][  kWestPlate ]  =  iWestWeddingCake4;
  m_layerEndplates[7][  kWestPlate ]  =  iWestWeddingCake4;
  m_layerEndplates[8][  kWestPlate ]  =  iWestWeddingCake5;
  m_layerEndplates[9][  kWestPlate ]  =  iWestWeddingCake5;
  m_layerEndplates[10][ kWestPlate ]  =  iWestWeddingCake6;
  m_layerEndplates[11][ kWestPlate ]  =  iWestWeddingCake6;
  m_layerEndplates[12][ kWestPlate ]  =  iWestWeddingCake7;
  m_layerEndplates[13][ kWestPlate ]  =  iWestWeddingCake7;
  m_layerEndplates[14][ kWestPlate ]  =  iWestWeddingCake8;
  m_layerEndplates[15][ kWestPlate ]  =  iWestWeddingCake8;

  for( ktLayer= kNumberOfAxialLayers; ktLayer < kNumberOfLayers; ++ktLayer )
    {  
      m_layerEndplates[ktLayer][ kEastPlate ]  =  iEastConicalEndPlate;
      m_layerEndplates[ktLayer][ kWestPlate ]  =  iWestConicalEndPlate;
    } 
 
  /* +-------------------------------------------------------+
     | get (x,y,z) nominal for endcaps coordinates systems   |
     +-------------------------------------------------------+
     */
  //   ofstream outf("ADRSenseWireStoreZlist.dump");  
 DGConstLabVolumePtr volumeE ;
 DGConstLabVolumePtr volumeW ;
 double zEndplateOriginNominalWest;
 DABoolean isNominal = true;
 for ( ktLayer= 0; ktLayer < kNumberOfLayers; ++ktLayer )
   {
     volumeE  =m_layerEndplates[ktLayer][ kEastPlate ];
     volumeW  =m_layerEndplates[ktLayer][ kWestPlate ];
     zEndplateOriginNominalWest =(volumeW->localToLabNominal()*HepPoint3D(0.,0.,0.)).z() ;
     if(
	!((volumeW->localToLabNominal() * HepPoint3D( 0., 0., 0.))==
          volumeW->localToLabAligned()  * HepPoint3D( 0., 0., 0.))
	|
	!((volumeE->localToLabNominal() * HepPoint3D( 0., 0., 0.))==
          volumeE->localToLabAligned()  * HepPoint3D( 0., 0., 0.))
        |
	!((volumeW->localToLabNominal() * HepVector3D( 1., 1., 1.))==
          volumeW->localToLabAligned()  * HepVector3D( 1., 1., 1.))
	|
	!((volumeE->localToLabNominal() * HepVector3D( 1., 1., 1.))==
          volumeE->localToLabAligned()  * HepVector3D( 1., 1., 1.))
	)
       {
         isNominal = false;
	 //	   outf<<" Layer "<<ktLayer+1<<" DRAligned NE Nominal"<<endl;
       }
     else
       {
	 //	   outf<<" Layer "<<ktLayer+1<<" DRAligned EQ Nominal"<<endl;
       }
     /*     outf<<" Layer "<< ktLayer+1<<" origin (0,0,0) "<<endl 
	    <<" West plate Aligned "
	    << volumeW->localToLabAligned() * HepPoint3D( 0., 0., 0.) 
	    <<" Nominal "
	    << volumeW->localToLabNominal() * HepPoint3D(0.,  0., 0.) << endl 
	    <<" East plate Aligned "
	    << volumeE->localToLabAligned() * HepPoint3D(0., 0., 0.) 
	    <<" Nominal  "
	    << volumeE->localToLabNominal()  * HepPoint3D(0., 0., 0.) << endl;
	    outf<<" Layer "<< ktLayer+1<<" Local Vector (1,1,1) "<<endl
	    <<" West plate Aligned " 
	    << volumeW->localToLabAligned() * HepVector3D( 1., 1., 1.)  
	    <<" Nominal "
	    << volumeW->localToLabNominal() * HepVector3D(1.,  1., 1.) << endl 
	    <<" East plate Aligned "
	    << volumeE->localToLabAligned()  * HepVector3D(1., 1., 1.)  
	    <<" Nominal "
	    << volumeE->localToLabNominal()  * HepVector3D(1., 1., 1.) << endl; */
	 
     layerData[ktLayer][kzOffsetWest] = layerDataDPP[ktLayer][kZWestInMMDPP]*.001
       - zEndplateOriginNominalWest;
     layerData[ktLayer][kzOffsetEast] = layerData[ktLayer][kzOffsetWest] ;
   }
 if(!isNominal)
   {
     report( VERIFY, kFacilityString )
       <<"+---------------------------------------------+"<<endl
       <<"|                                             |"<<endl
       <<"| ADRGeom DRAligned is not the same as Nominal|"<<endl
       <<"|           This is ok                        |"<<endl
       <<"+---------------------------------------------+"<<endl;

     /*    outf<<"+--------------------------------------------+"<<endl
	   <<"|ADRGeom DRAligned is not the same as Nominal|"<<endl
	   <<"+--------------------------------------------+"<<endl; */
   } 
 else
   {
     report( VERIFY, kFacilityString )
       <<"+---------------------------------------------+"<<endl
       <<"|                   A L E R T                 |"<<endl
       <<"|    ADRGeom DRAligned is the same as Nominal |"<<endl
       <<"|                     or                      |"<<endl
       <<"|    ADRGeom was extracted from Base Geometry |"<<endl
       <<"|                                             |"<<endl
       <<"+---------------------------------------------+"<<endl;
       /*    outf
	     <<"+-------------------------------------------+"<<endl
	     <<"|ADRGeom DRAligned is the same as Nominal   | "  <<endl
	     <<"|or ADRGeom was extracted from Base Geometry|"<<endl
	     <<"+-------------------------------------------+"<<endl; */
   }

  //-----------------------------------------------------+
  // the data for z-offsets, Axial layers (Wedding cake) |
  //         *** replaced with data from DPP  ****       |
  //-----------------------------------------------------+

  //----------------------------------------------------------------+
  // Stereo Layers                                                  |
  // the data for z-offsets are given by polycone z-offsets indices |
  //         *** replaced with data from DPP  ****                  |
  //----------------------------------------------------------------+
//         +--------------------------+
//         | Finally! Setup each layer|
//         +--------------------------+

     //   m_layers = new AEWireLayerCylindrical[kNumberOfLayers];
   m_layers = new ADRWireLayerRealistic[kNumberOfLayers];
   //---------------------------------+
   // sagx, sagy: quadratic fits      |
   // to the sensewire   x(z), y(z)   |
   // sagx=0 sagy(layer)=from DPP     |      
   // see Revision 3/10/99            |
   //---------------------------------+
   double zOffEast ;
   double zOffWest ;
   double z0=0.0;
   HepDouble  radius0;
   double phi0, phi0N;
   double zmin;
   double zmax;
   double radExtentMinus;
   double radExtentPlus;
   AEWireInfo::WireDirZ wDZ;
   double phiE,phiW;
   double zE,zW;
   double tanStereoAngle;
   double phi_dif;
   int    firstCellInLyr;
   int iwire;
   for( unsigned int itLayer(0); itLayer < kNumberOfLayers; ++itLayer ) { 

     //      AEWireLayerCylindrical* pLayer = m_layers+itLayer;
      ADRWireLayerRealistic* pLayer = m_layers+itLayer;
      firstCellInLyr = cellID(itLayer+ kFirstLayer, 0);
      ADRWireSurveyConstants** pCWire   
      	= pWireSurveyConstants + firstCellInLyr - kFirstCellInDR;
      zOffEast =  layerData[itLayer][kzOffsetEast];
      zOffWest =  layerData[itLayer][kzOffsetWest];
  // 9-11-00 OBSOLETE: 
      /*      zOffEast =  layerData[itLayer][kzOffsetEast]
	-  endPlateBowInch[itLayer]*0.0254
	-  z_wire_end_offset_mm[itLayer]*0.001;
      zOffWest =  layerData[itLayer][kzOffsetWest]
	-  endPlateBowInch[itLayer]*0.0254
	-  z_wire_end_offset_mm[itLayer]*0.001;
	*/
		   	   
      radExtentMinus = (layerData[itLayer][kRadiusInCM]
			-FieldRadiusCM[itLayer][rad_small])/100.;
      radExtentPlus = (FieldRadiusCM[itLayer][rad_big]
		       -layerData[itLayer][kRadiusInCM])/100.;

      pLayer->setLayer(int( layerData[itLayer][kNumberOfWires] ),
		       m_layerEndplates[itLayer][kEastPlate],
		       m_layerEndplates[itLayer][kWestPlate],       
		       wire_Sag[itLayer][sagx],
		       wire_Sag[itLayer][sagy],
		       int( layerData[itLayer][ReadoutEnd]),
		       (layerData[itLayer][kRadiusInCM]/100.) ,
  // 9-11-00 OBSOLETE:  
		       //M_PI/180.0 * layerData[itLayer][kAngleInDegreesE],
		       //M_PI/180.0 * layerData[itLayer][kAngleInDegreesW],
		       layerData[itLayer][kAngleInRadiansE],
		       layerData[itLayer][kAngleInRadiansW], 
		       zOffEast,
		       zOffWest,
		       radExtentMinus,
		       radExtentPlus,
		       (itLayer<kNumberOfAxialLayers),
		       (itLayer>=kNumberOfAxialLayers),
		       m_isULayer[itLayer],
		       m_isVLayer[itLayer],
		       pCWire);
// print out wires' positions:
      /*           outf<< endl <<
  "Ly/Wr     East (x,y,z-)              West (x,y,z+)                 z+(inch) phiE-phiW" 
 << endl; 
 for(  iwire=0;  iwire < 4 ; ++iwire) 		
 { 
 const AEWireInfo& wireo = wire(itLayer+kFirstLayer,iwire);        
 phiE =atan2(wireo.point1().y(),wireo.point1().x());        	  
 phiW =atan2(wireo.point2().y(),wireo.point2().x());
 phi_dif  =phiE - phiW;
 outf << setw(2) << (itLayer+1) << setw(3)<< iwire <<" " 
 <<  wireo.point1() <<"  "<< wireo.point2() <<" "
 << setw(7)<< wireo.point2().z()/0.0254<< " "<<setw(8)<< phi_dif<<endl; 
 }
 */
   } 
       
}

// ADRSenseWireStore::ADRSenseWireStore(const ADRSenseWireStore& ) 
// {
// }


ADRSenseWireStore::~ADRSenseWireStore()
{
   delete [] m_layers;   
   for(  int ix=0; ix < (kNumberOfLayers-1) ; ix++)  
       {	
	 delete m_PLayerConstants [ix]; 
	 delete m_PLayersEndsConstants [ix]; 
       } 
   delete [] m_PLayerConstants;
   delete [] m_PLayersEndsConstants;

   //NOTE: ADRWireLayerRealistics own the elements that this array holds
   //      pointers to
   delete [] m_PWireSurveyConstants;
}

//
// assignment operators
//
// const ADRSenseWireStore& ADRSenseWireStore::operator=
//                                 ( const ADRSenseWireStore& )
// {
// }

//
// member functions
// 
void ADRSenseWireStore:: updateSenseWireStore
         (const DGConstLabVolumePtr& iEastWeddingCake1,
	  const DGConstLabVolumePtr& iEastWeddingCake2,
	  const DGConstLabVolumePtr& iEastWeddingCake3,
	  const DGConstLabVolumePtr& iEastWeddingCake4,
	  const DGConstLabVolumePtr& iEastWeddingCake5,
	  const DGConstLabVolumePtr& iEastWeddingCake6,
	  const DGConstLabVolumePtr& iEastWeddingCake7,
	  const DGConstLabVolumePtr& iEastWeddingCake8,
	  const DGConstLabVolumePtr& iEastConicalEndPlate,
	  const DGConstLabVolumePtr& iWestWeddingCake1,
	  const DGConstLabVolumePtr& iWestWeddingCake2,
	  const DGConstLabVolumePtr& iWestWeddingCake3,
	  const DGConstLabVolumePtr& iWestWeddingCake4,
	  const DGConstLabVolumePtr& iWestWeddingCake5,
	  const DGConstLabVolumePtr& iWestWeddingCake6,
	  const DGConstLabVolumePtr& iWestWeddingCake7,
	  const DGConstLabVolumePtr& iWestWeddingCake8,
	  const DGConstLabVolumePtr& iWestConicalEndPlate)
{
    setEastWeddingCakeEndPlate (iEastWeddingCake1,
				iEastWeddingCake2,
				iEastWeddingCake3,
				iEastWeddingCake4,
				iEastWeddingCake5,
				iEastWeddingCake6,
				iEastWeddingCake7,
				iEastWeddingCake8,
				iEastConicalEndPlate);
    setWestWeddingCakeEndPlate (iWestWeddingCake1,
				iWestWeddingCake2,
				iWestWeddingCake3,
				iWestWeddingCake4,
				iWestWeddingCake5,
				iWestWeddingCake6,
				iWestWeddingCake7,
				iWestWeddingCake8,
				iWestConicalEndPlate);
   for( unsigned int itLayer(0); itLayer < kNumberOfLayers; ++itLayer ) {
     //     AEWireLayerCylindrical& layer = layerNC(itLayer);
     //     layer.updateLayer(m_layerEndplates[itLayer][kEastPlate],
     //		       m_layerEndplates[itLayer][kWestPlate]);
     ADRWireLayerRealistic& layer = layerNC(itLayer+1);
     layer.updateLayer(m_layerEndplates[itLayer][kEastPlate],
		       m_layerEndplates[itLayer][kWestPlate]);
   }
}
void ADRSenseWireStore::setEastWeddingCakeEndPlate ( 
	        const DGConstLabVolumePtr& iEastWeddingCake1,
                const DGConstLabVolumePtr& iEastWeddingCake2,
	        const DGConstLabVolumePtr& iEastWeddingCake3,
                const DGConstLabVolumePtr& iEastWeddingCake4,
	        const DGConstLabVolumePtr& iEastWeddingCake5,
                const DGConstLabVolumePtr& iEastWeddingCake6,
	        const DGConstLabVolumePtr& iEastWeddingCake7,
                const DGConstLabVolumePtr& iEastWeddingCake8,
		const DGConstLabVolumePtr& iEastConicalEndPlate)
{ 
  m_layerEndplates[0][ kEastPlate ] =   iEastWeddingCake1;
  m_layerEndplates[1][ kEastPlate ] =   iEastWeddingCake1;
  m_layerEndplates[2][ kEastPlate ] =   iEastWeddingCake2;
  m_layerEndplates[3][ kEastPlate ] =   iEastWeddingCake2;
  m_layerEndplates[4][ kEastPlate ]  =  iEastWeddingCake3;
  m_layerEndplates[5][ kEastPlate ]  =  iEastWeddingCake3;
  m_layerEndplates[6][ kEastPlate ]  =  iEastWeddingCake4;
  m_layerEndplates[7][ kEastPlate ]  =  iEastWeddingCake4;
  m_layerEndplates[8][ kEastPlate ]  =  iEastWeddingCake5;
  m_layerEndplates[9][ kEastPlate ]  =  iEastWeddingCake5;
  m_layerEndplates[10][ kEastPlate ] =  iEastWeddingCake6;
  m_layerEndplates[11][ kEastPlate ] =  iEastWeddingCake6;
  m_layerEndplates[12][ kEastPlate ] =  iEastWeddingCake7;
  m_layerEndplates[13][ kEastPlate ] =  iEastWeddingCake7;
  m_layerEndplates[14][ kEastPlate ] =  iEastWeddingCake8;
  m_layerEndplates[15][ kEastPlate ] =  iEastWeddingCake8;
  unsigned int itLayer;
  for( itLayer= kNumberOfAxialLayers; itLayer< kNumberOfLayers; ++itLayer )
    { 
      m_layerEndplates[itLayer][ kEastPlate ]  =  iEastConicalEndPlate; 
    }   

  for( itLayer=0 ; itLayer < kNumberOfLayers ; ++ itLayer )
    { 
    m_layers[itLayer].setVolume1( m_layerEndplates[itLayer][kEastPlate] );
    }
}

void ADRSenseWireStore::setWestWeddingCakeEndPlate (
		const DGConstLabVolumePtr& iWestWeddingCake1,
                const DGConstLabVolumePtr& iWestWeddingCake2,
		const DGConstLabVolumePtr& iWestWeddingCake3,
                const DGConstLabVolumePtr& iWestWeddingCake4,
		const DGConstLabVolumePtr& iWestWeddingCake5,
                const DGConstLabVolumePtr& iWestWeddingCake6,
		const DGConstLabVolumePtr& iWestWeddingCake7,
                const DGConstLabVolumePtr& iWestWeddingCake8,
		const DGConstLabVolumePtr& iWestConicalEndPlate)
{
  m_layerEndplates[0][ kWestPlate ]  =  iWestWeddingCake1;
  m_layerEndplates[1][ kWestPlate ]  =  iWestWeddingCake1;
  m_layerEndplates[2][ kWestPlate ]  =  iWestWeddingCake2;
  m_layerEndplates[3][ kWestPlate ]  =  iWestWeddingCake2; 
  m_layerEndplates[4][ kWestPlate ]  =  iWestWeddingCake3;
  m_layerEndplates[5][ kWestPlate ]  =  iWestWeddingCake3;
  m_layerEndplates[6][ kWestPlate ]  =  iWestWeddingCake4;
  m_layerEndplates[7][ kWestPlate ]  =  iWestWeddingCake4;
  m_layerEndplates[8][ kWestPlate ]  =  iWestWeddingCake5;
  m_layerEndplates[9][ kWestPlate ]  =  iWestWeddingCake5;
  m_layerEndplates[10][ kWestPlate ] =  iWestWeddingCake6;
  m_layerEndplates[11][ kWestPlate ] =  iWestWeddingCake6;
  m_layerEndplates[12][ kWestPlate ] =  iWestWeddingCake7;
  m_layerEndplates[13][ kWestPlate ] =  iWestWeddingCake7;
  m_layerEndplates[14][ kWestPlate ] =  iWestWeddingCake8;
  m_layerEndplates[15][ kWestPlate ] =  iWestWeddingCake8;
  unsigned int itLayer;
  for( itLayer= kNumberOfAxialLayers;itLayer < kNumberOfLayers; ++itLayer )
    {
      m_layerEndplates[itLayer][ kWestPlate ]  =  iWestConicalEndPlate; 
    } 

  for( itLayer=0; itLayer < kNumberOfLayers ; ++ itLayer )
    {
      m_layers[itLayer].setVolume2( m_layerEndplates[itLayer][kWestPlate] );
    }
}

//AEWireLayerCylindrical&
ADRWireLayerRealistic&
ADRSenseWireStore::layerNC( unsigned int iLayerID ) const
{
  assert( ( kFirstLayer <= iLayerID ) && (kNumberOfLayers >= iLayerID ) );
  return m_layers[iLayerID-kFirstLayer];
}

//
// const member functions
//
const AEWireLayerCylindrical&
ADRSenseWireStore::layer( unsigned int iLayerID ) const
{
  assert( ( kFirstLayer <= iLayerID ) && (kNumberOfLayers >= iLayerID ) );
  return m_layers[iLayerID-kFirstLayer];
}

const AEWireInfo
ADRSenseWireStore::wireMk( const ADRWireAddress& iWireAddress ) const
{
  return layer( iWireAddress.layerID() ).wireMk( iWireAddress.wireID() );
}

const AEWireInfo&
ADRSenseWireStore::wire( const ADRWireAddress& iWireAddress ) const
{
  return layer( iWireAddress.layerID() ).wire( iWireAddress.wireID() );
}

const AEWireInfo
ADRSenseWireStore::wireMk( const unsigned int iCellID ) const
{
  ADRSenseWireStore::LayerWire lw=layerWire(iCellID);
  return layer( lw.first ).wireMk( lw.second );
}

const AEWireInfo&
ADRSenseWireStore::wire( const unsigned int iCellID ) const
{ 
  ADRSenseWireStore::LayerWire lw=layerWire(iCellID);
  return layer( lw.first ).wire( lw.second );
}

const unsigned int*  
ADRSenseWireStore::aLayers() const
{ return  m_aLayer;}

const unsigned int*
ADRSenseWireStore::uLayers() const
{ return m_uLayer;}
 
const unsigned int*
ADRSenseWireStore::vLayers() const 
{ return m_vLayer;}

ADRSenseWireStore::LayerWire 
ADRSenseWireStore::layerWire(const UInt32 CellID) const
{ 
  if( (CellID <= 0) | (CellID > kNumberOfWiresTotal ))
    {
      report( DEBUG, kFacilityString )
	<<"Illegal CellID="<<CellID<<endl;
    }
  assert( CellID > 0 && CellID <= kNumberOfWiresTotal );
  unsigned short layerCellID = m_cellToLayer[CellID-1];
  //  return (layer,wire_in_layer=(1, Number of wires in lyr )):
  //  unsigned short wireCellID  = CellID - m_firstCellInLayer[layerCellID-1] + 1;
  //  return (layer,wire_in_layer=(0, Number of wires in lyr-1 )):
  unsigned short wireCellID  = CellID - m_firstCellInLayer[layerCellID-1] ;
  LayerWire wl(layerCellID,wireCellID);
  return wl;  
}

UInt32 
ADRSenseWireStore::cellID(const unsigned short layer,
		 const unsigned short wire) const
{ 
  //=============debug
  //     report( INFO, kFacilityString ) 
  //     <<endl<<"cellID for layer, wire"
  //    <<setw(3)<<layer<<setw(3)<<wire<<endl
  //    <<"first/lastcellid in this lyr"
  //     <<setw(5)<<m_firstCellInLayer[layer-1]
  //     <<setw(5)<<m_lastCellInLayer[layer-1]<<endl
  //    <<"first+wire-1"<<setw(5)
  //    <<(m_firstCellInLayer[layer-1] + wire - 1)<<endl; 
  //==========end debug

  assert (layer > 0 && layer <=kNumberOfLayers );

  // wire> 0    
  //  assert (wire >  0) ;
  //  UInt32 cell =m_firstCellInLayer[layer-1] + wire - 1 ;
  assert ((wire+1) >  0) ;
  UInt32 cell =m_firstCellInLayer[layer-1] + wire ;
  assert ( cell <= m_lastCellInLayer[layer-1] );
  return cell;
}

unsigned int
ADRSenseWireStore::layerNumber
(DABoolean& success, const HepPoint3D& position) const
{ 
  //Default:  success = false, irlayer = 0
  success  = false;
  unsigned int irlayer= 0;
  double zP   = position.z();
  double rP   = position.perp();
  double rLAYERMini;
  double rLAYERMaxi=.80 ;
  double rz;
  double zLAYERMin, zLAYERMax; 
  DABoolean success1;  
  for (unsigned int itLayer=1; 
       ((irlayer==0) && (itLayer <= kNumberOfLayers)) ; ++itLayer)
    { 
      const AEWireLayerCylindrical& layer_it = layer(itLayer);
      // Approximate radius of sense wires layer at zP, checks range 
      rz = layer_it.radiusAtz(success1, zP);
      if(success1)
	{
	  // Radial extent of the layer at zP, given by field wires
	  rLAYERMini = rz - layer_it.radialExtentMinus();
	  rLAYERMaxi = rz + layer_it.radialExtentPlus();
	  if( (rP > rLAYERMini) && (rP <= rLAYERMaxi) )
	    { 
	      irlayer = itLayer;
	      success = true;
	    }  
	}
    }  
  if(!success)
    {
      report( VERIFY, kFacilityString )
	<<"NO DR3 layer for point at r="<< rP<<" z="<<zP<<endl;
    }
  return irlayer;
}
const AEWireInfo& 
ADRSenseWireStore::wireObjectClosest
( DABoolean& success, const HepPoint3D& iPoint, unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  DABoolean success1;
  const AEWireInfo& itwire =itlayer.wireObjectClosest(success1, iPoint);
  success = success1;
  if(!success)
    {
      report( VERIFY, kFacilityString )
	<<"NO DR3 closest wire object for point"<<endl;
    }
  return itwire;
}

unsigned int ADRSenseWireStore::numberOfWiresLyr(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.numberOfWires();
}
double ADRSenseWireStore::radius(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.radius();
}
double ADRSenseWireStore::phi0(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.phi0();
}
double ADRSenseWireStore::radiusNominal(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.radiusNominal();
}
double ADRSenseWireStore::phi0Nominal(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.phi0Nominal();
}

double ADRSenseWireStore::cellPhiWidth(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return  itlayer.cellPhiWidth();
}

double ADRSenseWireStore::tanStereoAngle(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.tanStereoAngle();
}

double ADRSenseWireStore::zMax(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return  itlayer.zMax(); 
}

double ADRSenseWireStore::zMin(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.zMin();
  
}

DABoolean ADRSenseWireStore::isAxial(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.isAxial();
}

DABoolean ADRSenseWireStore::isStereo(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.isStereo();
}

DABoolean ADRSenseWireStore::isStereoV(unsigned int ilayer) const 
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return itlayer.isStereoV();  
}

DABoolean ADRSenseWireStore::isStereoU(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return  itlayer.isStereoU(); 
}  

double ADRSenseWireStore::radiusAtz(
     DABoolean& success, unsigned int ilayer, double z) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  DABoolean success1;
  double radiusAtz = itlayer.radiusAtz(success1, z);
  success = success1;
  if(!success) 
    { 
      //          report( INFO, kFacilityString )
          report( VERIFY, kFacilityString )
	    <<"fails to find radius at z="<< z<<" layer "<< ilayer
	    <<endl;
    }
  return radiusAtz;
} 
  
const AEWireInfo& 
ADRSenseWireStore::wire(unsigned int ilayer, unsigned int iwireID) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  const  AEWireInfo& wireL=itlayer.wire(iwireID);
  return wireL;
}



const ADRWireSurveyConstants&
ADRSenseWireStore::wireSurveyConstants( unsigned int iCellID ) const
{
  assert( ( kFirstCellInDR <= iCellID ) && (kNumberOfWiresTotal >= iCellID ) );
  return *m_PWireSurveyConstants[iCellID-kFirstCellInDR];
}

const ADRLayerConstants&
ADRSenseWireStore::layerConstants( unsigned int iLayerID ) const
{
  assert( ( kFirstLayer <= iLayerID ) && (kNumberOfLayers >= iLayerID ) );
  return *m_PLayerConstants[iLayerID-kFirstLayer];
}

const ADRGeomLayersEndsConstants&
ADRSenseWireStore::layersEndsConstants( unsigned int iLayerID ) const
{
  assert( ( kFirstLayer <= iLayerID ) && (kNumberOfLayers >= iLayerID ) );
  return *m_PLayersEndsConstants[iLayerID-kFirstLayer];
}

HepPoint3D 
ADRSenseWireStore::pointClosestFieldWireInFieldPlane
( DABoolean& success, const HepPoint3D& iPoint, unsigned int ilayer) const
{
  /* see function (below)
     pointClosestFieldWireInFieldPlane
     ( DABoolean& success, HepVector3D& tanVector,
     const HepPoint3D& iPoint, unsigned int ilayer)
     */
  DABoolean success1 ;
  HepVector3D tanVector;
  HepPoint3D OnField=pointClosestFieldWireInFieldPlane
                     ( success1, tanVector, iPoint, ilayer) ;
  success = success1;
  return OnField;
}

HepPoint3D 
ADRSenseWireStore::pointClosestFieldWireInFieldPlane
( DABoolean& success, HepVector3D& tanVector,
  const HepPoint3D& iPoint, unsigned int ilayer) const
{
     
  //----------------------------------------------------------------------------------+
  //  Input:   point iPoint in the field wire plane in layer ilayer.
  //           The convention is that the field wire plane belongs with the 
  //           sense wire layer above it, except for layer 16 where there 
  //           are two field wire planes, one below, one above.   
  //           iPoint must be within the field wire radius range (with some tolerance)
  //           ilayer = 1-47 
  //  Output:  the point on the closest field wire in this wire "plane", at z=iPoint.z() 
  //           the tangent vector tanVector
  //           success = false if radius of iPoint out of range, or search fails  
  //
  //           If success=false, returns point at origin (0,0.0), 
  //                                        and tanVector(-1.,-1.,-1.)
  //              
  //
 
  HepPoint3D OnField=HepPoint3D(0.,0.,0.);
  tanVector= HepVector3D(-1.,-1.,-1.) ;
  success = false;

  const float epsilon = 0.000200;
  double rP    = iPoint.perp();
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  double rmin = itlayer.radius() - itlayer.radialExtentMinus()-epsilon;
  double rmax = itlayer.radiusEndCap()- itlayer.radialExtentMinus() + epsilon;
  double r2min, r2max;
  if(ilayer==16)
    { 
      r2min = itlayer.radius() + itlayer.radialExtentPlus()-epsilon;
      r2max = itlayer.radiusEndCap() + itlayer.radialExtentPlus() + epsilon;
    }

// Verify iPoint within the range
  DABoolean radiusInRange = false ;
  DABoolean radiusInRange2 =  false ; 
  
  if ( (rP >= rmin) && (rP <= rmax) ) 
    {  radiusInRange =  true; }
  else 
    {
      if(ilayer == 16)
	{
	  // check upper layer field wires
	  if ( (rP >= r2min) && (rP <= r2max) )	
	    {
	      radiusInRange2 = true;
	      radiusInRange =  true;
	    }
	}
    }
  
  if (!radiusInRange) 
    {
	report( VERIFY, kFacilityString )
	  <<"Lyr " << ilayer<<" Point w/radius= "<< rP <<" outside Field Wire range" <<endl;
	report( DEBUG, kFacilityString )
	      <<"Point" << iPoint<< " at radius = "<< rP <<" is outside range" <<endl
	      <<"Field wire radius range for layer "<<ilayer<<endl
	      <<" is rmin to rmax :"<< rmin<<" to "<<rmax <<endl; 
      if(ilayer == 16)
	{ 
	   report( DEBUG, kFacilityString )
	     <<" or rmin to rmax :"<< r2min<<" to "<<r2max<<endl;
	}
    }
  else  
    {
      DABoolean success1;
      const AEWireInfo& itwire =itlayer.wireObjectClosest(success1, iPoint);
      success = success1;
      double phi,x,y,z;
      double rF, delphi;
      double xF0,yF0, xFp,yFp, xFm,yFm;
      double d0, dp, dm;      
      AEWireInfo::WireDirZ wDZ;
      HepPoint3D OnSense;
      if(!success)
	{
	  report( VERIFY, kFacilityString )
	    <<"NO DR3 closest wire object for point"<<endl;
	}
      else
	{
	  x = iPoint.x()/rP;    /* tested point cosphi, sinphi*/
	  y = iPoint.y()/rP;
	  z=  iPoint.z();       /* tested point z*/
	  wDZ  = itwire.zPoint(iPoint.z());
	  OnSense  = wDZ.first;
	  tanVector= wDZ.second;
	  phi = atan2(OnSense.y(),OnSense.x());
	  delphi = itlayer.cellPhiWidth()/2.;
          if (radiusInRange2)
	    { rF  = OnSense.perp() + itlayer.radialExtentPlus();  }
	  else
	    { rF  = OnSense.perp() - itlayer.radialExtentMinus(); }

	  xF0 = cos(phi);         /* closest sense wire cosphi, sinphi = central field wire*/
	  yF0 = sin(phi);      
	  xFp = cos(phi+delphi);  /* next field wire cosphi, sinphi */  
	  yFp = sin(phi+delphi); 
	  xFm = cos(phi-delphi);  /* previous field wire cosphi, sinphi */
	  yFm = sin(phi-delphi);
	  //find the closest field wire
	  d0 = sqrt((xF0-x)*(xF0-x) + (yF0-y)*(yF0-y));
	  dm = sqrt((xFm-x)*(xFm-x) + (yFm-y)*(yFm-y));
	  dp = sqrt((xFp-x)*(xFp-x) + (yFp-y)*(yFp-y));
	  
/*         cout<<endl<<" Field Plane search, lyr "<<ilayer<<endl		     
<<" input point r,x,y "<<rP<<",("<<rP*x  <<","<<rP*y  <<")"<<endl		     
<<" dm                "<<rF<<",("<<rF*xFm<<","<<rF*yFm<<") phi.dm= "<< phi-delphi<<dm<<endl     
<<" d0                "<<rF<<",("<<rF*xF0<<","<<rF*yF0<<") phi,d0= "<< phi       <<d0<<endl     
<<" dp                "<<rF<<",("<<rF*xFp<<","<<rF*yFp<<") phi,dp= "<< phi+delphi<<dp<<endl
		     <<endl; */

          if(ilayer==1)
	    {
	      // there is no field wire under the sense wire in layer 1
	      if(dp < dm)
		{ OnField = HepPoint3D(rF*xFp, rF*yFp, z);	}
	      else
		{ OnField = HepPoint3D(rF*xFm,rF*yFm,z);        }
	    }
	  else
	    {
	      if(d0 < dm) 
		{
		  if(dp < d0) 
		    {
		      OnField = HepPoint3D(rF*xFp, rF*yFp, z);
		    }
		  else
		    OnField = HepPoint3D(rF*xF0, rF*yF0, z);
		}
	      else
		{
		  OnField = HepPoint3D(rF*xFm,rF*yFm,z); 
		}
	    }
	}
    }

  /*  cout<<"tanVector "<<tanVector<<" (x,y,z)= ("
      <<setw(9) <<setprecision(6) <<tanVector.x()<<","
      <<setw(9) <<setprecision(6) <<tanVector.y()<<","
      <<setw(9) <<setprecision(6) <<tanVector.z()<<")"<<endl;*/
  return OnField;
}      


HepPoint3D 
ADRSenseWireStore::pointClosestFieldWireInSensePlane
( DABoolean& success, const HepPoint3D& iPoint, unsigned int ilayer) const
{
 /* see function (below)
     pointClosestFieldWireInSensePlane
     ( DABoolean& success, HepVector3D& tanVector,
     const HepPoint3D& iPoint, unsigned int ilayer)
     */
  DABoolean success1 ;
  HepVector3D tanVector;
  HepPoint3D OnField=pointClosestFieldWireInSensePlane
                     ( success1, tanVector, iPoint, ilayer) ;
  success = success1;
  return OnField;
}

HepPoint3D 
ADRSenseWireStore::pointClosestFieldWireInSensePlane
( DABoolean& success, HepVector3D& tanVector,
  const HepPoint3D& iPoint, unsigned int ilayer) const
{
  //  Input: iPoint at the sense/field wires "plane" in layer ilayer  
  //         iPoint must be within the layer
  //          (additional tolerance    = 200 microns)  
  //         ilayer = 1 - 47 

  //  Output:  success = false if iPoint not in the layer, or if distance algorithm fails
  //           The point on the closest field wire at z=iPoint.z() in this wires plane 
  //           the tangent vector tavVector 
  //           if success=false, returns point at origin (0,0.0) 
  //                                        and tanVector(-1.,-1.,-1.)


  HepPoint3D OnField=HepPoint3D(0.,0.,0.);
  tanVector= HepVector3D(-1.,-1.,-1.);
  success = false;

  double rP    = iPoint.perp();
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  //  +------------------------------------------------+
  //  Obsolete: Allow search from the entire layer:
  //  const float epsilon = 0.000200;
  //  double rmin = itlayer.radius() - epsilon;
  //  double rmax = itlayer.radiusEndCap()+ epsilon;
  // First verify iPoint within the range 
  //  if ( ! ((rP >= rmin) && (rP <= rmax)) )
  //    {
  //      report( INFO, kFacilityString )
  //	  <<"Lyr " << ilayer<<" Point w/radius= "<< rP <<" outside Sense/Field Wire range" <<endl;
  //      report( DEBUG, kFacilityString )
  //      <<"Point" << iPoint<< " at radius = "<< rP << " is outside range"<<endl
  //      <<"Field/Sense wire radius range for Layer "<<ilayer<<endl
  //      <<" is rmin to rmax :"<< rmin<<" to "<<rmax <<endl;
  //    }
  //  else
  //      {
  //  +------------------------------------------------+
      DABoolean success1;
      const AEWireInfo& itwire =itlayer.wireObjectClosest(success1, iPoint);
      success = success1;
      double phi,x,y,z;
      double rF, delphi;
      double xFp,yFp, xFm,yFm;
      double dp, dm;
      AEWireInfo::WireDirZ wDZ;
      HepPoint3D OnSense;
      if(!success)
	{
	  report( VERIFY, kFacilityString )
	    <<"NO DR3 closest wire object for point"<<endl;
	}
      else
	{
	  x = iPoint.x()/rP;    /* tested point cosphi, sinphi*/
	  y = iPoint.y()/rP;
	  z=  iPoint.z();       /* tested point z*/
	  wDZ  = itwire.zPoint(iPoint.z());
	  OnSense  = wDZ.first;
	  tanVector= wDZ.second;
	  phi = atan2(y,x);
	  delphi = itlayer.cellPhiWidth()/2.;
          rF  = OnSense.perp(); 
	  xFp = cos(phi+delphi); /* next field wire cosphi, sinphi */
	  yFp = sin(phi+delphi); 
	  xFm = cos(phi-delphi); /* previous field wire cosphi, sinphi */
	  yFm = sin(phi-delphi);
	  //find the closer field wire
	  dm = sqrt((xFm-x)*(xFm-x)+(yFm-y)*(yFm-y));
	  dp = sqrt((xFp-x)*(xFp-x)+(yFp-y)*(yFp-y));
	  
/*          cout<<endl<<" Sense Plane search, lyr "<<ilayer<<endl	      
<<" input point r,x,y "<<rP<<",("<<rP*x  <<","<<rP*y  <<")"<<endl	      
<<" dm                "<<rF<<",("<<rF*xFm<<","<<rF*yFm<<") phi.dm= "<< phi-delphi<<dm<<endl     
<<" dp                "<<rF<<",("<<rF*xFp<<","<<rF*yFp<<") phi,dp= "<< phi+delphi<<dp<<endl     
<<endl;  */

	  if (dp<dm) 
	    {
	      OnField = HepPoint3D(rF*xFp,rF*yFp,z);
	    }
	  else 
	    {
	      OnField = HepPoint3D(rF*xFm,rF*yFm,z); 	
	    }
	}
      //    }

  /*  cout<<"tanVector "<<tanVector<<" (x,y,z)= ("
      <<setw(9) <<setprecision(6) <<tanVector.x()<<","
      <<setw(9) <<setprecision(6) <<tanVector.y()<<","
      <<setw(9) <<setprecision(6) <<tanVector.z()<<")"<<endl; */

  return OnField;
}

double ADRSenseWireStore::fieldRadiusNominalUp(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return (itlayer.radiusNominal()+ itlayer.radialExtentPlus());
}

double ADRSenseWireStore::fieldRadiusNominalDown(unsigned int ilayer) const
{
  const AEWireLayerCylindrical& itlayer= layer(ilayer);
  return (itlayer.radiusNominal()- itlayer.radialExtentMinus());
}
  //
  // static member functions
  //









