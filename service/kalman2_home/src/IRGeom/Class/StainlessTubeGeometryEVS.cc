// -*- C++ -*-
//
// Package:     IRGeom
// Module:      StainlessTubeGeometryEVS
// 
// Description: function class to create G3iExtraVolSpecsStore for IRGeom
//              Stainless steel support tube only
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 14:58:43 EDT 1999
// $Id: StainlessTubeGeometryEVS.cc,v 1.2 2001/03/15 21:58:56 bkh Exp $
//
// Revision history
//
// $Log: StainlessTubeGeometryEVS.cc,v $
// Revision 1.2  2001/03/15 21:58:56  bkh
// Extra VOl SPecs now lives in Start Run not Base Geometry
//
// Revision 1.1.1.1  1999/07/22 14:38:43  lkg
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
#include "IRGeom/StainlessTubeGeometryEVS.h"

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

static const char* const kFacilityString = "IRGeom.StainlessTubeGeometryEVS" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: StainlessTubeGeometryEVS.cc,v 1.2 2001/03/15 21:58:56 bkh Exp $";
static const char* const kTagString = "$Name: v01_01_02 $";

//
// static data member definitions
//

//
// constructors and destructor
//
StainlessTubeGeometryEVS::StainlessTubeGeometryEVS(const Record& iRecord)
    : m_Record(iRecord)
{
}

// StainlessTubeGeometryEVS::StainlessTubeGeometryEVS( const StainlessTubeGeometryEVS& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

StainlessTubeGeometryEVS::~StainlessTubeGeometryEVS()
{
}

//
// assignment operators
//
// const StainlessTubeGeometryEVS& StainlessTubeGeometryEVS::operator=( const StainlessTubeGeometryEVS& rhs )
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
StainlessTubeGeometryEVS::operator()(G3iExtraVolSpecsStore& store)
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
	report(INFO, kFacilityString) << "Adding Stainless Tube EVS" << endl;
	
	G3iExtraVolSpecs* extra(0);

	// Needed tracking media
	const G3iTrackingMedium& stainlessTM (aMedia[kStainless316TMedID]);

	extra = store.insert(IRGeomParameters::kStainlessTubeVolName,
			     IRGeomEVSParameters::kStainlessTubeVolName4,
			     stainlessTM);

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
