// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannDBTrackerValuesProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 23:19:13 EDT 1999
// $Id: McCannDBTrackerValuesProxy.cc,v 1.2 2000/06/19 06:19:42 wsun Exp $
//
// Revision history
//
// $Log: McCannDBTrackerValuesProxy.cc,v $
// Revision 1.2  2000/06/19 06:19:42  wsun
// Removed use of TBThreeVector.
//
// Revision 1.1.1.1  1999/09/10 21:58:58  wsun
// Transfered from McCannFilter
//
// Revision 1.3  1999/08/03 22:35:26  wsun
// Extract McCannTrackFitter in faultHandler() so HIMagField is initialized.
//
// Revision 1.2  1999/06/24 22:44:24  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.1  1999/05/14 01:12:58  wsun
// First submission.
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

#include "McCannProd/McCannDBTrackerValuesProxy.h"
#include "McCannFilter/McCannTrackFitter.h"
#include "HelixIntersection/HIMagField.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#if 0
#include "TrackDelivery/TDKinematicPionFit.h" 
#endif

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
static const char* const kFacilityString = "McCannDBTrackerValuesProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: McCannDBTrackerValuesProxy.cc,v 1.2 2000/06/19 06:19:42 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
McCannDBTrackerValuesProxy::McCannDBTrackerValuesProxy()
   : m_McCannDBTrackerValues( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &McCannDBTrackerValuesProxy::boundMethod, Stream::kBeginRun );
}

// McCannDBTrackerValuesProxy::McCannDBTrackerValuesProxy( const McCannDBTrackerValuesProxy& iproxy )
// {
//   *this = iproxy;
// }

McCannDBTrackerValuesProxy::~McCannDBTrackerValuesProxy()
{
   delete m_McCannDBTrackerValues ;
}

//
// assignment operators
//
// const McCannDBTrackerValuesProxy& McCannDBTrackerValuesProxy::operator=( const McCannDBTrackerValuesProxy& iproxy )
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
McCannDBTrackerValuesProxy::invalidateCache()
{
   delete m_McCannDBTrackerValues ;
   m_McCannDBTrackerValues = 0 ;
}

const McCannDBTrackerValuesProxy::value_type*
McCannDBTrackerValuesProxy::faultHandler( const Record& iRecord,
					  const DataKey& iKey )
{
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_McCannDBTrackerValues );

   // ---------- create the new object --------------------

   // Extract the McCannTrackFitter so that the HIMagField is initialized.
   FAItem< McCannTrackFitter > fitter ;
   extract( iRecord, fitter ) ;

   HepVector3D magneticField =
     HIMagField::instance()->bfield( HepPoint3D( 0., 0., 0. ) ) ;
   GeV masses[ DBCandidate::kMaxHypo ];
   for ( SmallCount hypo = 0 ;
         hypo != DBCandidate::kMaxHypo ;
         ++hypo )
   {
      masses[ hypo ] = DBCandidate::mass( DBCandidate::Hypo( hypo ) );
   }
   m_McCannDBTrackerValues = new DBTrackerValues( magneticField, masses );
   if( 0 == m_McCannDBTrackerValues )
   {
      report( EMERGENCY, kFacilityString )
         << "can't allocate memory; aborting" << endl;
      assert( false );
      ::exit( 1 );
   }

   
   return ( m_McCannDBTrackerValues ) ;
}

//
// const member functions
//

//
// static member functions
//

//
// 'bound' functions
//

//void
//McCannDBTrackerValuesProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
PROXY_BIND_METHOD( McCannDBTrackerValuesProxy )
//-----------------------------------------------

