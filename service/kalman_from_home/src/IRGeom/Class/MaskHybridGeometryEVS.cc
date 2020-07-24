// -*- C++ -*-
//
// Package:     IRGeom
// Module:      MaskHybridGeometryEVS
// 
// Description: function class to create G3iExtraVolSpecsStore for IRGeom
//              Hybrid pipe Tungsten mask only
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Wed Jun 16 14:57:56 EDT 1999
// $Id: MaskHybridGeometryEVS.cc,v 1.2 2001/03/15 21:58:55 bkh Exp $
//
// Revision history
//
// $Log: MaskHybridGeometryEVS.cc,v $
// Revision 1.2  2001/03/15 21:58:55  bkh
// Extra VOl SPecs now lives in Start Run not Base Geometry
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
#include "IRGeom/MaskHybridGeometryEVS.h"

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

static const char* const kFacilityString = "IRGeom.MaskHybridGeometryEVS" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MaskHybridGeometryEVS.cc,v 1.2 2001/03/15 21:58:55 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
MaskHybridGeometryEVS::MaskHybridGeometryEVS(const Record& iRecord)
    : m_Record(iRecord)
{
}

// MaskHybridGeometryEVS::MaskHybridGeometryEVS( const MaskHybridGeometryEVS& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MaskHybridGeometryEVS::~MaskHybridGeometryEVS()
{
}

//
// assignment operators
//
// const MaskHybridGeometryEVS& MaskHybridGeometryEVS::operator=( const MaskHybridGeometryEVS& rhs )
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
MaskHybridGeometryEVS::operator()(G3iExtraVolSpecsStore& store)
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
	report(INFO, kFacilityString) << "Adding Hybrid Mask EVS" << endl;

	G3iExtraVolSpecs* extra(0);

	// Needed tracking media
        const G3iTrackingMedium& tungstenTM  (aMedia[kTungstenTMedID]);


	extra = store.insert(IRGeomParameters::kHybridTungstenMaskVol1Name,
			     IRGeomEVSParameters::kHybridTungstenMaskVol1Name4,
			     tungstenTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;


	extra = store.insert(IRGeomParameters::kHybridTungstenMaskVol2Name,
			     IRGeomEVSParameters::kHybridTungstenMaskVol2Name4,
			     tungstenTM);

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
