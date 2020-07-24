// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitDRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 15:35:14 EDT 1999
// $Id: ChisqFitDRHitLatticeProxy.cc,v 1.5 2002/06/12 19:26:40 cleo3 Exp $
//
// Revision history
//
// $Log: ChisqFitDRHitLatticeProxy.cc,v $
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

#include "ChisqFitProducer/ChisqFitDRHitLatticeProxy.h"
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
static const char* const kFacilityString = "ChisqFitProducer.ChisqFitDRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqFitDRHitLatticeProxy.cc,v 1.5 2002/06/12 19:26:40 cleo3 Exp $";
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
ChisqFitDRHitLatticeProxy< H >::ChisqFitDRHitLatticeProxy()
   : m_ChisqFitDRHitLattice( 0 )
{
}

// ChisqFitDRHitLatticeProxy::ChisqFitDRHitLatticeProxy( const ChisqFitDRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
ChisqFitDRHitLatticeProxy< H >::~ChisqFitDRHitLatticeProxy()
{
   delete m_ChisqFitDRHitLattice ;
}

//
// assignment operators
//
// const ChisqFitDRHitLatticeProxy& ChisqFitDRHitLatticeProxy::operator=( const ChisqFitDRHitLatticeProxy& iproxy )
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
ChisqFitDRHitLatticeProxy< H >::invalidateCache()
{
   delete m_ChisqFitDRHitLattice ;
   m_ChisqFitDRHitLattice = 0 ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename ChisqFitDRHitLatticeProxy< H >::value_type*
ChisqFitDRHitLatticeProxy< H >::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
   // be assertive: there should be no cached data at this point, else
   // it's a programming error
   assert( 0 == m_ChisqFitDRHitLattice );

   // Get the FATables to be linked and instantiate a new Lattice.
   FATable< TRHelixCandidateFit< H > > candHelices ;
   extract( iRecord, candHelices ) ;

   // Get the ChisqTrackFitter, which will have had its event()
   // function called.
   FAItem< ChisqTrackFitter > fitter;
   extract( iRecord, fitter );

   if ( fitter->useDRAxialHits() ||
	fitter->useDRStereoUHits() ||
	fitter->useDRStereoVHits() ) {

     try {

       FATable< CalibratedDRHit > drHits ;
       extract( iRecord, drHits ) ;

       m_ChisqFitDRHitLattice =  new value_type( candHelices,
						 drHits,
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
	   if( hitIter->calibratedHit()->deviceType() == CalibratedHit::DR )
	   {
	     if( m_ChisqFitDRHitLattice->connect(
		 candHelices.find( ( *helixItr )->identifier() )->identifier(),
		 hitIter->calibratedHit()->identifier(),
		 *( ( TrackFitDRHitLink* ) hitIter->linkData() ) ) == 0 )
	     {
	       report( EMERGENCY, kFacilityString )
		 << "DR Lattice connect failed." << endl ;
	     }
	   }
	 }
       }
     }
          catch ( NO_TABLE_EXCEPTION( CalibratedDRHit )& aException )  
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
   
   if ( 0 == m_ChisqFitDRHitLattice ) {
     // return an empty lattice
     auto_ptr< FAPtrTable< CalibratedDRHit > > drTable(
			new FAPtrTable< CalibratedDRHit > ) ;

     FATable< CalibratedDRHit > drHits( &(*drTable) ); 
     
     m_ChisqFitDRHitLattice =  new value_type( candHelices,   
					       drHits, 
					       value_type::LNodeMulti ) ;
   }
   
   
   return ( m_ChisqFitDRHitLattice ) ;
}

//
// const member functions
//

//
// static member functions
//


