// -*- C++ -*-
//
// Package:     <DualTrackHelicesProd>
// Module:      DualTrackHelicesProd
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Fri Jul  7 14:15:27 EDT 2000
// $Id: DualTrackHelicesProd.cc,v 1.1.1.1 2001/02/01 17:40:14 llh14 Exp $
//
// Revision history
//
// $Log: DualTrackHelicesProd.cc,v $
// Revision 1.1.1.1  2001/02/01 17:40:14  llh14
// imported DualTrackHelicesProd
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
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DualTrackHelicesProd/DualTrackHelicesProd.h"
#include "DualTrackHelicesProd/DualTrackHelicesProxy.h"
#include "DualTrackHelices/DualTrackHelices.h"

#include "HelixIntersection/HIMagField.h"

#include "Experiment/report.h"

#include "DataHandler/RecordMethods.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/ProxyFactoryFunction.h"

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
static const char* const kFacilityString = "DualTrackHelicesProd.DualTrackHelicesProd" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackHelicesProd.cc,v 1.1.1.1 2001/02/01 17:40:14 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrackHelicesProd::DualTrackHelicesProd( void )               // anal1
   : Producer( "DualTrackHelicesProd" ),
     m_UseAllHits("UseAllHits", this, true),
     m_UseLay1("UseLayer1", this, true),
     m_UseLay2("UseLayer2", this, true),
     m_UseLay3("UseLayer3", this, true),
     m_UseLay4("UseLayer4", this, true),
     m_svrWeight("SVRFittingWeight", this, 4.4e7),
     m_svzWeight("SVZFittingWeight", this, 8.2e4)
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

//   bind( &DualTrackHelicesProd::beginRun, Stream::kBeginRun);

   registerProxyFactory( 
                         Stream::kEvent, 
			 new ProxyFactory< DualTrackHelicesProxy >, 
			 UsageTag() );    // or UsageTag( "MyUsage" )

   registerProxyFactory( Stream::kEvent, 
			 new ProxyFactoryFunction< DualTrackHelicesProd, 
			 DualTrackHelicesProxy >
			 (*this, 
			  &DualTrackHelicesProd::makeDualTrackHelicesProxy), 
			 UsageTag() );    // or UsageTag( "MyUsage" )

}

// Proxy Registration Step 2:
// template instantiations for proxy factories
#include "DataHandler/Template/ProxyFactory.cc"
template class ProxyFactory< DualTrackHelicesProxy >;

#include "DataHandler/Template/ProxyFactoryFunction.cc"
template class ProxyFactoryFunction< DualTrackHelicesProd, DualTrackHelicesProxy >;

// non-trivial proxyfactory producer method used above
// (if you want to use it, don't forget to declare this method 
//  in "producer.h"!)

ProxyBase*
DualTrackHelicesProd::makeDualTrackHelicesProxy()
{ 
   return new DualTrackHelicesProxy( this );
}

// DualTrackHelicesProd::DualTrackHelicesProd( const DualTrackHelicesProd& iDualTrackHelicesProd )
// {
//    *this = iDualTrackHelicesProd;
// }

DualTrackHelicesProd::~DualTrackHelicesProd()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// assignment operators
//
// const DualTrackHelicesProd& DualTrackHelicesProd::operator=( const DualTrackHelicesProd& iDualTrackHelicesProd )
// {
//   if( this != &iDualTrackHelicesProd ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iDualTrackHelicesProd );
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
DualTrackHelicesProd::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
DualTrackHelicesProd::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}

// ---------------- standard place to book histograms ---------------
void
DualTrackHelicesProd::hist_book( HIHistoManager& )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

//ActionBase::ActionResult
//ProxyExample::beginRun( Frame& iFrame )
//{

// We must initialize the magnitice field using the beginrun record
//   HIMagField::instance()->setBfield( iFrame.record(Stream::kBeginRun) );

//   return ActionBase::kPassed
//}


//
// const member functions
//

//
// static member functions
//
