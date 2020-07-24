// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitZDHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 15:35:14 EDT 1999
// $Id: ChisqFitZDHitLatticeProxy.cc,v 1.1 2003/04/17 17:00:46 bkh Exp $
//
// Revision history
//
// $Log: ChisqFitZDHitLatticeProxy.cc,v $
// Revision 1.1  2003/04/17 17:00:46  bkh
// Add ZD options & throw exceptions for wrong config
//
// Revision 1.5  2002/06/12 19:26:40  cleo3
// added missing typename
//
// Revision 1.4  2000/06/09 01:42:36  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.3  2000/02/28 23:17:04  lyon
// Added code to avoid exceptions if data not present and not wanted
//
// Revision 1.2  1999/07/17 20:28:32  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:45:23  wsun
// First submission.
//
// Revision 1.1.1.1  1999/06/16 02:48:28  wsun
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
#include <memory>

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "ChisqFitProducer/ChisqFitZDHitLatticeProxy.h"
#include "ChisqFitter/ChisqTrackFitter.h"

#include "FrameAccess/FAPtrTable.h"
#include "DAException/DAException.h"
#include "DAException/DANoDataException.h"
#include "DetectorConfiguration/DetectorConfiguration.h"


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
static const char* const kFacilityString = "ChisqFitProducer.ChisqFitZDHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqFitZDHitLatticeProxy.cc,v 1.1 2003/04/17 17:00:46 bkh Exp $";
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
ChisqFitZDHitLatticeProxy< H >::ChisqFitZDHitLatticeProxy()
   : m_ChisqFitZDHitLattice( 0 )
{
}

// ChisqFitZDHitLatticeProxy::ChisqFitZDHitLatticeProxy( const ChisqFitZDHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
ChisqFitZDHitLatticeProxy< H >::~ChisqFitZDHitLatticeProxy()
{
   delete m_ChisqFitZDHitLattice ;
}

//
// assignment operators
//
// const ChisqFitZDHitLatticeProxy& ChisqFitZDHitLatticeProxy::operator=( const ChisqFitZDHitLatticeProxy& iproxy )
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
ChisqFitZDHitLatticeProxy< H >::invalidateCache()
{
   delete m_ChisqFitZDHitLattice ;
   m_ChisqFitZDHitLattice = 0 ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename ChisqFitZDHitLatticeProxy< H >::value_type*
ChisqFitZDHitLatticeProxy< H >::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
   FAItem<DetectorConfiguration>                              iConfig   ;
   extract( iRecord.frame().record( Stream::kBaseGeometry ) , iConfig ) ;
   if( iConfig->useSInotZD() )
   {
      throw DANoDataException< value_type >( iRecord.stream(),iKey ) ;
   }
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_ChisqFitZDHitLattice );

   // Get the FATables to be linked and instantiate a new Lattice.
   FATable< TRHelixCandidateFit< H > > candHelices ;
   extract( iRecord, candHelices ) ;

   // Get the ChisqTrackFitter, which will have had its event()
   // function called.
   FAItem< ChisqTrackFitter > fitter;
   extract( iRecord, fitter );

   if ( fitter->useZDStereoUHits() ||
	fitter->useZDStereoVHits() ) {

     try {

       FATable< CalibratedZDHit > ZDHits ;
       extract( iRecord, ZDHits ) ;

       m_ChisqFitZDHitLattice =  new value_type( candHelices,
						 ZDHits,
						 value_type::LNodeMulti,
						 value_type::RNodeMulti ) ;
       
       // Get the vector of ChisqHelix pointers for this hypothesis.
       STL_VECTOR( ChisqHelix* )* chisqHelices = fitter->fittedHelices() ;

       // Link the fitted helices with their hits -- 
       //   the TRHelixCandidateFit and
       // the corresponding ChisqHelix objects have the same identifier.
       STL_VECTOR( ChisqHelix* )::iterator helixItr = chisqHelices->begin() ;
       STL_VECTOR( ChisqHelix* )::iterator helixEnd = chisqHelices->end() ;
       for( ; helixItr != helixEnd ; ++helixItr )
       {
	 STL_VECTOR( ChisqHelix::HitAndLinkData )::const_iterator hitIter =
	   ( *helixItr )->hitsOnTrack().begin() ;
	 STL_VECTOR( ChisqHelix::HitAndLinkData )::const_iterator last =
	   ( *helixItr )->hitsOnTrack().end() ;
	 
	 for( ; hitIter != last ; ++hitIter )
	 {
	   if( hitIter->calibratedHit()->deviceType() == CalibratedHit::ZD )
	   {
	     if( m_ChisqFitZDHitLattice->connect(
		 candHelices.find( ( *helixItr )->identifier() )->identifier(),
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
          catch ( NO_TABLE_EXCEPTION( CalibratedZDHit )& aException )  
     {
       if( ! fitter->continueIfHitsMissing() ) {        
	 // This rethrows the excpetion currently being handled.  
	 throw;
       }
     }
     catch ( DAExceptionBase& aException )
     {
       if( ! fitter->continueIfAnyException() ) { 
	 // This rethrows the excpetion currently being handled.  
	 throw;
       }
     }
   }
   
   if ( 0 == m_ChisqFitZDHitLattice ) {
     // return an empty lattice
     auto_ptr< FAPtrTable< CalibratedZDHit > > ZDTable(
			new FAPtrTable< CalibratedZDHit > ) ;

     FATable< CalibratedZDHit > ZDHits( &(*ZDTable) ); 
     
     m_ChisqFitZDHitLattice =  new value_type( candHelices,   
					       ZDHits, 
					       value_type::LNodeMulti ) ;
   }
   
   
   return ( m_ChisqFitZDHitLattice ) ;
}

//
// const member functions
//

//
// static member functions
//


