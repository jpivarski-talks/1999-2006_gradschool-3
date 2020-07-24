// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannFitSVRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon May 10 23:49:23 EDT 1999
// $Id: McCannFitSVRHitLatticeProxy.cc,v 1.7 2003/04/17 17:05:12 bkh Exp $
//
// Revision history
//
// $Log: McCannFitSVRHitLatticeProxy.cc,v $
// Revision 1.7  2003/04/17 17:05:12  bkh
// Add ZD Lattice Proxy & throw exceptions if wrong config
//
// Revision 1.6  2002/06/17 20:01:01  cleo3
// added missing typename
//
// Revision 1.5  2001/03/27 06:43:04  wsun
// Extract McCann objects with same production tag as McCannProd.
//
// Revision 1.4  2000/06/09 00:57:39  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.3  2000/03/02 08:57:27  wsun
// Cast helix identifier to correct type instead of using find().
//
// Revision 1.2  2000/02/26 22:37:33  cdj
// LatticeProxies now use McCannProd's running options, e.g. catch exceptions if requested
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from McCannFilter
//
// Revision 1.3  1999/07/15 23:01:34  cdj
// now compiles with CC optimization on
//
// Revision 1.2  1999/06/18 19:27:49  wsun
// More improvements to low momentum fitting.
//
// Revision 1.1  1999/05/14 01:12:11  wsun
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
#include <memory>

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "McCannProd/McCannFitSVRHitLatticeProxy.h"
#include "McCannFilter/McCannTrackFitter.h"

#include "FrameAccess/FAPtrTable.h"
#include "DAException/DAException.h"
#include "DAException/DANoDataException.h"

#include "DetectorConfiguration/DetectorConfiguration.h"

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
static const char* const kFacilityString = "McCannFitSVRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: McCannFitSVRHitLatticeProxy.cc,v 1.7 2003/04/17 17:05:12 bkh Exp $";
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
McCannFitSVRHitLatticeProxy< H >::McCannFitSVRHitLatticeProxy()
   : m_McCannFitSVRHitLattice( 0 )
{
}

// McCannFitSVRHitLatticeProxy::McCannFitSVRHitLatticeProxy( const McCannFitSVRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
McCannFitSVRHitLatticeProxy< H >::~McCannFitSVRHitLatticeProxy()
{
   delete m_McCannFitSVRHitLattice ;
}

//
// assignment operators
//
// const McCannFitSVRHitLatticeProxy& McCannFitSVRHitLatticeProxy::operator=( const McCannFitSVRHitLatticeProxy& iproxy )
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
McCannFitSVRHitLatticeProxy< H >::invalidateCache()
{
   delete m_McCannFitSVRHitLattice ;
   m_McCannFitSVRHitLattice = 0 ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename McCannFitSVRHitLatticeProxy< H >::value_type*
McCannFitSVRHitLatticeProxy< H >::faultHandler( const Record& iRecord,
						const DataKey& iKey )
{
   FAItem<DetectorConfiguration>                              iConfig   ;
   extract( iRecord.frame().record( Stream::kBaseGeometry ) , iConfig ) ;
   if( iConfig->useZDnotSI() )
   {
      throw DANoDataException< value_type >( iRecord.stream(),iKey ) ;
   }
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_McCannFitSVRHitLattice );

   // ---------- create the new object --------------------

   // Get the FATables to be linked and instantiate a new Lattice.
   FATable< TRHelixCandidateFit< H > > candHelices ;
   extract( iRecord, candHelices, "", iKey.production().value() ) ;

   // Get the McCannTrackFitter, and call event().
   FAItem< McCannTrackFitter > fitter;
   extract( iRecord, fitter, "", iKey.production().value() );

   if ( fitter->useSVRPhiHits() ) {
      try {
	 FATable< CalibratedSVRphiHit > svrHits ;
	 extract( iRecord, svrHits ) ;
	 
	 m_McCannFitSVRHitLattice =  new value_type( candHelices,
						     svrHits,
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
		   CalibratedHit::SVR )
	       {
		  if( m_McCannFitSVRHitLattice->connect(
		     ( value_type::LeftID )( *helixItr )->identifier(),
		     hitIter->calibratedHit()->identifier(),
		     *( ( TrackFitSVRHitLink* ) hitIter->linkData() ) ) == 0 )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "SVR Lattice connect failed." << endl ;
		  }
	       }
	    }
	 }
      }
      catch( NO_TABLE_EXCEPTION( CalibratedSVRphiHit )& aException )
      {
	 if( ! fitter->continueIfHitsMissing() ) {
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
   if( 0 == m_McCannFitSVRHitLattice ) {
      //return an empty lattice
      auto_ptr< FAPtrTable< CalibratedSVRphiHit > > svTable(
	 new FAPtrTable< CalibratedSVRphiHit > ) ;
      
      FATable< CalibratedSVRphiHit > svHits( &(*svTable) );
      
      m_McCannFitSVRHitLattice =  new value_type( candHelices,
						  svHits,
						  value_type::LNodeMulti ) ;
   }
   return ( m_McCannFitSVRHitLattice ) ;
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
//McCannFitSVRHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}


