// -*- C++ -*-
//
// Package:     <CalibratedZDHitProd>
// Module:      CalibratedZDHitProd
// 
// Description: 
//
// Implementation:
//
// Author:      Brian Heltsley
// Created:     Wed Sep 22 14:17:43 EDT 1999
// $Id: CalibratedZDHitProd.cc,v 1.1.1.1 2002/09/27 20:34:02 bkh Exp $
//
// Revision history
//
// $Log: CalibratedZDHitProd.cc,v $
// Revision 1.1.1.1  2002/09/27 20:34:02  bkh
// imported CalibratedZDHitProd sources
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
#include "CalibratedZDHitProd/CalibratedZDHitProd.h"
#include "Experiment/report.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

#include "CalibratedZDHitProd/CalibratedZDHitProxy.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "CalibratedData/CDPreliminaryID.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
#include <map>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "CalibratedZDHitProd.CalibratedZDHitProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CalibratedZDHitProd.cc,v 1.1.1.1 2002/09/27 20:34:02 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
CalibratedZDHitProd::CalibratedZDHitProd( void )               // anal1
   : Producer( "CalibratedZDHitProd" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ----- register proxy factories ------------------
   // (Please see the "ProxyExample" library for examples!)
   //
   // Two steps: 1) use "registerProxyFactory" to register
   //            2) instantiate ProxyFactory/Function
   //               (if you're using the standard templated factories;
   //                you may of course roll your own!)

   // Proxy Registration Step 1:
   // either
   //    register default-ctor proxy
   registerProxyFactory( Stream::kEvent, 
			 new ProxyFactory< CalibratedZDHitProxy >, 
			 UsageTag() );    // or UsageTag( "MyUsage" )
}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory< CalibratedZDHitProxy >;


// CalibratedZDHitProd::CalibratedZDHitProd( const CalibratedZDHitProd& iCalibratedZDHitProd )
// {
//    *this = iCalibratedZDHitProd;
// }

CalibratedZDHitProd::~CalibratedZDHitProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const CalibratedZDHitProd& CalibratedZDHitProd::operator=( const CalibratedZDHitProd& iCalibratedZDHitProd )
// {
//   if( this != &iCalibratedZDHitProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iCalibratedZDHitProd );
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
CalibratedZDHitProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
CalibratedZDHitProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
CalibratedZDHitProd::hist_book( HIHistoManager& )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

//
// const member functions
//

//
// static member functions
//
