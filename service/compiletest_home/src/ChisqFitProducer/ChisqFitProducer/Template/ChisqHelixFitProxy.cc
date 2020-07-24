// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqHelixFitProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 15:15:54 EDT 1999
// $Id: ChisqHelixFitProxy.cc,v 1.3 2002/06/12 19:26:41 cleo3 Exp $
//
// Revision history
//
// $Log: ChisqHelixFitProxy.cc,v $
// Revision 1.3  2002/06/12 19:26:41  cleo3
// added missing typename
//
// Revision 1.2  1999/07/17 20:28:34  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:45:26  wsun
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

// user include files
#include "Experiment/report.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"

#include "ChisqFitProducer/ChisqHelixFitProxy.h"
#include "ChisqFitter/ChisqTrackFitter.h"

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
static const char* const kFacilityString = "ChisqFitProducer.ChisqHelixFitProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqHelixFitProxy.cc,v 1.3 2002/06/12 19:26:41 cleo3 Exp $";
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
ChisqHelixFitProxy< H >::ChisqHelixFitProxy()
   : m_ptrTable( new value_type )
{
   if ( 0 == m_ptrTable ) {
      report( EMERGENCY, kFacilityString )
         << "Unable to allocate memory"
         << endl ;
      ::exit( 1 ) ;
   }
}

// ChisqHelixFitProxy::ChisqHelixFitProxy( const ChisqHelixFitProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
ChisqHelixFitProxy< H >::~ChisqHelixFitProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
}

//
// assignment operators
//
// const ChisqHelixFitProxy& ChisqHelixFitProxy::operator=( const ChisqHelixFitProxy& iproxy )
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
ChisqHelixFitProxy< H >::invalidateCache()
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
const typename ChisqHelixFitProxy< H >::value_type*
ChisqHelixFitProxy< H >::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // Get the ChisqTrackFitter, and call event().
   FAItem< ChisqTrackFitter > fitter;
   extract( iRecord, fitter );

   // Get the vector of ChisqHelix pointers for this hypothesis.
   STL_VECTOR( ChisqHelix* )* chisqHelices = fitter->fittedHelices() ;

   // Loop over all the ChisqHelix objects and transfer them to m_ptrTable.
   STL_VECTOR( ChisqHelix* )::iterator helixItr = chisqHelices->begin() ;
   STL_VECTOR( ChisqHelix* )::iterator helixEnd = chisqHelices->end() ;

   for( ; helixItr != helixEnd ; ++helixItr )
   {
      // The TRHelixCandidateFit will have the same identifier as the
      // ChisqHelix.
      m_ptrTable->insert( new TRHelixCandidateFit< H >(
	 ( *helixItr )->identifier(), **helixItr ) ) ;
   }

   return ( m_ptrTable ) ;
}

//
// const member functions
//

//
// static member functions
//



