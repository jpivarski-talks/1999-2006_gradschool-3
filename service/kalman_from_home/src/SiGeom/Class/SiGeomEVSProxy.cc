// -*- C++ -*-
//
// Package:     SiGeom
// Module:      SiGeomEVSProxy
// 
// Description: Proxy to create a G3iExtraVolSpecsStore for SiGeom
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Mon Jun 14 10:02:49 EDT 1999
// $Id: SiGeomEVSProxy.cc,v 1.12 2002/04/30 17:42:47 bkh Exp $
//
// Revision history
//
// $Log: SiGeomEVSProxy.cc,v $
// Revision 1.12  2002/04/30 17:42:47  bkh
// Change silicon "tungsten" back to Silicon!
//
// Revision 1.11  2002/03/04 21:18:00  bkh
// Make parent volumes enclose children completely; revamp of ladder object
//
// Revision 1.10  2001/03/15 21:54:36  bkh
// ExtraVolSpecs now lives in StartRun not base geometry
//
// Revision 1.9  2000/11/15 21:53:10  cdj
// now inherits from ProxyTemplate instead of ProxyBindableTemplate
//
// Revision 1.8  2000/02/03 14:45:30  pg
//   Assigned unique names for all ladders and their contents.
// So, now we have 61 separate volumes to hold ladders, with sensors
// inside getting unique names as well (61 of them). This is forced
// by GEANT requirements.
//
// Revision 1.7  2000/01/06 02:55:00  undrus
// Vees become parts of ladders; Names of Vees get the layer number
//
// Revision 1.6  1999/11/16 20:25:39  pg
// Modifications for use in Cleog3
//
// Revision 1.5  1999/09/02 18:36:05  pg
// Modified ExtraVolSpecs proxy for Si to be placed inside coil, as prescribed by Brian
//
// Revision 1.4  1999/07/27 22:31:10  lkg
// Al -> CarbonFiber for tube medium, tweaks for SunOs/CC compilation
//
// Revision 1.3  1999/07/01 14:43:21  pg
// Added Instantiate directory. Moved ExtraVolSpecs template instantiation to the separate file.
//
// Revision 1.2  1999/06/30 22:34:52  pg
// Fixed a bug for Extra Volume Spec proxy. Couple of bugs fixed in SiGeometryProxy: copy numbers for Vees and bounding volumes for the lab frame caused problems for the Geant3Interface.
//
// Revision 1.1  1999/06/29 19:42:05  lkg
// Add Si3 Geant3 extra volume spec's information and associated proxy
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
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "DataHandler/Frame.h"

#include "SiGeom/SiGeomEVSProxy.h"
#include "SiGeom/SiGeant3SetNames.h"
#include "SiGeom/SiGeomDescription.h"
#include "SiGeom/SiGeomDescriptionEVS.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD


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
static const char* const kFacilityString = "SiGeom.SiGeomEVSProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: SiGeomEVSProxy.cc,v 1.12 2002/04/30 17:42:47 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
SiGeomEVSProxy::SiGeomEVSProxy()
   : m_G3iExtraVolSpecsStore( 0 )
{
}

// SiGeomEVSProxy::SiGeomEVSProxy( const SiGeomEVSProxy& iproxy )
// {
//   *this = iproxy;
// }

SiGeomEVSProxy::~SiGeomEVSProxy()
{
   delete m_G3iExtraVolSpecsStore ;
}

//
// assignment operators
//
// const SiGeomEVSProxy& SiGeomEVSProxy::operator=( const SiGeomEVSProxy& iproxy )
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
SiGeomEVSProxy::invalidateCache()
{
   delete m_G3iExtraVolSpecsStore ;
   m_G3iExtraVolSpecsStore = 0 ;
}

const SiGeomEVSProxy::value_type*
SiGeomEVSProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
    assert(0 == m_G3iExtraVolSpecsStore);
    
    FATable<G3iTrackingMedium> mediaTable;
    extract( iRecord, mediaTable );

    if (!mediaTable.valid())
    {
	report(WARNING, kFacilityString)
	    << "No Media table found!" << endl;
    }
    else
    {
	m_G3iExtraVolSpecsStore = &(buildEVSS(mediaTable));
    }

   
    return ( m_G3iExtraVolSpecsStore ) ;
}

//
// const member functions
//

G3iExtraVolSpecsStore&
SiGeomEVSProxy::buildEVSS(const G3iTrackingMediaStore& aMedia)
{
    G3iExtraVolSpecsStore& store(*new G3iExtraVolSpecsStore);
    
    G3iExtraVolSpecs* extra(0);

    // Needed tracking media
    const G3iTrackingMedium& vacuumTM   (aMedia[kVacuumTMedID]); 
    const G3iTrackingMedium& siliconTM  (aMedia[kSiliconWaferTMedID]);
    const G3iTrackingMedium& carbonTM   (aMedia[kSVCarbonFiberTMedID]); 
    const G3iTrackingMedium& airTM      (aMedia[kAirLoCutFieldTMedID]);  
    
    // Diamond Media Type currently being added (verify indentifier choice)
    const G3iTrackingMedium& diamondTM  (aMedia[kDiamondTMedID]);


    extra = store.insert(SiGeomDescription::kNameLabFrame, 
			 SiGeomDescriptionEVS::kNameLabFrame4, 
			 vacuumTM);

    extra = store.insert(SiGeomDescription::kVolumeNameSi,
			 SiGeomDescriptionEVS::kVolumeNameSi4,
			 airTM);

    extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			   G3iExtraVolSpecs::k_seenUnseen  );

//    extra->insertAttribute( G3iExtraVolSpecs::k_color ,
//			   G3iExtraVolSpecs::k_colorGreen ) ;

    extra = store.insert(SiGeomDescription::kSiBBName,
			 SiGeomDescriptionEVS::kSiBBName4,
			 airTM);

    extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			   G3iExtraVolSpecs::k_seenUnseen  );

//    extra->insertAttribute( G3iExtraVolSpecs::k_color ,
//			   G3iExtraVolSpecs::k_colorGreen ) ;

    extra = store.insert(SiGeomDescription::kCylName,
			 SiGeomDescriptionEVS::kCylName4,
			 carbonTM);


    extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			   G3iExtraVolSpecs::k_seenUnseen  );




    STL_VECTOR( G3iHitElementSpec ) hits ;
    hits.reserve(14) ;
    hits.push_back( G3iHitElementSpec( 21,  20.,   100000., "XINW" ) ) ;
    hits.push_back( G3iHitElementSpec( 21,  20.,   100000., "YINW" ) ) ;
    hits.push_back( G3iHitElementSpec( 22,  30.,   100000., "ZINW" ) ) ;
    hits.push_back( G3iHitElementSpec( 21,   0.,    10000., "P   " ) ) ;
    hits.push_back( G3iHitElementSpec( 21,  10.,   100000., "XINL" ) ) ;
    hits.push_back( G3iHitElementSpec( 21,  10.,   100000., "YINL" ) ) ;
    hits.push_back( G3iHitElementSpec( 22,  30.,   100000., "ZINL" ) ) ;
    hits.push_back( G3iHitElementSpec( 21,  10.,   100000., "XOUT" ) ) ;
    hits.push_back( G3iHitElementSpec( 21,  10.,   100000., "YOUT" ) ) ;
    hits.push_back( G3iHitElementSpec( 22,  30.,   100000., "ZOUT" ) ) ;
    hits.push_back( G3iHitElementSpec( "WAFN", 0., 512., 1.  ) ) ;
    hits.push_back( G3iHitElementSpec( 21,   0.,   100000., "S   " ) ) ;
    hits.push_back( G3iHitElementSpec( 32,   0., 10000000., "EDEP" ) ) ;
    hits.push_back( G3iHitElementSpec( 22,   0.,    10000., "TLEN" ) ) ;

    const string setName ( SILICON_SETNAME ) ;    
// Ladder counter
    int ilc = 0;
    for (int index = 0; index < 4; ++index)
    {
//Layers
	extra = store.insert(SiGeomDescription::kLayerName[index],
			     SiGeomDescriptionEVS::kLayerName4[index],
			     airTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenUnseen  );

	extra = store.insert(SiGeomDescription::kLayerBBName[index],
			     SiGeomDescriptionEVS::kLayerBBName4[index],
			     airTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenUnseen  );

	for (int ilad = 0; ilad<SiGeomDescription::NPhiStep[index] ; ++ilad)
	{
// Ladders
	   extra = store.insert(SiGeomDescription::kLadderName[ilc],
				SiGeomDescriptionEVS::kLadderName4[ilc],
				airTM);
//	   extra->insertAttribute(G3iExtraVolSpecs::k_seen,
//				  G3iExtraVolSpecs::k_seenUnseen  );
	   
//	   extra->insertAttribute( G3iExtraVolSpecs::k_color ,
//				   G3iExtraVolSpecs::k_colorGreen ) ;
	   extra = store.insert(SiGeomDescription::kSensorName[ilc],
				SiGeomDescriptionEVS::kSensorName4[ilc],
				siliconTM);
	   
	   extra->insertAttribute(G3iExtraVolSpecs::k_seen,
				  G3iExtraVolSpecs::k_seenSeen  );
	   
	   extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				   G3iExtraVolSpecs::k_colorGreen ) ;
	   
	   extra->setSensitiveVolume( G3iSensitiveVolume( setName, 
							  3, 1000, hits ) ) ; 
	   
	   ilc++;
	}


	extra = store.insert(SiGeomDescription::kLadderBBName[index],
			     SiGeomDescriptionEVS::kLadderBBName4[index],
			     airTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenUnseen  );

        extra = store.insert(SiGeomDescription::kVeeName[index],
	        	     SiGeomDescriptionEVS::kVeeName4[index],
			     airTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

        extra = store.insert(SiGeomDescription::kVeeBBName[index],
	        	     SiGeomDescriptionEVS::kVeeBBName4[index],
			     airTM);


	extra = store.insert(SiGeomDescription::kVeeSideName[index],
			     SiGeomDescriptionEVS::kVeeSideName4[index],
			     diamondTM);

//	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
//				G3iExtraVolSpecs::k_colorGreen ) ;

//	extra->setSensitiveVolume( G3iSensitiveVolume( setName, 
//						       3, 1000, hits ) ) ; 
    }

    return store;
}


//
// static member functions
//


