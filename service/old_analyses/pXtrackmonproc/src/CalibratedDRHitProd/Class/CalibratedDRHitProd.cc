// -*- C++ -*-
//
// Package:     <CalibratedDRHitProd>
// Module:      CalibratedDRHitProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Sep 22 14:17:43 EDT 1999
// $Id: CalibratedDRHitProd.cc,v 1.4 2001/11/29 22:18:09 bkh Exp $
//
// Revision history
//
// $Log: CalibratedDRHitProd.cc,v $
// Revision 1.4  2001/11/29 22:18:09  bkh
// Restored raw dr hit proxy back to RawData producer
// insert raw charge and time into hit object
//
// Revision 1.2  2000/02/08 18:24:32  lyon
// Moved preliminary usage tag string to CalibratedData
//
// Revision 1.1.1.1  1999/09/23 02:44:15  cdj
// imported CalibratedDRHitProd sources
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
#include "CalibratedDRHitProd/CalibratedDRHitProd.h"
#include "Experiment/report.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

#include "CalibratedDRHitProd/CDRCalibratedDRHitProxy.h"
#include "CalibratedData/CalibratedDRHit.h"
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
static const char* const kFacilityString = "CalibratedDRHitProd.CalibratedDRHitProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: CalibratedDRHitProd.cc,v 1.4 2001/11/29 22:18:09 bkh Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
CalibratedDRHitProd::CalibratedDRHitProd( void )               // anal1
   : Producer( "CalibratedDRHitProd" )
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
			 new ProxyFactory< CDRCalibratedDRHitProxy >, 
			 UsageTag() );    // or UsageTag( "MyUsage" )
}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory< CDRCalibratedDRHitProxy >;


// CalibratedDRHitProd::CalibratedDRHitProd( const CalibratedDRHitProd& iCalibratedDRHitProd )
// {
//    *this = iCalibratedDRHitProd;
// }

CalibratedDRHitProd::~CalibratedDRHitProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const CalibratedDRHitProd& CalibratedDRHitProd::operator=( const CalibratedDRHitProd& iCalibratedDRHitProd )
// {
//   if( this != &iCalibratedDRHitProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iCalibratedDRHitProd );
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
CalibratedDRHitProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
CalibratedDRHitProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
CalibratedDRHitProd::hist_book( HIHistoManager& )
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
