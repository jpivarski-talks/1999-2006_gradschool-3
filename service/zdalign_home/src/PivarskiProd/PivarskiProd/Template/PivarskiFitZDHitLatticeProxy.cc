// -*- C++ -*-
//
// Package:     <PivarskiProd>
// Module:      PivarskiFitZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon May 10 23:49:23 EDT 1999
// $Id: PivarskiFitZDHitLatticeProxy.cc,v 1.2 2003/07/25 23:34:53 mccann Exp $
//
// Revision history
//
// $Log: PivarskiFitZDHitLatticeProxy.cc,v $
// Revision 1.2  2003/07/25 23:34:53  mccann
// Added the ZD to track fitting
//
// Revision 1.1  2003/04/17 17:05:13  bkh
// Add ZD Lattice Proxy & throw exceptions if wrong config
//
// Revision 1.6  2002/06/17 20:01:00  cleo3
// added missing typename
//
// Revision 1.5  2001/03/27 06:43:03  wsun
// Extract Pivarski objects with same production tag as PivarskiProd.
//
// Revision 1.4  2000/06/09 00:57:38  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.3  2000/03/02 08:57:27  wsun
// Cast helix identifier to correct type instead of using find().
//
// Revision 1.2  2000/02/26 22:37:33  cdj
// LatticeProxies now use PivarskiProd's running options, e.g. catch exceptions if requested
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from PivarskiFilter
//
// Revision 1.3  1999/07/15 23:01:34  cdj
// now compiles with CC optimization on
//
// Revision 1.2  1999/06/18 19:27:48  wsun
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

#include "PivarskiProd/PivarskiFitZDHitLatticeProxy.h"
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
static const char* const kFacilityString = "PivarskiFitZDHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: PivarskiFitZDHitLatticeProxy.cc,v 1.2 2003/07/25 23:34:53 mccann Exp $";
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
PivarskiFitZDHitLatticeProxy< H >::PivarskiFitZDHitLatticeProxy()
   : m_PivarskiFitZDHitLattice( 0 )
{
}

// PivarskiFitZDHitLatticeProxy::PivarskiFitZDHitLatticeProxy( const PivarskiFitZDHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
PivarskiFitZDHitLatticeProxy< H >::~PivarskiFitZDHitLatticeProxy()
{
   delete m_PivarskiFitZDHitLattice ;
}

//
// assignment operators
//
// const PivarskiFitZDHitLatticeProxy& PivarskiFitZDHitLatticeProxy::operator=( const PivarskiFitZDHitLatticeProxy& iproxy )
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
PivarskiFitZDHitLatticeProxy< H >::invalidateCache()
{
   delete m_PivarskiFitZDHitLattice ;
   m_PivarskiFitZDHitLattice = 0 ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename PivarskiFitZDHitLatticeProxy< H >::value_type*
PivarskiFitZDHitLatticeProxy< H >::faultHandler( const Record& iRecord,
					       const DataKey& iKey )
{
   report(DEBUG, kFacilityString) << "enter PivarskiFitZDHitLatticeProxy< H >" << endl;

   FAItem<DetectorConfiguration>                              iConfig   ;
   extract( iRecord.frame().record( Stream::kBaseGeometry ) , iConfig ) ;
   if( iConfig->useSInotZD() )
   {
      report(DEBUG, kFacilityString) << "exit PivarskiFitZDHitLatticeProxy< H > 1" << endl;
      throw DANoDataException< value_type >( iRecord.stream(),iKey ) ;
   }
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_PivarskiFitZDHitLattice );

   // ---------- create the new object --------------------

   // Get the FATables to be linked and instantiate a new Lattice.
   FATable< TRHelixCandidateFit< H > > candHelices ;
   extract( iRecord, candHelices, "", iKey.production().value() ) ;

   report(DEBUG, kFacilityString) << "got candHelices" << endl;

   // Get the PivarskiTrackFitter, which will have had its event()
   // function called.
   FAItem< PivarskiTrackFitter > fitter;
   extract( iRecord, fitter, "", iKey.production().value() );

   report(DEBUG, kFacilityString) << "got fitter" << endl;

   if( fitter->useZDStereoUHits() ||
       fitter->useZDStereoVHits() ) {
      try {
	 FATable< CalibratedZDHit > zdHits ;
	 extract( iRecord, zdHits ) ;

	 report(DEBUG, kFacilityString) << "got zdHits" << endl;

	 m_PivarskiFitZDHitLattice =  new value_type( candHelices,
						    zdHits,
						    value_type::LNodeMulti,
						    value_type::RNodeMulti ) ;

	 report(DEBUG, kFacilityString) << "made lattice" << endl;

	 // Fit the tracks under the hypothesis H.
	 fitter->event( ( Frame& ) iRecord.frame(),
			( TrackFitter::HypoType ) H ) ;

	 report(DEBUG, kFacilityString) << "fitted (!?) tracks" << endl;

	 // Get the vector of PivarskiHelix pointers for this hypothesis.
	 STL_VECTOR( PivarskiHelix* )* kalmanHelices =
	    fitter->fittedHelices( ( TrackFitter::HypoType ) H ) ;

	 report(DEBUG, kFacilityString) << "got " << kalmanHelices->size() << " kalmanHelices" << endl;

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
	    
	    report(DEBUG, kFacilityString) << "  track has " << (*helixItr)->hitsOnTrack().size() << " hits" << endl;

	    for( ; hitIter != last ; ++hitIter )
	    {
	       if( hitIter->calibratedHit()->deviceType() ==
		   CalibratedHit::ZD )
	       {
		  report(DEBUG, kFacilityString) << "    found a ZD hit" << endl;

		  if( m_PivarskiFitZDHitLattice->connect(
		     ( value_type::LeftID )( *helixItr )->identifier(),
		     hitIter->calibratedHit()->identifier(),
		     *( ( TrackFitZDHitLink* ) hitIter->linkData() ) ) == 0 )
		  {
		     report( EMERGENCY, kFacilityString )
			<< "ZD Lattice connect failed." << endl ;
		  }
	       }
	    }
	 }
      }
      catch( NO_TABLE_EXCEPTION( CalibratedZDHit )& aException )
      {
	 if( ! fitter->continueIfHitsMissing() ) {
	    // This rethrows the excpetion currently being handled.
	    report(DEBUG, kFacilityString) << "exit PivarskiFitZDHitLatticeProxy< H > 2" << endl;
	    throw;
	 }
      }
      catch( DAExceptionBase& aException )
      {
	 if( ! fitter->continueIfAnyException() ) {
	    // This rethrows the excpetion currently being handled.
	    report(DEBUG, kFacilityString) << "exit PivarskiFitZDHitLatticeProxy< H > 3" << endl;
	    throw;
	 }
      }
   }

   if( 0 == m_PivarskiFitZDHitLattice ) {
      report(DEBUG, kFacilityString) << "returning an empty lattice" << endl;

      // return an empty lattice
      auto_ptr< FAPtrTable< CalibratedZDHit > > zdTable(
	 new FAPtrTable< CalibratedZDHit > ) ;

      FATable< CalibratedZDHit > zdHits( &(*zdTable) );

      m_PivarskiFitZDHitLattice =  new value_type( candHelices,
						 zdHits,
						 value_type::LNodeMulti ) ;

   }

   report(DEBUG, kFacilityString) << "end PivarskiFitZDHitLatticeProxy< H >" << endl;
   return ( m_PivarskiFitZDHitLattice ) ;
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
//PivarskiFitZDHitLatticeProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}


