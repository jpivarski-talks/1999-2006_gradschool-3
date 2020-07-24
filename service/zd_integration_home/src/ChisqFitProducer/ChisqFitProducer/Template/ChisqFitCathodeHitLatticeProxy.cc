// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitCathodeHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Fri Sep 10 19:10:46 EDT 1999
// $Id: ChisqFitCathodeHitLatticeProxy.cc,v 1.4 2002/06/12 19:26:40 cleo3 Exp $
//
// Revision history
//
// $Log: ChisqFitCathodeHitLatticeProxy.cc,v $
// Revision 1.4  2002/06/12 19:26:40  cleo3
// added missing typename
//
// Revision 1.3  2000/06/09 01:42:36  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.2  2000/02/28 23:17:04  lyon
// Added code to avoid exceptions if data not present and not wanted
//
// Revision 1.1  1999/09/11 19:34:33  wsun
// Added DR cathodes.
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

#include "ChisqFitProducer/ChisqFitCathodeHitLatticeProxy.h"
#include "ChisqFitter/ChisqTrackFitter.h"

#include "FrameAccess/FAPtrTable.h"
#include "DAException/DAException.h"
#include "DAException/DANoDataException.h"


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
static const char* const kFacilityString = "ChisqFitProducer.ChisqFitCathodeHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqFitCathodeHitLatticeProxy.cc,v 1.4 2002/06/12 19:26:40 cleo3 Exp $";
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
ChisqFitCathodeHitLatticeProxy< H >::ChisqFitCathodeHitLatticeProxy()
   : m_ChisqFitCathodeHitLattice( 0 )
{
}

// ChisqFitCathodeHitLatticeProxy::ChisqFitCathodeHitLatticeProxy( const ChisqFitCathodeHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
ChisqFitCathodeHitLatticeProxy< H >::~ChisqFitCathodeHitLatticeProxy()
{
   delete m_ChisqFitCathodeHitLattice ;
}

//
// assignment operators
//
// const ChisqFitCathodeHitLatticeProxy& ChisqFitCathodeHitLatticeProxy::operator=( const ChisqFitCathodeHitLatticeProxy& iproxy )
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
ChisqFitCathodeHitLatticeProxy< H >::invalidateCache()
{
   delete m_ChisqFitCathodeHitLattice ;
   m_ChisqFitCathodeHitLattice = 0 ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename ChisqFitCathodeHitLatticeProxy< H >::value_type*
ChisqFitCathodeHitLatticeProxy< H >::faultHandler( const Record& iRecord,
						   const DataKey& iKey )
{
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_ChisqFitCathodeHitLattice );

   // Get the FATables to be linked and instantiate a new Lattice.
   FATable< TRHelixCandidateFit< H > > candHelices ;
   extract( iRecord, candHelices ) ;

   // Get the ChisqTrackFitter, which will have had its event()
   // function called.
   FAItem< ChisqTrackFitter > fitter;
   extract( iRecord, fitter );

   if ( fitter->useDRCathodeHits() )
   {

     try 
     {

       FATable< CalibratedCathodeHit > cathodeHits ;
       extract( iRecord, cathodeHits ) ;
       
       m_ChisqFitCathodeHitLattice =  new value_type( candHelices,
						      cathodeHits,
						      value_type::LNodeMulti );
       

       // Get the vector of ChisqHelix pointers for this hypothesis.
       STL_VECTOR( ChisqHelix* )* chisqHelices = fitter->fittedHelices() ;

       // Link the fitted helices with their hits -- 
       // the TRHelixCandidateFit and
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
	   if( hitIter->calibratedHit()->deviceType() == 
	       CalibratedHit::Cathode )
	   {
	     if( m_ChisqFitCathodeHitLattice->connect(
	        candHelices.find( ( *helixItr )->identifier() )->identifier(),
		hitIter->calibratedHit()->identifier(),
		*( ( TrackFitCathodeHitLink* ) hitIter->linkData() ) ) == 0 )
	     {
               report( EMERGENCY, kFacilityString )
		 << "Cathode Lattice connect failed." << endl ;
	     }
	   }
	 }
       }
     }

     catch ( NO_TABLE_EXCEPTION( CalibratedCathodeHit )& aException )  
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
   
   if ( 0 == m_ChisqFitCathodeHitLattice ) {
     // return an empty lattice
     auto_ptr< FAPtrTable< CalibratedCathodeHit > > cathodeTable(
			new FAPtrTable< CalibratedCathodeHit > ) ;

     FATable< CalibratedCathodeHit > cathodeHits( &(*cathodeTable) ); 
     
     m_ChisqFitCathodeHitLattice =  new value_type( candHelices,   
						    cathodeHits,
						    value_type::LNodeMulti ) ;
   }
       
   return ( m_ChisqFitCathodeHitLattice ) ;
}

//
// const member functions
//

//
// static member functions
//
