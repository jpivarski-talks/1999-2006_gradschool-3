// -*- C++ -*-
//
// Package:     IRGeom
// Module:      IRGeometryEVSProxy
// 
// Description: Proxy to create G3iExtraVolSpecsStore for IRGeom
//
// Implementation:
//     No extra vol specs defined here.  This module combines component stores
//     into one main store.
//
// Author:      Aren Jansen
// Created:     Tue Jun 15 17:45:35 EDT 1999
// $Id: IRGeometryEVSProxy.cc,v 1.4 2001/03/15 21:58:54 bkh Exp $
//
// Revision history
//
// $Log: IRGeometryEVSProxy.cc,v $
// Revision 1.4  2001/03/15 21:58:54  bkh
// Extra VOl SPecs now lives in Start Run not Base Geometry
//
// Revision 1.3  2000/11/15 21:58:40  cdj
// proxies no longer inherit from ProxyBindableTemplate to aid static linking
//
// Revision 1.2  2000/02/02 15:13:45  lkg
// Deal properly with CoilGeom mother volume
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

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "CoilGeom/CoilAirName.h"


#include "IRGeom/IRGeometryEVSProxy.h"
#include "IRGeom/IRGeomParameters.h"
#include "IRGeom/IRGeomEVSParameters.h"

// Modules for given IRGeom components
#include "IRGeom/BeamPipeGeometryEVS.h"
#include "IRGeom/MaskMainGeometryEVS.h"
#include "IRGeom/MaskInnerGeometryEVS.h"
#include "IRGeom/MaskHybridGeometryEVS.h"
#include "IRGeom/REQMagnetGeometryEVS.h"
#include "IRGeom/StainlessTubeGeometryEVS.h"
#include "IRGeom/CuSupportGeometryEVS.h"

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
static const char* const kFacilityString = "IRGeom.IRGeometryEVSProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: IRGeometryEVSProxy.cc,v 1.4 2001/03/15 21:58:54 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
IRGeometryEVSProxy::IRGeometryEVSProxy()
   : m_G3iExtraVolSpecsStore( 0 )
{
}

// IRGeometryEVSProxy::IRGeometryEVSProxy( const IRGeometryEVSProxy& iproxy )
// {
//   *this = iproxy;
// }

IRGeometryEVSProxy::~IRGeometryEVSProxy()
{
   delete m_G3iExtraVolSpecsStore ;
}

//
// assignment operators
//
// const IRGeometryEVSProxy& IRGeometryEVSProxy::operator=( const IRGeometryEVSProxy& iproxy )
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
IRGeometryEVSProxy::invalidateCache()
{
    delete m_G3iExtraVolSpecsStore ;
    m_G3iExtraVolSpecsStore = 0 ;
}

const IRGeometryEVSProxy::value_type*
IRGeometryEVSProxy::faultHandler( const Record& iRecord,
				  const DataKey& iKey )
{
    assert (0 == m_G3iExtraVolSpecsStore);

    m_G3iExtraVolSpecsStore = new G3iExtraVolSpecsStore;

    FATable<G3iTrackingMedium> aMedia;
    extract( iRecord, aMedia );

    if (!aMedia.valid())
    {
	report(WARNING, kFacilityString)
	    << "No Media table found!" << endl;
    }
    else
    {
	G3iExtraVolSpecs* extra(0);

	// Needed tracking media
	const G3iTrackingMedium& vacuumTM    (aMedia[kVacuumTMedID]); 

       	extra = m_G3iExtraVolSpecsStore->insert(IRGeomEVSParameters::kLabName4,
			     kCoilAirName4, 
			     vacuumTM);

	extra = m_G3iExtraVolSpecsStore->insert(
	    IRGeomParameters::kIRMotherVolName,
	    IRGeomEVSParameters::kIRMotherVolName4,
	    vacuumTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenUnseen  );
	
 
        // When removing an IR component, remove its given following block

	BeamPipeGeometryEVS bPG(iRecord);
	bPG(*m_G3iExtraVolSpecsStore);

	MaskMainGeometryEVS mMG(iRecord);
	mMG(*m_G3iExtraVolSpecsStore);

	MaskInnerGeometryEVS mIG(iRecord);
	mIG(*m_G3iExtraVolSpecsStore);

	MaskHybridGeometryEVS mHG(iRecord);
	mHG(*m_G3iExtraVolSpecsStore);

	REQMagnetGeometryEVS rEQMG(iRecord);
	rEQMG(*m_G3iExtraVolSpecsStore);

	StainlessTubeGeometryEVS sTG(iRecord);
	sTG(*m_G3iExtraVolSpecsStore);    

	CuSupportGeometryEVS cSG(iRecord);
	cSG(*m_G3iExtraVolSpecsStore);    

    }

    return ( m_G3iExtraVolSpecsStore ) ;
}

//
// const member functions
//

//
// static member functions
//






