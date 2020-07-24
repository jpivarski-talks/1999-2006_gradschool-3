// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanFitCathodeHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:03:34 EDT 1999
// $Id: KalmanFitCathodeHitLatticeProxy.cc,v 1.6 2002/06/17 20:01:00 cleo3 Exp $
//
// Revision history
//
// $Log: KalmanFitCathodeHitLatticeProxy.cc,v $
// Revision 1.6  2002/06/17 20:01:00  cleo3
// added missing typename
//
// Revision 1.5  2001/03/27 06:43:02  wsun
// Extract Kalman objects with same production tag as KalmanProd.
//
// Revision 1.4  2000/06/09 00:57:38  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.3  2000/03/02 08:57:26  wsun
// Cast helix identifier to correct type instead of using find().
//
// Revision 1.2  2000/02/26 22:37:33  cdj
// LatticeProxies now use KalmanProd's running options, e.g. catch exceptions if requested
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from KalmanFilter
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
#include <memory>

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "KalmanProd/KalmanFitCathodeHitLatticeProxy.h"
#include "KalmanFilter/KalmanTrackFitter.h"

#include "FrameAccess/FAPtrTable.h"
#include "DAException/DAException.h"
#include "DAException/DANoDataException.h"

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
static const char* const kFacilityString = "KalmanFitCathodeHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: KalmanFitCathodeHitLatticeProxy.cc,v 1.6 2002/06/17 20:01:00 cleo3 Exp $";
static const char* const kTagString = "$Name: v02_03_00 $";

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
KalmanFitCathodeHitLatticeProxy< H >::KalmanFitCathodeHitLatticeProxy()
   : m_KalmanFitCathodeHitLattice( 0 )
{
}

// KalmanFitCathodeHitLatticeProxy::KalmanFitCathodeHitLatticeProxy( const KalmanFitCathodeHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
KalmanFitCathodeHitLatticeProxy< H >::~KalmanFitCathodeHitLatticeProxy()
{
   delete m_KalmanFitCathodeHitLattice ;
}

//
// assignment operators
//
// const KalmanFitCathodeHitLatticeProxy& KalmanFitCathodeHitLatticeProxy::operator=( const KalmanFitCathodeHitLatticeProxy& iproxy )
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
KalmanFitCathodeHitLatticeProxy< H >::invalidateCache()
{
   delete m_KalmanFitCathodeHitLattice ;
   m_KalmanFitCathodeHitLattice = 0 ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename KalmanFitCathodeHitLatticeProxy< H >::value_type*
KalmanFitCathodeHitLatticeProxy< H >::faultHandler( const Record& iRecord,
						    const DataKey& iKey )
{
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_KalmanFitCathodeHitLattice );

   // ---------- create the new object --------------------

   // Get the KalmanTrackFitter, and call event().
   FAItem< KalmanTrackFitter > fitter;
   extract( iRecord, fitter, "", iKey.production().value() );
   
   // Get the FATables to be linked and instantiate a new Lattice.
   FATable< TRHelixCandidateFit< H > > candHelices ;
   extract( iRecord, candHelices, "", iKey.production().value() ) ;

   if( fitter->useDRCathodeHits() ) {
      try {
	 FATable< CalibratedCathodeHit > cathodeHits ;
	 extract( iRecord, cathodeHits ) ;
	 
	 m_KalmanFitCathodeHitLattice =
	    new value_type( candHelices,
			    cathodeHits,
			    value_type::LNodeMulti ) ;
	 
	 // Fit the tracks under the hypothesis H.
	 fitter->event( ( Frame& ) iRecord.frame(),
			( TrackFitter::HypoType ) H ) ;
	 
	 // Get the vector of KalmanHelix pointers for this hypothesis.
	 STL_VECTOR( KalmanHelix* )* kalmanHelices =
	    fitter->fittedHelices( ( TrackFitter::HypoType ) H ) ;
	 
	 // Link the fitted helices with their hits -- the TRHelixCandidateFit
	 // and the corresponding KalmanHelix objects have the same identifier.
	 STL_VECTOR( KalmanHelix* )::iterator helixItr =kalmanHelices->begin();
	 STL_VECTOR( KalmanHelix* )::iterator helixEnd = kalmanHelices->end() ;
	 
	 for( ; helixItr != helixEnd ; ++helixItr )
	 {
	    STL_VECTOR( KalmanHelix::HitAndLinkData )::const_iterator hitIter =
	       ( *helixItr )->hitsOnTrack().begin() ;
	    STL_VECTOR( KalmanHelix::HitAndLinkData )::const_iterator last =
	       ( *helixItr )->hitsOnTrack().end() ;
	    
	    for( ; hitIter != last ; ++hitIter )
	    {
	       if( hitIter->calibratedHit()->deviceType() ==
		   CalibratedHit::Cathode )
	       {
		  if( m_KalmanFitCathodeHitLattice->connect(
		     ( value_type::LeftID )( *helixItr )->identifier(),
		     hitIter->calibratedHit()->identifier(),
		     *( (TrackFitCathodeHitLink*) hitIter->linkData() ) ) ==0 )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "Cathode Lattice connect failed." << endl ;
		  }
	       }
	    }
	 }
      } 
      catch( NO_TABLE_EXCEPTION( CalibratedCathodeHit )& aException )
      {
	 if(! fitter->continueIfHitsMissing() ) {
	    // This rethrows the excpetion currently being handled.
	    throw;
	 }
      }
      catch( DAExceptionBase& aException )
      {
	 if( ! fitter->continueIfAnyException() ) {
	    // This rethrows the excpetion currently being handled.
	    throw;
	 }
      }
   }

   if( 0 == m_KalmanFitCathodeHitLattice ) {
      //return an empty lattice
      auto_ptr< FAPtrTable< CalibratedCathodeHit > > cathodeTable( 
	 new FAPtrTable< CalibratedCathodeHit > );
      
      FATable< CalibratedCathodeHit > cathodeHits( &(*cathodeTable) );
      
      m_KalmanFitCathodeHitLattice =
	 new value_type( candHelices,
			 cathodeHits,
			 value_type::LNodeMulti ) ;
   }
   return ( m_KalmanFitCathodeHitLattice ) ;
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
//KalmanFitCathodeHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}


