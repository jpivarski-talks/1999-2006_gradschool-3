// -*- C++ -*-
//
// Package:     <McCannProd>
// Module:      McCannBaseQualityProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 01:58:33 EDT 1999
// $Id: McCannBaseQualityProxy.cc,v 1.4 2001/03/27 06:43:01 wsun Exp $
//
// Revision history
//
// $Log: McCannBaseQualityProxy.cc,v $
// Revision 1.4  2001/03/27 06:43:01  wsun
// Extract McCann objects with same production tag as McCannProd.
//
// Revision 1.3  1999/11/30 04:32:49  wsun
// Use FAPtrTable::eraseWithoutDeleteAll() instead of eraseAll().
//
// Revision 1.2  1999/11/30 04:31:17  wsun
// Improved tag message.
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from McCannFilter
//
// Revision 1.2  1999/07/15 23:01:32  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:12:09  wsun
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

#include "McCannProd/McCannBaseQualityProxy.h"
#include "McCannFilter/McCannCandidateQuality.h"
//#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#if 0
#include "AnalysisExplorer/NavTrack.h" 
#endif

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
static const char* const kFacilityString = "McCannBaseQualityProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: McCannBaseQualityProxy.cc,v 1.4 2001/03/27 06:43:01 wsun Exp $";
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
McCannBaseQualityProxy< H >::McCannBaseQualityProxy()
   : m_ptrTable( new value_type )
{
   if ( 0 == m_ptrTable ) {
      report( EMERGENCY, kFacilityString )
         << "Unable to allocate memory"
         << endl ;
      ::exit( 1 ) ;
   }

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &McCannBaseQualityProxy::boundMethod, Stream::kBeginRun );
}

// McCannBaseQualityProxy::McCannBaseQualityProxy( const McCannBaseQualityProxy& iproxy )
// {
//   *this = iproxy;
// }

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
McCannBaseQualityProxy< H >::~McCannBaseQualityProxy()
{
   // call eraseWithoutDeleteAll() to prevent FAPtrTable dtor from deleting its
   // contents, which it doesn't own.
   (*m_ptrTable).eraseWithoutDeleteAll();
   delete m_ptrTable ;
}

//
// assignment operators
//
// const McCannBaseQualityProxy& McCannBaseQualityProxy::operator=( const McCannBaseQualityProxy& iproxy )
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
McCannBaseQualityProxy< H >::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      // call eraseWithoutDeleteAll() instead of eraseAll() because the
      // FAPtrTable doesn't own its contents.
      (*m_ptrTable).eraseWithoutDeleteAll() ;
   }
}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
const McCannBaseQualityProxy< H >::value_type*
McCannBaseQualityProxy< H >::faultHandler( const Record& iRecord,
					   const DataKey& iKey )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   // ---------- fill FAPtrTable with new items ----------------

   FATable< McCannCandidateQuality< H > > kalmanQualities;
   extract( iRecord, kalmanQualities, "", iKey.production().value() );
   if ( kalmanQualities.valid() )
   {
      FATableItr< McCannCandidateQuality< H > >
         kalmanFinished( kalmanQualities.end() );
      for ( FATableItr< McCannCandidateQuality< H > >
               kalmanQuality( kalmanQualities.begin() ) ;
            kalmanQuality != kalmanFinished ;
            ++kalmanQuality )
      {
         // we don't actually create anything new
         (*m_ptrTable).insert(
            (TRTrackCandidateFitQuality< H >*)(&*kalmanQuality) ) ;
      }
      return ( m_ptrTable );
   }

   // put other fitters' qualities here

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

//
// 'bound' functions
//

//void
//McCannBaseQualityProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE

/*
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
PROXY_BIND_METHOD( McCannBaseQualityProxy< H > )
PROXY_BIND_METHOD( McCannBaseQualityProxy< DBCandidate::kMuon > )
PROXY_BIND_METHOD( McCannBaseQualityProxy< DBCandidate::kChargedPion > )
PROXY_BIND_METHOD( McCannBaseQualityProxy< DBCandidate::kChargedKaon > )
PROXY_BIND_METHOD( McCannBaseQualityProxy< DBCandidate::kProton > )
PROXY_BIND_METHOD( McCannBaseQualityProxy< DBCandidate::kExitElectron > )
PROXY_BIND_METHOD( McCannBaseQualityProxy< DBCandidate::kExitMuon > )
PROXY_BIND_METHOD( McCannBaseQualityProxy< DBCandidate::kExitPion > )
PROXY_BIND_METHOD( McCannBaseQualityProxy< DBCandidate::kExitKaon > )
PROXY_BIND_METHOD( McCannBaseQualityProxy< DBCandidate::kExitProton > )
*/
//-----------------------------------------------


