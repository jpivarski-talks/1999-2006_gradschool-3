// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRGeomProd
// 
// Description: This feeds the Frame the ADR3, in its many forms.
//
// Implementation:
//     Note: Temporary: this version binds ADRSenseWireProxy to BaseGeometry
//
// Author:      Inga Karliner
// Created:     Sep 23, 1998
//
// Revision history
// updated to new Proxy registration method
// 7/27/99 add  CathodeProxy DRAlignement registration
// 8/31/99 remove headers DRGeom/DRFieldWireProxy.h,/DRSenseWireProxy.h"
//                              /DRCathodeProxy.h"
// 9/27/99 cleanup
// 
// New ADRGeom library
// 12/1/99 I.Karliner New library ADRGeomProd (was in ADRGeom)
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DRGeom/DRGeom.h"
#include "DRGeom/DRGeometryParams.h"
#include "DRGeom/DRPerfectProxy.h"
#include "DRGeom/DRAlignedProxy.h"
#include "ADRGeomProd/ADRGeomProd.h"
#include "ADRGeomProd/ADRSenseWireProxy.h"
#include "ADRGeomProd/ADRCathodeProxy.h"


#include "Experiment/report.h"

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"

//#include "ProxyDict/Ifp.hh"
//#include "ProxyDict/IfdStrKey.hh"

#include "DataHandler/ProxyFactory.h"

// STL classes (Do I need these?)
#include <string>
#include <vector>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Producer.ADRGeomProd" ;

//
// static data member definitions
//

//
// constructors and destructor
//
ADRGeomProd::ADRGeomProd( void )               // anal1
   : Producer( "ADRGeomProd" )
{
   // ----- register the Stream we're supplying proxies for ------
   // These lines ARE VERY IMPORTANT! If you don't tell us what
   // streams you provide proxies for, we won't call you!
   
  //   registerStream( Stream::kBaseGeometry );
  //   registerStream( Stream::kDRAlignment );
   registerProxyFactory( Stream::kBaseGeometry,
			 new ProxyFactory<ADRSenseWireProxy>,
			 UsageTag() );

   //go to alignment when available: 
   registerProxyFactory( Stream::kDRAlignment,
   			 new ProxyFactory<ADRSenseWireProxy>,
   			 UsageTag() );

   registerProxyFactory( Stream::kBaseGeometry,
			 new ProxyFactory<ADRCathodeProxy>,
			 UsageTag() ); 
	 
  
   registerProxyFactory( Stream::kDRAlignment,
			 new ProxyFactory<ADRCathodeProxy>,
			 UsageTag() );
   
}

#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory<ADRSenseWireProxy>;
template class ProxyFactory<ADRCathodeProxy>;
// ADRGeomProd::ADRGeomProd( const ADRGeomProd& iADRGeomProd )
// {
//    *this = iADRGeomProd;
// }

ADRGeomProd::~ADRGeomProd()                    // anal5
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const ADRGeomProd& ADRGeomProd::operator=( const ADRGeomProd& iADRGeomProd )
// {
//   if( this != &iADRGeomProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iADRGeomProd );
//   }
//
//   return *this;
// }

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
ADRGeomProd::init( void )          // anal1 "Interactive"
{
   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
ADRGeomProd::terminate( void )     // anal5 "Interactive"
{
   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
ADRGeomProd::hist_book( HIHistoManager& )
{
   // book your histograms here

}

// ---------------- main method to register proxies -----------------
//void
//ADRGeomProd::registerProxies( const Stream::Type& aStream,
//			   KeyedProxies& aProxyList )
//{
//   IfdDataProxyIFace* proxy ;

//   if( aStream == Stream::kBaseGeometry ) 
//   {
     // The ADRSenseWireProxy proxy: go to Alignment when implemented
//     ADRSenseWireProxy* adrSenseWireProxy = new ADRSenseWireProxy;
//      if( 0 != adrSenseWireProxy  ) {
//	 aProxyList.push_back( 
//	    KeyedProxy( 
//	       Ifp< ADRSenseWireProxy::value_type >::makeKey(),
//	      adrSenseWireProxy )
//	    );
//     }
//      
//   }
//}

//
// const member functions
//

//
// static member functions
//








