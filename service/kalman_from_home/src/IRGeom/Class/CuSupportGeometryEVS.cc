// -*- C++ -*-
//
// Package:     IRGeom
// Module:      CuSupportGeometryEVS
// 
// Description: Function class to create G3iExtraVolSpecsStore for IRGeom
//              Copper Support Structure
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Aren Jansen
// Created:     Mon Jun 28 15:06:13 EDT 1999
// $Id: CuSupportGeometryEVS.cc,v 1.4 2001/03/15 21:58:53 bkh Exp $
//
// Revision history
//
// $Log: CuSupportGeometryEVS.cc,v $
// Revision 1.4  2001/03/15 21:58:53  bkh
// Extra VOl SPecs now lives in Start Run not Base Geometry
//
// Revision 1.3  2000/02/02 15:08:10  lkg
// Eliminate some valid() checks since exceptions get thrown anyway.
//
// Revision 1.2  1999/07/23 22:23:46  lkg
// Encase various for loops in a pair of {} to deal with screwy loop index
// scope under SunOs/CC
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
//#include "Experiment/report.h"
#include "Experiment/report.h"
#include "IRGeom/CuSupportGeometryEVS.h"

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

static const char* const kFacilityString = "IRGeom.CuSupportGeometryEVS" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CuSupportGeometryEVS.cc,v 1.4 2001/03/15 21:58:53 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
CuSupportGeometryEVS::CuSupportGeometryEVS(const Record& iRecord)
   : m_Record(iRecord)
{
}

// CuSupportGeometryEVS::CuSupportGeometryEVS( const CuSupportGeometryEVS& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

CuSupportGeometryEVS::~CuSupportGeometryEVS()
{
}

//
// assignment operators
//
// const CuSupportGeometryEVS& CuSupportGeometryEVS::operator=( const CuSupportGeometryEVS& rhs )
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
CuSupportGeometryEVS::operator()(G3iExtraVolSpecsStore& store)
{
    FATable<G3iTrackingMedium> aMedia;
    extract( m_Record.frame().record(Stream::kStartRun), aMedia );

    report(INFO, kFacilityString) 
       << "Adding Copper/Carbon Support EVS" << endl;
	
    G3iExtraVolSpecs* extra(0);

    // Needed tracking media
    const G3iTrackingMedium& defaultTM  (aMedia[kVacuumTMedID]);
    const G3iTrackingMedium& copperTM   (aMedia[kCopperTMedID]);
    const G3iTrackingMedium& carbonTM   (aMedia[kSVCarbonFiberTMedID]);
    const G3iTrackingMedium& aluminumTM (aMedia[kAluminum6061TMedID]);
    const G3iTrackingMedium& beOTM      (aMedia[kBerylliumOxideTMedID]);
	
    extra = store.insert(IRGeomParameters::kCuMainStructVolName,
			 IRGeomEVSParameters::kCuMainStructVolName4,
			 copperTM);

    extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			   G3iExtraVolSpecs::k_seenSeen  );

    extra->insertAttribute( G3iExtraVolSpecs::k_color ,
			    G3iExtraVolSpecs::k_colorRed ) ;

    { // change scope because SunOS/CC screws up scope of loop variable
       for (int i = 0; i < IRGeomParameters::kNumBoxes4; ++i)
       {
	  extra = store.insert(IRGeomParameters::kCuRadialBoxVolName[i],
			       IRGeomEVSParameters::kCuRadialBoxVolName4[i],
			       copperTM);
	   
	  extra->insertAttribute(G3iExtraVolSpecs::k_seen,
				 G3iExtraVolSpecs::k_seenSeen  );
	   
	  extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				  G3iExtraVolSpecs::k_colorRed ) ;
       }
    } // end of scope change


    extra = store.insert(IRGeomParameters::kBeOBoardVolName,
			 IRGeomEVSParameters::kBeOBoardVolName4,
			 beOTM);
	
    extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			   G3iExtraVolSpecs::k_seenSeen  );
	
    extra->insertAttribute( G3iExtraVolSpecs::k_color ,
			    G3iExtraVolSpecs::k_colorRed ) ;


    extra = store.insert(IRGeomParameters::kBeOCuPadVolName,
			 IRGeomEVSParameters::kBeOCuPadVolName4,
			 beOTM);
    
    extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			   G3iExtraVolSpecs::k_seenSeen  );
    
    extra->insertAttribute( G3iExtraVolSpecs::k_color ,
			    G3iExtraVolSpecs::k_colorRed ) ;
    
    { // change scope because SunOS/CC screws up scope of loop variable
       for (int i = 0; i < IRGeomParameters::kNumBoxes1; ++i)
       {
	  extra = store.insert(
	     IRGeomParameters::kBeOHybridL1VolName[i],
	     IRGeomEVSParameters::kBeOHybridL1VolName4[i],
	     defaultTM);
	  
	  extra->insertAttribute(G3iExtraVolSpecs::k_seen,
				 G3iExtraVolSpecs::k_seenSeen  );
	  
	  extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				  G3iExtraVolSpecs::k_colorRed ) ;
       }
    } // end of scope change

    { // change scope because SunOS/CC screws up scope of loop variable
       for (int i = 0; i < IRGeomParameters::kNumBoxes2; ++i)
       {
	  extra = store.insert(
	     IRGeomParameters::kBeOHybridL2VolName[i],
	     IRGeomEVSParameters::kBeOHybridL2VolName4[i],
	     defaultTM);
	  
	  extra->insertAttribute(G3iExtraVolSpecs::k_seen,
				 G3iExtraVolSpecs::k_seenSeen  );
	  
	  extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				  G3iExtraVolSpecs::k_colorRed ) ;
       }
    } // end of scope change
    
    { // change scope because SunOS/CC screws up scope of loop variable
       for (int i = 0; i < IRGeomParameters::kNumBoxes3; ++i)
       {
	  extra = store.insert(
	     IRGeomParameters::kBeOHybridL3VolName[i],
	     IRGeomEVSParameters::kBeOHybridL3VolName4[i],
	     defaultTM);
	  
	  extra->insertAttribute(G3iExtraVolSpecs::k_seen,
				 G3iExtraVolSpecs::k_seenSeen  );
	  
	  extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				  G3iExtraVolSpecs::k_colorRed ) ;
       }
    } // end of scope change
    
    { // change scope because SunOS/CC screws up scope of loop variable
       for (int i = 0; i < IRGeomParameters::kNumBoxes4; ++i)
       {
	  extra = store.insert(
	     IRGeomParameters::kBeOHybridL4Set1VolName[i],
	     IRGeomEVSParameters::kBeOHybridL4Set1VolName4[i],
	     defaultTM);
	  
	  extra->insertAttribute(G3iExtraVolSpecs::k_seen,
				 G3iExtraVolSpecs::k_seenSeen  );
	  
	  extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				  G3iExtraVolSpecs::k_colorRed ) ;
	  
	  extra = store.insert(
	     IRGeomParameters::kBeOHybridL4Set2VolName[i],
	     IRGeomEVSParameters::kBeOHybridL4Set2VolName4[i],
	     defaultTM);
	  
	  extra->insertAttribute(G3iExtraVolSpecs::k_seen,
				 G3iExtraVolSpecs::k_seenSeen  );
	  
	  extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				  G3iExtraVolSpecs::k_colorRed ) ;
       }
    } // end of scope change
    
    extra = store.insert(IRGeomParameters::kCFiberConeVolName,
			 IRGeomEVSParameters::kCFiberConeVolName4,
			 carbonTM);
    
    extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			   G3iExtraVolSpecs::k_seenSeen  );
    
    extra->insertAttribute( G3iExtraVolSpecs::k_color ,
			    G3iExtraVolSpecs::k_colorRed ) ;
    
    
    
    extra = store.insert(IRGeomParameters::kAlInnerConeVolName,
			 IRGeomEVSParameters::kAlInnerConeVolName4,
			 aluminumTM);
    
    extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			   G3iExtraVolSpecs::k_seenSeen  );
    
    extra->insertAttribute( G3iExtraVolSpecs::k_color ,
			    G3iExtraVolSpecs::k_colorRed ) ;
    
    
    extra = store.insert(IRGeomParameters::kAlOuterConeVolName,
			 IRGeomEVSParameters::kAlOuterConeVolName4,
			 aluminumTM);
    
    extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			   G3iExtraVolSpecs::k_seenSeen  );
    
    extra->insertAttribute( G3iExtraVolSpecs::k_color ,
			    G3iExtraVolSpecs::k_colorRed ) ;
    
    
    { // change scope because SunOS/CC screws up scope of loop variable
       for (int i = 0; i < IRGeomParameters::kAlNumBoxes1; ++i)
       {
	  extra = store.insert(
	     IRGeomParameters::kAlRadialBoxVolName1[i],
	     IRGeomEVSParameters::kAlRadialBoxVolName14[i],
		 aluminumTM);
	   
	      extra->insertAttribute(G3iExtraVolSpecs::k_seen,
				     G3iExtraVolSpecs::k_seenSeen  );
	   
	      extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				      G3iExtraVolSpecs::k_colorRed ) ;
	   }
	} // end of scope change

	{ // change scope because SunOS/CC screws up scope of loop variable
	   for (int i = 0; i < IRGeomParameters::kAlNumBoxes2; ++i)
	   {
	      extra = store.insert(
		 IRGeomParameters::kAlRadialBoxVolName2[i],
		 IRGeomEVSParameters::kAlRadialBoxVolName24[i],
		 aluminumTM);
	   
	      extra->insertAttribute(G3iExtraVolSpecs::k_seen,
				     G3iExtraVolSpecs::k_seenSeen  );
	   
	      extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				      G3iExtraVolSpecs::k_colorRed ) ;
	   }
	} // end of scope change


	extra = store.insert(IRGeomParameters::kCuInnerConeVolName,
			     IRGeomEVSParameters::kCuInnerConeVolName4,
			     copperTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;
	   
	extra = store.insert(
	   IRGeomParameters::kCuInnerLipVolName,
	   IRGeomEVSParameters::kCuInnerLipVolName4,
	   copperTM);
	   
	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );
	   
	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;
	
	
	extra = store.insert(IRGeomParameters::kAl12NoseConeVolName,
			     IRGeomEVSParameters::kAl12NoseConeVolName4,
			     copperTM);

	extra->insertAttribute(G3iExtraVolSpecs::k_seen,
			       G3iExtraVolSpecs::k_seenSeen  );

	extra->insertAttribute( G3iExtraVolSpecs::k_color ,
				G3iExtraVolSpecs::k_colorRed ) ;
	
}

//
// const member functions
//

//
// static member functions
//
