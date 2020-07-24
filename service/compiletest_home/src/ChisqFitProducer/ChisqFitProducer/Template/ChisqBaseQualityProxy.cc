// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqBaseQualityProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 16:25:31 EDT 1999
// $Id: ChisqBaseQualityProxy.cc,v 1.4 2002/06/12 19:26:39 cleo3 Exp $
//
// Revision history
//
// $Log: ChisqBaseQualityProxy.cc,v $
// Revision 1.4  2002/06/12 19:26:39  cleo3
// added missing typename
//
// Revision 1.3  1999/11/30 04:27:42  wsun
// Use FAPtrTable::eraseWithoutDeleteAll() instead of eraseAll().
//
// Revision 1.2  1999/07/17 20:28:27  cdj
// now compiles when CC optimization is on
//
// Revision 1.1  1999/06/20 22:45:22  wsun
// First submission.
//
// Revision 1.1.1.1  1999/06/16 02:48:29  wsun
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

#include "ChisqFitProducer/ChisqBaseQualityProxy.h"

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
static const char* const kFacilityString = "ChisqFitProducer.ChisqBaseQualityProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqBaseQualityProxy.cc,v 1.4 2002/06/12 19:26:39 cleo3 Exp $";
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
ChisqBaseQualityProxy< H >::ChisqBaseQualityProxy()
   : m_ptrTable( new value_type )
{
   if ( 0 == m_ptrTable ) {
      report( EMERGENCY, kFacilityString )
         << "Unable to allocate memory"
         << endl ;
      ::exit( 1 ) ;
   }
}

// ChisqBaseQualityProxy::ChisqBaseQualityProxy( const ChisqBaseQualityProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
ChisqBaseQualityProxy< H >::~ChisqBaseQualityProxy()
{
   // call eraseWithoutDeleteAll() to prevent FATable dtor from deleting its
   // contents, which it doesn't own.
   (*m_ptrTable).eraseWithoutDeleteAll();
   delete m_ptrTable ;
}

//
// assignment operators
//
// const ChisqBaseQualityProxy& ChisqBaseQualityProxy::operator=( const ChisqBaseQualityProxy& iproxy )
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
ChisqBaseQualityProxy< H >::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      // call eraseWithoutDeleteAll() instead of eraseAll() because the FATable
      // doesn't own its contents.
      (*m_ptrTable).eraseWithoutDeleteAll() ;
   }
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const typename ChisqBaseQualityProxy< H >::value_type*
ChisqBaseQualityProxy< H >::faultHandler( const Record& iRecord,
					  const DataKey& iKey )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // ---------- fill FAPtrTable with new items ----------------

   FATable< ChisqCandidateQuality< H > > chisqQualities;
   extract( iRecord, chisqQualities );

   if( chisqQualities.valid() )
   {
      FATableItr< ChisqCandidateQuality< H > > chisqFinished(
	 chisqQualities.end() );
      for ( FATableItr< ChisqCandidateQuality< H > > chisqQuality(
	 chisqQualities.begin() ) ;
            chisqQuality != chisqFinished ;
            ++chisqQuality )
      {
         // we don't actually create anything new
         ( *m_ptrTable ).insert(
	    ( value_type::value_type* )( &*chisqQuality ) ) ;
      }
      return ( m_ptrTable );
   }

   report( WARNING, kFacilityString )
      << "no fit track quality objects found" << endl;
   return ( 0 ) ;
}

//
// const member functions
//

//
// static member functions
//


