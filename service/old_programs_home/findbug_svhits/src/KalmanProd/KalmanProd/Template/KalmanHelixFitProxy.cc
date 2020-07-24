// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanHelixFitProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon May 10 22:49:14 EDT 1999
// $Id: KalmanHelixFitProxy.cc,v 1.2 2001/03/27 06:43:05 wsun Exp $
//
// Revision history
//
// $Log: KalmanHelixFitProxy.cc,v $
// Revision 1.2  2001/03/27 06:43:05  wsun
// Extract Kalman objects with same production tag as KalmanProd.
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from KalmanFilter
//
// Revision 1.3  1999/07/15 23:01:36  cdj
// now compiles with CC optimization on
//
// Revision 1.2  1999/06/18 19:27:51  wsun
// More improvements to low momentum fitting.
//
// Revision 1.1  1999/05/14 01:12:13  wsun
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
//#include <vector>
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
#include "FrameAccess/FAPtrTable.h"

#include "KalmanProd/KalmanHelixFitProxy.h"
#include "KalmanFilter/KalmanTrackFitter.h"


// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "KalmanHelixFitProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanHelixFitProxy.cc,v 1.2 2001/03/27 06:43:05 wsun Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
KalmanHelixFitProxy< H >::KalmanHelixFitProxy()
   : m_ptrTable( new value_type )
{
   if ( 0 == m_ptrTable ) {
      report( EMERGENCY, kFacilityString )
         << "Unable to allocate memory"
         << endl ;
      ::exit( 1 ) ;
   }
}

// KalmanHelixFitProxy::KalmanHelixFitProxy( const KalmanHelixFitProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
KalmanHelixFitProxy< H >::~KalmanHelixFitProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const KalmanHelixFitProxy& KalmanHelixFitProxy::operator=( const KalmanHelixFitProxy& iproxy )
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
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
void
KalmanHelixFitProxy< H >::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const KalmanHelixFitProxy< H >::value_type*
KalmanHelixFitProxy< H >::faultHandler( const Record& iRecord,
					const DataKey& iKey )
{
// ---------- sample implementation ---------------------
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // ---------- fill FAPtrTable with new items ----------------

   // Get the KalmanTrackFitter, and call event().
   FAItem< KalmanTrackFitter > fitter;
   extract( iRecord, fitter, "", iKey.production().value() );

   // Fit the tracks under the hypothesis H.
   fitter->event( ( Frame& ) iRecord.frame(), ( TrackFitter::HypoType ) H ) ;

   // Get the vector of KalmanHelix pointers for this hypothesis.
   STL_VECTOR( KalmanHelix* )* kalmanHelices =
      fitter->fittedHelices( ( TrackFitter::HypoType ) H ) ;

   // Loop over all the KalmanHelix objects and transfer them to m_ptrTable.
   STL_VECTOR( KalmanHelix* )::iterator helixItr = kalmanHelices->begin() ;
   STL_VECTOR( KalmanHelix* )::iterator helixEnd = kalmanHelices->end() ;

   for( ; helixItr != helixEnd ; ++helixItr )
   {
      // The TRHelixCandidateFit will have the same identifier as the
      // KalmanHelix.
      m_ptrTable->insert( new TRHelixCandidateFit< H >(
	 ( *helixItr )->identifier(),
	 **helixItr ) ) ;
   }

   return ( m_ptrTable ) ;
}

//
// const member functions
//

//
// static member functions
//

