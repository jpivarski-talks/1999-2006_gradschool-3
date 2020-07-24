// -*- C++ -*-
//
// Package:     <PivarskiProd>
// Module:      PivarskiFitSVZHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon May 10 23:49:23 EDT 1999
// $Id: PivarskiFitSVZHitLatticeProxy.cc,v 1.7 2003/04/17 17:05:13 bkh Exp $
//
// Revision history
//
// $Log: PivarskiFitSVZHitLatticeProxy.cc,v $
// Revision 1.7  2003/04/17 17:05:13  bkh
// Add ZD Lattice Proxy & throw exceptions if wrong config
//
// Revision 1.6  2002/06/17 20:01:01  cleo3
// added missing typename
//
// Revision 1.5  2001/03/27 06:43:05  wsun
// Extract Pivarski objects with same production tag as PivarskiProd.
//
// Revision 1.4  2000/06/09 00:57:39  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.3  2000/03/02 08:57:28  wsun
// Cast helix identifier to correct type instead of using find().
//
// Revision 1.2  2000/02/26 22:37:34  cdj
// LatticeProxies now use PivarskiProd's running options, e.g. catch exceptions if requested
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from PivarskiFilter
//
// Revision 1.3  1999/07/15 23:01:35  cdj
// now compiles with CC optimization on
//
// Revision 1.2  1999/06/18 19:27:50  wsun
// More improvements to low momentum fitting.
//
// Revision 1.1  1999/05/14 01:12:12  wsun
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

#include "PivarskiProd/PivarskiFitSVZHitLatticeProxy.h"
#include "PivarskiFilter/PivarskiTrackFitter.h"

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
static const char* const kFacilityString = "PivarskiFitSVZHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: PivarskiFitSVZHitLatticeProxy.cc,v 1.7 2003/04/17 17:05:13 bkh Exp $";
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
PivarskiFitSVZHitLatticeProxy< H >::PivarskiFitSVZHitLatticeProxy()
   : m_PivarskiFitSVZHitLattice( 0 )
{
}

// PivarskiFitSVZHitLatticeProxy::PivarskiFitSVZHitLatticeProxy( const PivarskiFitSVZHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
PivarskiFitSVZHitLatticeProxy< H >::~PivarskiFitSVZHitLatticeProxy()
{
   delete m_PivarskiFitSVZHitLattice ;
}

//
// assignment operators
//
// const PivarskiFitSVZHitLatticeProxy& PivarskiFitSVZHitLatticeProxy::operator=( const PivarskiFitSVZHitLatticeProxy& iproxy )
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
PivarskiFitSVZHitLatticeProxy< H >::invalidateCache()
{
   delete m_PivarskiFitSVZHitLattice ;
   m_PivarskiFitSVZHitLattice = 0 ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename PivarskiFitSVZHitLatticeProxy< H >::value_type*
PivarskiFitSVZHitLatticeProxy< H >::faultHandler( const Record& iRecord,
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
   assert( 0 == m_PivarskiFitSVZHitLattice );

   // ---------- create the new object --------------------

   // Get the FATables to be linked and instantiate a new Lattice.
   FATable< TRHelixCandidateFit< H > > candHelices ;
   extract( iRecord, candHelices, "", iKey.production().value() ) ;


   // Get the PivarskiTrackFitter, and call event().
   FAItem< PivarskiTrackFitter > fitter;
   extract( iRecord, fitter, "", iKey.production().value() );

   if( fitter->useSVZHits() ) {
      try {
	 FATable< CalibratedSVZHit > svzHits ;
	 extract( iRecord, svzHits ) ;
	 
	 m_PivarskiFitSVZHitLattice =  new value_type( candHelices,
						     svzHits,
						     value_type::LNodeMulti ) ;
	 // Fit the tracks under the hypothesis H.
	 fitter->event( ( Frame& ) iRecord.frame(),
			( TrackFitter::HypoType ) H ) ;
	 
	 // Get the vector of PivarskiHelix pointers for this hypothesis.
	 STL_VECTOR( PivarskiHelix* )* kalmanHelices =
	    fitter->fittedHelices( ( TrackFitter::HypoType ) H ) ;
	 
	 // Link the fitted helices with their hits -- the TRHelixCandidateFit
	 // and the corresponding PivarskiHelix objects have the same identifier.
	 STL_VECTOR( PivarskiHelix* )::iterator helixItr =kalmanHelices->begin();
	 STL_VECTOR( PivarskiHelix* )::iterator helixEnd = kalmanHelices->end() ;
	 
	 for( ; helixItr != helixEnd ; ++helixItr )
	 {
	    STL_VECTOR( PivarskiHelix::HitAndLinkData )::const_iterator hitIter =
	       ( *helixItr )->hitsOnTrack().begin() ;
	    STL_VECTOR( PivarskiHelix::HitAndLinkData )::const_iterator last =
	       ( *helixItr )->hitsOnTrack().end() ;
	    
	    for( ; hitIter != last ; ++hitIter )
	    {
	       if( hitIter->calibratedHit()->deviceType() ==
		   CalibratedHit::SVZ )
	       {
		  if( m_PivarskiFitSVZHitLattice->connect(
		     ( value_type::LeftID )( *helixItr )->identifier(),
		     hitIter->calibratedHit()->identifier(),
		     *( ( TrackFitSVZHitLink* ) hitIter->linkData() ) ) == 0 )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "SVZ Lattice connect failed." << endl ;
		  }
	       }
	    }
	 }      
      }
      catch( NO_TABLE_EXCEPTION( CalibratedSVZHit )& aException )
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
   if( 0 == m_PivarskiFitSVZHitLattice ) {
      //return an empty lattice
      auto_ptr< FAPtrTable< CalibratedSVZHit > > svTable(
	 new FAPtrTable< CalibratedSVZHit > ) ;
      
      FATable< CalibratedSVZHit > svHits( &(*svTable) );
      
      m_PivarskiFitSVZHitLattice =  new value_type( candHelices,
						  svHits,
						  value_type::LNodeMulti ) ;
   }
   return ( m_PivarskiFitSVZHitLattice ) ;
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
//PivarskiFitSVZHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}


