// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitSVRHitLatticeProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 15:54:38 EDT 1999
// $Id: ChisqFitSVRHitLatticeProxy.cc,v 1.6 2003/04/17 17:00:44 bkh Exp $
//
// Revision history
//
// $Log: ChisqFitSVRHitLatticeProxy.cc,v $
// Revision 1.6  2003/04/17 17:00:44  bkh
// Add ZD options & throw exceptions for wrong config
//
// Revision 1.5  2002/06/12 19:26:40  cleo3
// added missing typename
//
// Revision 1.4  2000/06/09 01:42:37  wsun
// Updated to new HIFitHelix::HitAndLinkData.
//
// Revision 1.3  2000/02/28 23:17:04  lyon
// Added code to avoid exceptions if data not present and not wanted
//
// Revision 1.2  1999/07/17 20:28:33  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:45:24  wsun
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
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"

#include "ChisqFitProducer/ChisqFitSVRHitLatticeProxy.h"
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
static const char* const kFacilityString = "ChisqFitProducer.ChisqFitSVRHitLatticeProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqFitSVRHitLatticeProxy.cc,v 1.6 2003/04/17 17:00:44 bkh Exp $";
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
ChisqFitSVRHitLatticeProxy< H >::ChisqFitSVRHitLatticeProxy()
   : m_ChisqFitSVRHitLattice( 0 )
{
}

// ChisqFitSVRHitLatticeProxy::ChisqFitSVRHitLatticeProxy( const ChisqFitSVRHitLatticeProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
ChisqFitSVRHitLatticeProxy< H >::~ChisqFitSVRHitLatticeProxy()
{
   delete m_ChisqFitSVRHitLattice ;
}

//
// assignment operators
//
// const ChisqFitSVRHitLatticeProxy& ChisqFitSVRHitLatticeProxy::operator=( const ChisqFitSVRHitLatticeProxy& iproxy )
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
ChisqFitSVRHitLatticeProxy< H >::invalidateCache()
{
   delete m_ChisqFitSVRHitLattice ;
   m_ChisqFitSVRHitLattice = 0 ;
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename ChisqFitSVRHitLatticeProxy< H >::value_type*
ChisqFitSVRHitLatticeProxy< H >::faultHandler( const Record& iRecord,
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
   assert( 0 == m_ChisqFitSVRHitLattice );

   // Get the FATables to be linked and instantiate a new Lattice.
   FATable< TRHelixCandidateFit< H > > candHelices ;
   extract( iRecord, candHelices ) ;

   // Get the ChisqTrackFitter, which will have had its event()
   // function called.
   FAItem< ChisqTrackFitter > fitter;
   extract( iRecord, fitter );

   if ( fitter->useSVRPhiHits() ) 
   {
     try {
       FATable< CalibratedSVRphiHit > svrHits ;
       extract( iRecord, svrHits ) ;

       m_ChisqFitSVRHitLattice =  new value_type( candHelices,
						  svrHits,
						  value_type::LNodeMulti ) ;

       
       // Get the vector of ChisqHelix pointers for this hypothesis.
       STL_VECTOR( ChisqHelix* )* chisqHelices = fitter->fittedHelices() ;
       
       // Link the fitted helices with their hits -- the TRHelixCandidateFit and
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
	   if( hitIter->calibratedHit()->deviceType() == CalibratedHit::SVR )
	   {
	     if( m_ChisqFitSVRHitLattice->connect(
               candHelices.find( ( *helixItr )->identifier() )->identifier(),
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
     catch ( NO_TABLE_EXCEPTION( CalibratedSVRphiHit )& aException )  
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
   
   if ( 0 == m_ChisqFitSVRHitLattice ) {
     // return an empty lattice
     auto_ptr< FAPtrTable< CalibratedSVRphiHit > > svTable(
			new FAPtrTable< CalibratedSVRphiHit > ) ;

     FATable< CalibratedSVRphiHit > svHits( &(*svTable) ); 
     
     m_ChisqFitSVRHitLattice =  new value_type( candHelices,   
						 svHits, 
						 value_type::LNodeMulti ) ;
   }


   return ( m_ChisqFitSVRHitLattice ) ;
}

//
// const member functions
//

//
// static member functions
//
