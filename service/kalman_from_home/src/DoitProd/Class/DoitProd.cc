// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Fri Jul 23 17:06:16 EDT 1999
// $Id: DoitProd.cc,v 1.9 2000/08/11 07:17:23 lyon Exp $
//
// Revision history
//
// $Log: DoitProd.cc,v $
// Revision 1.9  2000/08/11 07:17:23  lyon
// Change DoitModeNumber to 38
//
// Revision 1.8  2000/06/09 00:42:09  lkg
// New DABoolean parameter UseSilicon, and member fcn to access the parameter
// value (useSilicon()) added.
//
// Revision 1.7  2000/05/30 20:45:18  pg
// Added proxies for Si lattices
//
// Revision 1.6  2000/05/27 05:31:28  lyon
// Handle cathode clusters on tracks
//
// Revision 1.5  2000/02/09 16:45:37  lyon
// changed mode number
//
// Revision 1.4  2000/02/08 18:37:35  lyon
// Added DoitModeNumber parameter
//
// Revision 1.3  2000/01/14 02:54:32  lyon
// Add parameters to continue if hits are missing
//
// Revision 1.2  1999/10/31 16:11:11  lyon
// Fixes to call clearCache
//
// Revision 1.1.1.1  1999/09/08 18:53:33  lyon
// imported DoitProd sources
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DoitProd/DoitProd.h"
#include "Experiment/report.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"
#include "DoitProd/DoitProxy.h"
#include "DoitProd/DoitTRSeedTrackProxy.h"
#include "DoitProd/DoitLatticeProxy.h"
#include "DoitProd/DoitSVRLatticeProxy.h"
#include "DoitProd/DoitSVZLatticeProxy.h"
#include "DoitProd/DoitCathodeLatticeProxy.h"
#include "DoitProd/DoitBaseQualityProxy.h"
#include "DoitProd/DoitSeedQualityProxy.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DoitProd.DoitProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitProd.cc,v 1.9 2000/08/11 07:17:23 lyon Exp $";
static const char* const kTagString = "$Name: v01_10_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoitProd::DoitProd( void )               // anal1
   : Producer( "DoitProd" ),
     m_diagLevel("DiagLevel", this, 0),
     m_modeNumber("DoitModeNumber", this, 38),
     m_continueIfHitsMissing("ContinueIfHitsMissing", this, false ),
     m_continueIfAnyException("ContinueIfAnyException", this, false ),
     m_useSilicon("UseSilicon", this, true )
{
  
  m_modeNumber.setHelpString("Use the default unless told otherwise!\n");
   
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
			 new ProxyFactory< DoitTRSeedTrackProxy >, 
			 UsageTag() );

   registerProxyFactory( Stream::kEvent, 
			 new ProxyFactory< DoitLatticeProxy >, 
			 UsageTag() );

   registerProxyFactory( Stream::kEvent, 
			 new ProxyFactory< DoitSVRLatticeProxy >, 
			 UsageTag() );

   registerProxyFactory( Stream::kEvent, 
			 new ProxyFactory< DoitSVZLatticeProxy >, 
			 UsageTag() );

   registerProxyFactory( Stream::kEvent, 
			 new ProxyFactory< DoitCathodeLatticeProxy >, 
			 UsageTag() );

   registerProxyFactory( Stream::kEvent, 
			 new ProxyFactory< DoitSeedQualityProxy >, 
			 UsageTag() );

   registerProxyFactory( Stream::kEvent, 
			 new ProxyFactory< DoitBaseQualityProxy >, 
			 UsageTag() );

   // or
   //    register non-default-ctor proxy 
   //       (don't forget to write "DoitProd::makeMyProxy()")
   registerProxyFactory( 
      Stream::kEvent, 
      new ProxyFactoryFunction< DoitProd, DoitProxy >( *this, 
						&DoitProd::makeDoitProxy ),
      UsageTag() );
}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory< DoitTRSeedTrackProxy >;
template class ProxyFactory< DoitLatticeProxy >;
template class ProxyFactory< DoitSVRLatticeProxy >;
template class ProxyFactory< DoitSVZLatticeProxy >;
template class ProxyFactory< DoitCathodeLatticeProxy >;
template class ProxyFactory< DoitSeedQualityProxy >;
template class ProxyFactory< DoitBaseQualityProxy >;
#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< DoitProd, DoitProxy >;

// non-trivial proxyfactory producer method used above
// (if you want to use it, don't forget to declare this method 
//  in "producer.h"!)
ProxyBase*
DoitProd::makeDoitProxy()
{ 
   return new DoitProxy( this );
}

// DoitProd::DoitProd( const DoitProd& iDoitProd )
// {
//    *this = iDoitProd;
// }

DoitProd::~DoitProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const DoitProd& DoitProd::operator=( const DoitProd& iDoitProd )
// {
//   if( this != &iDoitProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iDoitProd );
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
DoitProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DoitProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
DoitProd::hist_book( TBHistoManager& )
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

