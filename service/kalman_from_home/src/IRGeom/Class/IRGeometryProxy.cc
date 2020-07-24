// -*- C++ -*-
//
// Package:     IRGeom
// Module:      IRGeometryProxy
// 
// Description: Builds the Interaction region geometry detector store
//
// Implementation:
//     Interaction region components built in seperate function classes--
//     This class builds the lab volume, IRMotherVolume, retrieves the 
//     components from their given function classes and places them in the 
//     IRMotherVolume.
//
// Author:      Lawrence Gibbons
// Created:     Mon Jan  4 08:44:08 EST 1999
// $Id: IRGeometryProxy.cc,v 1.5 2002/05/31 17:19:46 bkh Exp $
//
// Revision history
//
// $Log: IRGeometryProxy.cc,v $
// Revision 1.5  2002/05/31 17:19:46  bkh
// Repair so does not overlap DR or CC endcap and also
// make it one sided only and replicate that side for negative z
//
// Revision 1.4  2000/02/02 15:25:35  lkg
// * Extract the materials explicitly from the basegeometry record.
// * Better debugging statements (only seen when CLEO_DEBUG is defined)
//
// Revision 1.3  1999/07/23 22:23:47  lkg
// Encase various for loops in a pair of {} to deal with screwy loop index
// scope under SunOs/CC
//
// Revision 1.2  1999/07/22 22:01:10  lkg
// Create a real lab volume (a la CcGeom) so that we end up with a copy number
//
// Revision 1.1.1.1  1999/07/22 14:38:42  lkg
// First release
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Experiment/units.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FAItem.h"           

#include "DRGeom/DRGeometryParams.h"

#include "IRGeom/IRGeometryProxy.h"
#include "IRGeom/IRGeomParameters.h"
#include "IRGeom/BeamPipeGeometry.h"
#include "IRGeom/BeamPipeGeometry23.h"
#include "IRGeom/MaskMainGeometry.h"
#include "IRGeom/MaskInnerGeometry.h"
#include "IRGeom/MaskHybridGeometry.h"
#include "IRGeom/REQMagnetGeometry.h"
#include "IRGeom/StainlessTubeGeometry.h"
#include "IRGeom/CuSupportGeometry.h"

#include "ToolBox/Triplet.h"

#include "DetectorGeometry/DGVolume.h"
#include "DetectorGeometry/DGVolumePtr.h"
#include "DetectorGeometry/DGMaterial.h"
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "DetectorGeometry/DGPlacedVolume.h"
#include "DetectorGeometry/DGPlacedVolumePtr.h"
#include "DetectorGeometry/DGZrRList.h"
#include "DetectorGeometry/DGVolumeBox.h"
#include "DetectorGeometry/DGVolumeTube.h"
#include "DetectorGeometry/DGVolumeCone.h"
#include "DetectorGeometry/DGVolumePolyCone.h"
#include "DetectorGeometry/DGVolumeParentPlaced.h"
#include "DetectorGeometry/DGMaterialStore.h"
#include "DetectorGeometry/DGDetectorStore.h"

#include "MaterialDelivery/material_identifiers.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//
typedef double IRreal;
typedef pair<IRreal,IRreal> IRpair;

static const char* const kFacilityString = "IRGeom.IRGeometryProxy" ;

static const DGConstMaterialPtr k_nullMaterialPtr (  new DGMaterial() ) ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: IRGeometryProxy.cc,v 1.5 2002/05/31 17:19:46 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
IRGeometryProxy::IRGeometryProxy()
   : m_IRGeometry( 0 )
{
}

// IRGeometryProxy::IRGeometryProxy( const IRGeometryProxy& iproxy )
// {
//   *this = iproxy;
// }

IRGeometryProxy::~IRGeometryProxy()
{
   delete m_IRGeometry ;
}

//
// assignment operators
//
// const IRGeometryProxy& IRGeometryProxy::operator=( const IRGeometryProxy& iproxy )
// {
//   if( this != &iproxy ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iproxy );
//   }
//
//   return *this;
// }

//
// member functions
//
void
IRGeometryProxy::invalidateCache()
{
   delete m_IRGeometry ;
   m_IRGeometry = 0 ;
}

const IRGeometryProxy::value_type*
IRGeometryProxy::faultHandler( const Record& iRecord,
			       const DataKey& iKey )
{
#if defined(CLEO_DEBUG)
   report( DEBUG, kFacilityString )
      << "Entering faultHandler for IR Geometry" << endl;
#endif
    m_IRGeometry = new DGDetectorStore;

    if( 0 == m_IRGeometry )
    {
	report( EMERGENCY, kFacilityString )
	   << "can't allocate memory; aborting" << endl;
	assert( false );
	::exit( 1 );
    }
    
    report( VERIFY, kFacilityString ) << "Reading materials from the frame..."
				      << endl;
   
    DGMaterialStore  materials;
    extract( iRecord.frame().record( Stream::kBaseGeometry ) , materials );

    DGConstMaterialPtr pVacuum = materials[ kVacuumMatID ];

// Create a bounding mother volume to house all components

    // IR Polycone Bounding Volume Constants
    const int VERTEXNUM = 14;
    _vector_DGZrR_ motherRadiiVector;

    const IRreal zCoordinates[] = {
       0.0 * k_m,                                                      //0
       0.116225 * k_m,                                                 //1
       0.116225 * k_m,                                                 //2
       17.09/2.0 * k_in - (0.1472 + 0.8018) * k_in,                    //3
       17.09/2.0 * k_in - (0.1472 + 0.8018) * k_in,                    //4
       17.09/2.0 * k_in - (0.1472 + 0.8018) * k_in + (7.807-6.857) * k_in,//5
       17.09/2.0 * k_in - (0.1472 + 0.8018) * k_in + (7.807-6.857) * k_in,//6
       (7.807-3.710+( 17.09/2.0 - (0.1472 + 0.8018) )) * k_in,           //7
       (7.807-3.710+( 17.09/2.0 - (0.1472 + 0.8018) )) * k_in,           //8
       DRGeometryParams::kBoundingZ[3],                                  //9
       DRGeometryParams::kBoundingZ[4],                                  //10
       DRGeometryParams::kBoundingZ[5]+0.001,                            //11
       DRGeometryParams::kBoundingZ[5]+0.001,                            //12
       (14.426/2.0 - 1.257 + 0.713) * k_in + 243.0 * k_cm };            //13

    const IRreal outerRadii[] = {
       (1.814/2.0) * k_in,                                                //0
       (1.814/2.0) * k_in,                                                //1
       tan(acos(0.93)) * zCoordinates[2],                                 //2
       tan(acos(0.94)) * zCoordinates[3], // Had to change arccos arg here //3
                                          // to fit inside Silicon layer 2 
       3.199 * k_in,                                                       //4
       3.199 * k_in,                                                       //5
       3.141 * k_in,                                                       //6
       3.984 * k_in,                                                       //7
       4.479 * k_in,                                                       //8
       4.479 * k_in,                                                       //9
       DRGeometryParams::kBoundingRInner[4]-.001,                          //10
       DRGeometryParams::kBoundingRInner[4]-.001,                          //11
       16.9 * k_in  -0.001                 ,                               //12
       16.9 * k_in  -.001};                                                //13

    for (int i = 0; i < VERTEXNUM; ++i)
    {
       DGZrR breakPoint(zCoordinates[i],
			0.0,
			outerRadii[i]);
       motherRadiiVector.push_back(breakPoint);
    }    

    DGZrRList motherRadiiList( motherRadiiVector );

    // Build Bounding polycone volume
    DGVolumePtr spIRMother = 
	new DGVolumePolyCone( IRGeomParameters::kIRMotherVolName,
			      pVacuum,
			      motherRadiiList);
 
    DGVolumeParentPlaced* pParentPlacedIRMother =
	new DGVolumeParentPlaced( IRGeomParameters::kIRMotherVolName,
				  spIRMother );

    DGVolumePtr spParentPlacedIRMother( pParentPlacedIRMother );

    // Build a big dummy Lab Mother Volume
    const DGConstVolumePtr pLab ( new DGVolumeBox( // big lab volume
       "lab", k_nullMaterialPtr,
       Triplet<double,double,double>( 370.,370.,370. ) ) ) ;

    DGVolumeParentPlaced* vpLab = 
	new DGVolumeParentPlaced( IRGeomParameters::kCLEO, pLab );
    DGVolumePtr spLab( vpLab );

    // Pass mother volume to component classes

    // BeamPipeGeometry beamPipe(iRecord);
#if defined(CLEO_DEBUG)
   report( DEBUG, kFacilityString )
      << "...Building beam pipe" << endl;
#endif
    BeamPipeGeometry23 beamPipe(iRecord);
    beamPipe(pParentPlacedIRMother);


#if defined(CLEO_DEBUG)
   report( DEBUG, kFacilityString )
      << "...Building hybrid mask" << endl;
#endif
    MaskHybridGeometry hybridMask(iRecord);
    hybridMask(pParentPlacedIRMother);


#if defined(CLEO_DEBUG)
   report( DEBUG, kFacilityString )
      << "Building inner mask" << endl;
#endif
    MaskInnerGeometry innerMask(iRecord);
    innerMask(pParentPlacedIRMother);


#if defined(CLEO_DEBUG)
   report( DEBUG, kFacilityString )
      << "Building main mask" << endl;
#endif
    MaskMainGeometry mainMask(iRecord);
    mainMask(pParentPlacedIRMother);


#if defined(CLEO_DEBUG)
   report( DEBUG, kFacilityString )
      << "Building REQ" << endl;
#endif
    REQMagnetGeometry reqMagnet(iRecord);
    reqMagnet(pParentPlacedIRMother);


#if defined(CLEO_DEBUG)
   report( DEBUG, kFacilityString )
      << "Building stainless tube" << endl;
#endif
    StainlessTubeGeometry stainlessTube(iRecord);
    stainlessTube(pParentPlacedIRMother);
   
#if defined(CLEO_DEBUG)
   report( DEBUG, kFacilityString )
      << "Building silicon inner support" << endl;
#endif
    CuSupportGeometry cuSupport(iRecord);
    cuSupport(pParentPlacedIRMother);


    // Add Interaction Region to lab volume and set root

    // bkh 5/02: IR is now only half the region so 2 copies must be placed
    
    DGPlacedVolumePtr spPlacedIRMother = 
	new DGPlacedVolume( spParentPlacedIRMother,
			    1,
			    DGTransform::Identity,
			    DGTransform::Identity);

    vpLab->addChild( spPlacedIRMother );
    
    DGPlacedVolumePtr spPlacedIRMother2 = 
	new DGPlacedVolume( spParentPlacedIRMother,
			    2,
			    HepRotateX3D( M_PI ) ,
			    DGTransform::Identity  );

    vpLab->addChild( spPlacedIRMother2 );

    m_IRGeometry->setRoot( spLab );

#if defined(CLEO_DEBUG)
   report( DEBUG, kFacilityString )
      << "Leaving faultHandler for IR Geometry" << endl;
#endif
    return m_IRGeometry;
}

//
// const member functions
//

//
// static member functions
//


