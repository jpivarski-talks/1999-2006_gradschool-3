// -*- C++ -*-
//
// Package:     <PivarskiProd>
// Module:      PivarskiHelixFitProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Mon May 10 22:49:14 EDT 1999
// $Id: PivarskiHelixFitProxy.cc,v 1.3 2002/06/17 20:01:01 cleo3 Exp $
//
// Revision history
//
// $Log: PivarskiHelixFitProxy.cc,v $
// Revision 1.3  2002/06/17 20:01:01  cleo3
// added missing typename
//
// Revision 1.2  2001/03/27 06:43:05  wsun
// Extract Pivarski objects with same production tag as PivarskiProd.
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from PivarskiFilter
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

#include "PivarskiProd/PivarskiHelixFitProxy.h"
#include "PivarskiFilter/PivarskiTrackFitter.h"


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
static const char* const kFacilityString = "PivarskiHelixFitProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: PivarskiHelixFitProxy.cc,v 1.3 2002/06/17 20:01:01 cleo3 Exp $";
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
PivarskiHelixFitProxy< H >::PivarskiHelixFitProxy()
   : m_ptrTable( new value_type )
{
   if ( 0 == m_ptrTable ) {
      report( EMERGENCY, kFacilityString )
         << "Unable to allocate memory"
         << endl ;
      ::exit( 1 ) ;
   }
}

// PivarskiHelixFitProxy::PivarskiHelixFitProxy( const PivarskiHelixFitProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
PivarskiHelixFitProxy< H >::~PivarskiHelixFitProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const PivarskiHelixFitProxy& PivarskiHelixFitProxy::operator=( const PivarskiHelixFitProxy& iproxy )
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
PivarskiHelixFitProxy< H >::invalidateCache()
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
const typename PivarskiHelixFitProxy< H >::value_type*
PivarskiHelixFitProxy< H >::faultHandler( const Record& iRecord,
					const DataKey& iKey )
{
// ---------- sample implementation ---------------------
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // ---------- fill FAPtrTable with new items ----------------

   // Get the PivarskiTrackFitter, and call event().
   FAItem< PivarskiTrackFitter > fitter;
   extract( iRecord, fitter, "", iKey.production().value() );

   // Fit the tracks under the hypothesis H.
   fitter->event( ( Frame& ) iRecord.frame(), ( TrackFitter::HypoType ) H ) ;

   // Get the vector of PivarskiHelix pointers for this hypothesis.
   STL_VECTOR( PivarskiHelix* )* kalmanHelices =
      fitter->fittedHelices( ( TrackFitter::HypoType ) H ) ;

   // Loop over all the PivarskiHelix objects and transfer them to m_ptrTable.
   STL_VECTOR( PivarskiHelix* )::iterator helixItr = kalmanHelices->begin() ;
   STL_VECTOR( PivarskiHelix* )::iterator helixEnd = kalmanHelices->end() ;

   for( ; helixItr != helixEnd ; ++helixItr )
   {
      // The TRHelixCandidateFit will have the same identifier as the
      // PivarskiHelix.
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

