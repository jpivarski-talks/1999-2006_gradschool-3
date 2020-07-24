// -*- C++ -*-
//
// Package:     IRGeom
// Module:      BeamPipeGeometryEVS
// 
// Description: function class to create G3iExtraVolSpecsStore for 
//              beam pipe only 
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Tue Jun 15 18:12:16 EDT 1999
// $Id: BeamPipeGeometryEVS.cc,v 1.3 2001/03/15 21:58:51 bkh Exp $
//
// Revision history
//
// $Log: BeamPipeGeometryEVS.cc,v $
// Revision 1.3  2001/03/15 21:58:51  bkh
// Extra VOl SPecs now lives in Start Run not Base Geometry
//
// Revision 1.2  2000/03/06 18:38:53  lkg
// Break Cu pipe into two sections to avoid GEANT limit on volume parameters
//
// Revision 1.1.1.1  1999/07/22 14:38:42  lkg
// First release
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "IRGeom/BeamPipeGeometryEVS.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "DataHandler/Frame.h"

#include "IRGeom/IRGeomParameters.h"
#include "IRGeom/IRGeomEVSParameters.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "IRGeom.BeamPipeGeometryEVS" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: BeamPipeGeometryEVS.cc,v 1.3 2001/03/15 21:58:51 bkh Exp $";
static const char* const kTagString = "$Name: v01_01_02 $";

//
// static data member definitions
//

//
// constructors and destructor
//
BeamPipeGeometryEVS::BeamPipeGeometryEVS(const Record& iRecord)
    : m_Record(iRecord)
{
}

// BeamPipeGeometryEVS::BeamPipeGeometryEVS( const BeamPipeGeometryEVS& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

BeamPipeGeometryEVS::~BeamPipeGeometryEVS()
{
}

//
// assignment operators
//
// const BeamPipeGeometryEVS& BeamPipeGeometryEVS::operator=( const BeamPipeGeometryEVS& rhs )
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

void
BeamPipeGeometryEVS::operator()(G3iExtraVolSpecsStore& store)
{
    FATable<G3iTrackingMedium> aMedia;
    extract( m_Record.frame().record(Stream::kStartRun), aMedia );

    if (!aMedia.valid())
    {
	report(WARNING, kFacilityString)
	    << "No Media table found!" << endl;
    }
    else
    {
	report(INFO, kFacilityString) << "Adding Beam Pipe EVS" << endl;

	G3iExtraVolSpecs* extra(0);

	// Needed tracking media
	const G3iTrackingMedium& goldTM      (aMedia[kGoldTMedID]);
	const G3iTrackingMedium& berylliumTM (aMedia[kBerylliumTMedID]); 
	const G3iTrackingMedium& copperTM    (aMedia[kCopperTMedID]);
	const G3iTrackingMedium& waterTM     (aMedia[kWaterTMedID]);
	const G3iTrackingMedium& aluminumTM  (aMedia[kAluminumLoCutTMedID]);


	extra = store.insert(IRGeomParameters::kAuLiningVolName,
			     IRGeomEVSParameters::kAuLiningVolName4,
			     goldTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;

	extra = store.insert(IRGeomParameters::kOuterBePipeVolName,
			     IRGeomEVSParameters::kOuterBePipeVolName4,
			     berylliumTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;


	extra = store.insert(IRGeomParameters::kInnerBePipeVolName,
			     IRGeomEVSParameters::kInnerBePipeVolName4,
			     berylliumTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;


	// the small z portion of the Cu Pipe
	extra = store.insert(IRGeomParameters::kCuPipeVolName1,
			     IRGeomEVSParameters::kCuPipeVolName4_1,
			     copperTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;

	// the large z portion of the Cu Pipe
	extra = store.insert(IRGeomParameters::kCuPipeVolName2,
			     IRGeomEVSParameters::kCuPipeVolName4_2,
			     copperTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;


	extra = store.insert(IRGeomParameters::kCoolantPipeVolName,
			     IRGeomEVSParameters::kCoolantPipeVolName4,
			     waterTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;


	extra = store.insert(IRGeomParameters::kAlCapVolName,
			     IRGeomEVSParameters::kAlCapVolName4,
			     aluminumTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;

    }    
}


//
// const member functions
//

//
// static member functions
//








