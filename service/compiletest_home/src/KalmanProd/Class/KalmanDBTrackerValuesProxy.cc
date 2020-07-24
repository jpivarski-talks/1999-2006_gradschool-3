// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanDBTrackerValuesProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 23:19:13 EDT 1999
// $Id: KalmanDBTrackerValuesProxy.cc,v 1.2 2000/06/19 06:19:42 wsun Exp $
//
// Revision history
//
// $Log: KalmanDBTrackerValuesProxy.cc,v $
// Revision 1.2  2000/06/19 06:19:42  wsun
// Removed use of TBThreeVector.
//
// Revision 1.1.1.1  1999/09/10 21:58:58  wsun
// Transfered from KalmanFilter
//
// Revision 1.3  1999/08/03 22:35:26  wsun
// Extract KalmanTrackFitter in faultHandler() so HIMagField is initialized.
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

#include "KalmanProd/KalmanDBTrackerValuesProxy.h"
#include "KalmanFilter/KalmanTrackFitter.h"
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
static const char* const kFacilityString = "KalmanDBTrackerValuesProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanDBTrackerValuesProxy.cc,v 1.2 2000/06/19 06:19:42 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
KalmanDBTrackerValuesProxy::KalmanDBTrackerValuesProxy()
   : m_KalmanDBTrackerValues( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &KalmanDBTrackerValuesProxy::boundMethod, Stream::kBeginRun );
}

// KalmanDBTrackerValuesProxy::KalmanDBTrackerValuesProxy( const KalmanDBTrackerValuesProxy& iproxy )
// {
//   *this = iproxy;
// }

KalmanDBTrackerValuesProxy::~KalmanDBTrackerValuesProxy()
{
   delete m_KalmanDBTrackerValues ;
}

//
// assignment operators
//
// const KalmanDBTrackerValuesProxy& KalmanDBTrackerValuesProxy::operator=( const KalmanDBTrackerValuesProxy& iproxy )
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
KalmanDBTrackerValuesProxy::invalidateCache()
{
   delete m_KalmanDBTrackerValues ;
   m_KalmanDBTrackerValues = 0 ;
}

const KalmanDBTrackerValuesProxy::value_type*
KalmanDBTrackerValuesProxy::faultHandler( const Record& iRecord,
					  const DataKey& iKey )
{
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_KalmanDBTrackerValues );

   // ---------- create the new object --------------------

   // Extract the KalmanTrackFitter so that the HIMagField is initialized.
   FAItem< KalmanTrackFitter > fitter ;
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
   m_KalmanDBTrackerValues = new DBTrackerValues( magneticField, masses );
   if( 0 == m_KalmanDBTrackerValues )
   {
      report( EMERGENCY, kFacilityString )
         << "can't allocate memory; aborting" << endl;
      assert( false );
      ::exit( 1 );
   }

   
   return ( m_KalmanDBTrackerValues ) ;
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
//KalmanDBTrackerValuesProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
PROXY_BIND_METHOD( KalmanDBTrackerValuesProxy )
//-----------------------------------------------

