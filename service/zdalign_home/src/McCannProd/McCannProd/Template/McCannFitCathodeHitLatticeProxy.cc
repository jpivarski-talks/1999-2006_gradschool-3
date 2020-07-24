// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannFitCathodeHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep  7 01:03:34 EDT 1999
// $Id: McCannFitCathodeHitLatticeProxy.cc,v 1.6 2002/06/17 20:01:00 cleo3 Exp $
//
// Revision history
//
// $Log: McCannFitCathodeHitLatticeProxy.cc,v $
// Revision 1.6  2002/06/17 20:01:00  cleo3
// added missing typename
//
// Revision 1.5  2001/03/27 06:43:02  wsun
// Extract McCann objects with same production tag as McCannProd.
//
// Revision 1.4  2000/06/09 00:57:38  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.3  2000/03/02 08:57:26  wsun
// Cast helix identifier to correct type instead of using find().
//
// Revision 1.2  2000/02/26 22:37:33  cdj
// LatticeProxies now use McCannProd's running options, e.g. catch exceptions if requested
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from McCannFilter
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

#include "McCannProd/McCannFitCathodeHitLatticeProxy.h"
#include "McCannFilter/McCannTrackFitter.h"

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
static const char* const kFacilityString = "McCannFitCathodeHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: McCannFitCathodeHitLatticeProxy.cc,v 1.6 2002/06/17 20:01:00 cleo3 Exp $";
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
McCannFitCathodeHitLatticeProxy< H >::McCannFitCathodeHitLatticeProxy()
   : m_McCannFitCathodeHitLattice( 0 )
{
}

// McCannFitCathodeHitLatticeProxy::McCannFitCathodeHitLatticeProxy( const McCannFitCathodeHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
McCannFitCathodeHitLatticeProxy< H >::~McCannFitCathodeHitLatticeProxy()
{
   delete m_McCannFitCathodeHitLattice ;
}

//
// assignment operators
//
// const McCannFitCathodeHitLatticeProxy& McCannFitCathodeHitLatticeProxy::operator=( const McCannFitCathodeHitLatticeProxy& iproxy )
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
McCannFitCathodeHitLatticeProxy< H >::invalidateCache()
{
   delete m_McCannFitCathodeHitLattice ;
   m_McCannFitCathodeHitLattice = 0 ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename McCannFitCathodeHitLatticeProxy< H >::value_type*
McCannFitCathodeHitLatticeProxy< H >::faultHandler( const Record& iRecord,
						    const DataKey& iKey )
{
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_McCannFitCathodeHitLattice );

   // ---------- create the new object --------------------

   // Get the McCannTrackFitter, and call event().
   FAItem< McCannTrackFitter > fitter;
   extract( iRecord, fitter, "", iKey.production().value() );
   
   // Get the FATables to be linked and instantiate a new Lattice.
   FATable< TRHelixCandidateFit< H > > candHelices ;
   extract( iRecord, candHelices, "", iKey.production().value() ) ;

   if( fitter->useDRCathodeHits() ) {
      try {
	 FATable< CalibratedCathodeHit > cathodeHits ;
	 extract( iRecord, cathodeHits ) ;
	 
	 m_McCannFitCathodeHitLattice =
	    new value_type( candHelices,
			    cathodeHits,
			    value_type::LNodeMulti ) ;
	 
	 // Fit the tracks under the hypothesis H.
	 fitter->event( ( Frame& ) iRecord.frame(),
			( TrackFitter::HypoType ) H ) ;
	 
	 // Get the vector of McCannHelix pointers for this hypothesis.
	 STL_VECTOR( McCannHelix* )* kalmanHelices =
	    fitter->fittedHelices( ( TrackFitter::HypoType ) H ) ;
	 
	 // Link the fitted helices with their hits -- the TRHelixCandidateFit
	 // and the corresponding McCannHelix objects have the same identifier.
	 STL_VECTOR( McCannHelix* )::iterator helixItr =kalmanHelices->begin();
	 STL_VECTOR( McCannHelix* )::iterator helixEnd = kalmanHelices->end() ;
	 
	 for( ; helixItr != helixEnd ; ++helixItr )
	 {
	    STL_VECTOR( McCannHelix::HitAndLinkData )::const_iterator hitIter =
	       ( *helixItr )->hitsOnTrack().begin() ;
	    STL_VECTOR( McCannHelix::HitAndLinkData )::const_iterator last =
	       ( *helixItr )->hitsOnTrack().end() ;
	    
	    for( ; hitIter != last ; ++hitIter )
	    {
	       if( hitIter->calibratedHit()->deviceType() ==
		   CalibratedHit::Cathode )
	       {
		  if( m_McCannFitCathodeHitLattice->connect(
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

   if( 0 == m_McCannFitCathodeHitLattice ) {
      //return an empty lattice
      auto_ptr< FAPtrTable< CalibratedCathodeHit > > cathodeTable( 
	 new FAPtrTable< CalibratedCathodeHit > );
      
      FATable< CalibratedCathodeHit > cathodeHits( &(*cathodeTable) );
      
      m_McCannFitCathodeHitLattice =
	 new value_type( candHelices,
			 cathodeHits,
			 value_type::LNodeMulti ) ;
   }
   return ( m_McCannFitCathodeHitLattice ) ;
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
//McCannFitCathodeHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}


