// -*- C++ -*-
//
// Package:     <IRGeom>
// Module:      IRGeomProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lawrence Gibbons
// Created:     Thu Jan  7 14:54:35 EST 1999
// $Id: IRGeomProd.cc,v 1.3 2001/03/15 21:58:54 bkh Exp $
//
// Revision history
//
// $Log: IRGeomProd.cc,v $
// Revision 1.3  2001/03/15 21:58:54  bkh
// Extra VOl SPecs now lives in Start Run not Base Geometry
//
// Revision 1.2  1999/07/22 21:58:12  lkg
// Finish producer name change: IRGeom -> IRGeomProd
//
// Revision 1.1.1.1  1999/07/22 14:38:43  lkg
// First release
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "IRGeom/IRGeomProd.h"
#include "IRGeom/IRGeometryProxy.h"
#include "IRGeom/IRGeometryEVSProxy.h"
#include "IRGeom/IRGeomParameters.h"

#include "Experiment/report.h"

#include "DataHandler/Frame.h"
#include "DataHandler/Record.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

// STL classes
#include <map>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "IRGeom.IRGeomProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: IRGeomProd.cc,v 1.3 2001/03/15 21:58:54 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
IRGeomProd::IRGeomProd( void )               // anal1
   : Producer( "IRGeomProd" )
{
#if defined( CLEO_DEBUG )
   report( DEBUG, kFacilityString ) << "constructing IRGeomProd" << endl;
#endif

   // ----- register proxy factories ------------------

   // Proxy Registration Step 1:
   registerProxyFactory( Stream::kBaseGeometry, 
			 new ProxyFactory< IRGeometryProxy >, 
			 UsageTag( IRGeomParameters::kUsageTag) );  

   registerProxyFactory( Stream::kStartRun, 
			 new ProxyFactory< IRGeometryEVSProxy >, 
			 UsageTag( IRGeomParameters::kUsageTag) );     
}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory< IRGeometryProxy >;
template class ProxyFactory< IRGeometryEVSProxy >;

#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< IRGeomProd, IRGeometryProxy >;
template class ProxyFactoryFunction< IRGeomProd, IRGeometryEVSProxy >;


// IRGeom::IRGeom( const IRGeom& iIRGeom )
// {
//    *this = iIRGeom;
// }

IRGeomProd::~IRGeomProd()                    // anal5
{
#if defined( CLEO_DEBUG )
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
#endif
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
IRGeomProd::init( void )          // anal1 "Interactive"
{
#if defined( CLEO_DEBUG )
   report( DEBUG, kFacilityString ) << "here in init()" << endl;
#endif

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
IRGeomProd::terminate( void )     // anal5 "Interactive"
{
#if defined( CLEO_DEBUG )
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;
#endif

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
IRGeomProd::hist_book( TBHistoManager& )
{
#if defined( CLEO_DEBUG )
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;
#endif
   // book your histograms here

}
