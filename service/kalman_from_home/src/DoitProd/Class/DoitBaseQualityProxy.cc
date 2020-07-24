// -*- C++ -*-
//
// Package:     <DoitProducer>
// Module:      DoitBaseQualityProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Thu Jun  3 01:10:06 EDT 1999
// $Id: DoitBaseQualityProxy.cc,v 1.4 2000/08/16 19:45:50 wsun Exp $
//
// Revision history
//
// $Log: DoitBaseQualityProxy.cc,v $
// Revision 1.4  2000/08/16 19:45:50  wsun
// Updated to new TRSeedTrackQuality.
//
// Revision 1.3  2000/02/14 19:21:46  lyon
// Propagate Usage and Production tags
//
// Revision 1.2  2000/02/08 18:35:33  lyon
// Added template instantiation in the right place
//
// Revision 1.1  1999/10/31 16:10:17  lyon
// Need proxy for the base quality information
//
// Revision 1.1.1.1  1999/06/04 07:23:27  wsun
// Imported sources.
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
#include <set>
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

#include "DoitProd/DoitBaseQualityProxy.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#if 0
#include "AnalysisExplorer/NavTrack.h" 
#endif

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "DoitBaseQualityProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitBaseQualityProxy.cc,v 1.4 2000/08/16 19:45:50 wsun Exp $";
static const char* const kTagString = "$Name: v01_10_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoitBaseQualityProxy::DoitBaseQualityProxy()
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
   //bind( &DoitBaseQualityProxy::boundMethod, Stream::kBeginRun );
}

// DoitBaseQualityProxy::DoitBaseQualityProxy( const DoitBaseQualityProxy& iproxy )
// {
//   *this = iproxy;
// }

DoitBaseQualityProxy::~DoitBaseQualityProxy()
{
  // Erase the map to avoid a call to eraseAll by the destructor
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseWithoutDeleteAll() ;
   }
   delete m_ptrTable ;
}

//
// assignment operators
//
// const DoitBaseQualityProxy& DoitBaseQualityProxy::operator=( const DoitBaseQualityProxy& iproxy )
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
void
DoitBaseQualityProxy::invalidateCache()
{
  // Erase the pointer table BUT DO NOT DELETE THE CONTENTS!
  // The contents are deleted by DoitProxy
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseWithoutDeleteAll() ;
   }
}

const DoitBaseQualityProxy::value_type*
DoitBaseQualityProxy::faultHandler( const Record& iRecord,
				    const DataKey& iKey )
{
   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

   FATable< DoitSeedQuality > DoitQualities ;
   extract( iRecord, DoitQualities, iKey.usage().value(), 
	    iKey.production().value() ) ;

   FATableItr< DoitSeedQuality > qualityItr = DoitQualities.begin() ;
   FATableItr< DoitSeedQuality > qualityEnd = DoitQualities.end() ;

   for( ; qualityItr != qualityEnd ; ++qualityItr )
   {
      // we don't actually create anything new
      m_ptrTable->insert( ( TRSeedTrackQuality* ) ( &*qualityItr ) ) ;
   }

   return ( m_ptrTable ) ;
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
//DoitBaseQualityProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< TRSeedTrackQuality > >;

PROXY_BIND_METHOD( DoitBaseQualityProxy )
//-----------------------------------------------


