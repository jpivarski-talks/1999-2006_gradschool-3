// -*- C++ -*-
//
// Package:     <ChisqFilter>
// Module:      ChisqDBTrackerValuesProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 23:19:13 EDT 1999
// $Id: ChisqDBTrackerValuesProxy.cc,v 1.6 2001/02/15 01:50:43 wsun Exp $
//
// Revision history
//
// $Log: ChisqDBTrackerValuesProxy.cc,v $
// Revision 1.6  2001/02/15 01:50:43  wsun
// Extract ChisqTrackFitter in faultHandler() to so HIMagField is initialized.
//
// Revision 1.5  2000/09/06 21:58:05  wsun
// Included <vector> for OSF.
//
// Revision 1.4  2000/09/05 20:43:21  wsun
// Mods to allow user to force fitting weight to a given value.
//
// Revision 1.3  2000/06/16 18:21:21  wsun
// Replaced TBThreeVector with HepVector3D.
//
// Revision 1.2  1999/06/24 22:56:33  wsun
// Added use of HIMagField.
//
// Revision 1.1.1.1  1999/06/16 02:48:28  wsun
// First submission.
//
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

#include "ChisqFitProducer/ChisqDBTrackerValuesProxy.h"
#include "ChisqFitter/ChisqTrackFitter.h"
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
static const char* const kFacilityString = "ChisqDBTrackerValuesProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqDBTrackerValuesProxy.cc,v 1.6 2001/02/15 01:50:43 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ChisqDBTrackerValuesProxy::ChisqDBTrackerValuesProxy()
   : m_ChisqDBTrackerValues( 0 )
{
   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &ChisqDBTrackerValuesProxy::boundMethod, Stream::kBeginRun );
}

// ChisqDBTrackerValuesProxy::ChisqDBTrackerValuesProxy( const ChisqDBTrackerValuesProxy& iproxy )
// {
//   *this = iproxy;
// }

ChisqDBTrackerValuesProxy::~ChisqDBTrackerValuesProxy()
{
   delete m_ChisqDBTrackerValues ;
}

//
// assignment operators
//
// const ChisqDBTrackerValuesProxy& ChisqDBTrackerValuesProxy::operator=( const ChisqDBTrackerValuesProxy& iproxy )
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
ChisqDBTrackerValuesProxy::invalidateCache()
{
   delete m_ChisqDBTrackerValues ;
   m_ChisqDBTrackerValues = 0 ;
}

const ChisqDBTrackerValuesProxy::value_type*
ChisqDBTrackerValuesProxy::faultHandler( const Record& iRecord,
					 const DataKey& iKey )
{
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_ChisqDBTrackerValues );

   // ---------- create the new object --------------------

   // Extract the ChisqTrackFitter so that the HIMagField is initialized.
   FAItem< ChisqTrackFitter > fitter ;
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
   m_ChisqDBTrackerValues = new DBTrackerValues( magneticField, masses );
   if( 0 == m_ChisqDBTrackerValues )
   {
      report( EMERGENCY, kFacilityString )
         << "can't allocate memory; aborting" << endl;
      assert( false );
      ::exit( 1 );
   }

   
   return ( m_ChisqDBTrackerValues ) ;
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
//ChisqDBTrackerValuesProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
PROXY_BIND_METHOD( ChisqDBTrackerValuesProxy )
//-----------------------------------------------

#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< DBTrackerValues >;
