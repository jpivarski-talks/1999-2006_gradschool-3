// -*- C++ -*-
//
// Package:     DataHandler
// Module:      FrameProvider
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Simon Patton
// Created:     Sat Nov  8 15:24:10 EST 1997
// $Id: FrameProvider.cc,v 1.7 2002/03/08 20:24:57 cdj Exp $
//
// Revision history
//
// $Log: FrameProvider.cc,v $
// Revision 1.7  2002/03/08 20:24:57  cdj
// speed improvement: under perverse conditions, was throwing many exceptions per event
//
// Revision 1.6  2000/11/18 19:59:40  cdj
// changed failed to synch Record message from WARNING to DEBUG
//
// Revision 1.5  2000/06/03 21:20:52  cdj
// now issue warning and continue if a Record that is supposed to be in the Frame fails to synchronize
//
// Revision 1.4  2000/03/17 15:33:04  cdj
// Added contents. Needed for FrameIterate
//
// Revision 1.3  2000/03/09 18:56:27  mkl
// FrameProvider: need to wipe out Records when wiping RecordProviders
//
// Revision 1.2  1998/01/23 05:18:12  mkl
// added reconfigureCurrentStop method to clear transient proxies
//
// Revision 1.1  1997/11/19 15:22:42  sjp
// New class for basic Frame Management
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>  // For 'exit'
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "DataHandler/RecordProvider.h"
#include "DataHandler/FrameProvider.h"
#include "DataHandler/StreamSet.h"
#include "DataHandler/DHStreamDidNotSyncException.h"

// STL classes

#include <map>

//
// constants, enums and typedefs
//

typedef _frameprovider_recordproviders_ Providers ;

static const char* const kFacilityString = "DataHandler.FrameProvider" ;

//
// static data member definitions
//

//
// constructors and destructor
//

FrameProvider::FrameProvider() :
   m_providers( *( new Providers ) )
{
   if ( 0 == &m_providers ) {
      report( EMERGENCY ,
	      kFacilityString )
		 << "Unable to allocate memory"
		 << endl ;
      exit( 1 ) ;
   }
}

// FrameProvider::FrameProvider( const FrameProvider& )
// {
// }

FrameProvider::~FrameProvider()
{
   eraseAll() ;
   delete &m_providers ;
}

//
// assignment operators
//
// const FrameProvider& FrameProvider::operator=( const FrameProvider& )
// {
// }

//
// member functions
//

Frame&
FrameProvider::currentFrame()
{
   return ( m_frame ) ;
}

void
FrameProvider::reconfigureCurrentStop()
{
   // reconfigure proxies for current stop
   // (clear transient proxies)
   recordProvider( currentFrame().stopStream() ).reconfigure();
}

Frame&
FrameProvider::frame( const Stream::Type& aStop ,
		      const SyncValue& aSyncValue )
{
//
// until 'mutable' is widely implemented need to cast away 'const'
//
   (*const_cast<FrameProvider*>(this)).m_frame.removeAll() ;
   (*const_cast<FrameProvider*>(this)).m_frame.setStop( aStop ) ;
   (*const_cast<FrameProvider*>(this)).m_frame.setSyncValue( aSyncValue ) ;
   Providers::const_iterator finished( m_providers.end() ) ;
   for ( Providers::const_iterator provider( m_providers.begin() ) ;
	 provider != finished ;
	 ++provider ) {
//
// test to see if this record is appropriate for this stop
//
      Record* record = 0;
      if ( ! ( aStop.priority() > (*((*provider).second)).stream().priority() )
	 && ( 0 != (record = ((*provider).second)->record(aSyncValue) ) ) ) {
	 (*const_cast<FrameProvider*>(this)).m_frame.add( *record ) ;
      }
   }
   return ( m_frame ) ;
}

Frame&
FrameProvider::emptyFrame()
{
   (*const_cast<FrameProvider*>(this)).m_frame.removeAll() ;
   (*const_cast<FrameProvider*>(this)).m_frame.setStop( Stream::kNone ) ;
   return ( m_frame ) ;
}

RecordProvider&
FrameProvider::recordProvider( const Stream::Type& aStream )
{
   if ( ! contains( aStream ) ) {
      report( ERROR ,
	      kFacilityString )
		 << "\nThere is no RecordProvider in this FrameProvider"
		 << " for the "
		 << aStream
		 << " stream"
		 << endl ;
      exit( 1 ) ;
   }
   return ( *((*(m_providers.find( aStream ))).second) ) ;
}

void
FrameProvider::insert( RecordProvider* pProvider )
{
//
// check the RecordProvider is not already in the list.  If it is
//   then this is an error, as each Stream should only appear once.
//
   if ( m_providers.end() != m_providers.find( (*pProvider).stream() ) ) {
      report( ERROR ,
	      kFacilityString )
		 << "\nTrying to `insert' Stream that already exists in Frame."
		 << "\nThis will be ignored, but you should check the code!"
		 << endl ;
      return ;
   }
//
// add ProxyProvider to list
//
   m_providers.insert( Providers::value_type( (*pProvider).stream() ,
					      pProvider ) ) ;
}

void
FrameProvider::erase( const Stream::Type& aStream )
{
   // need to wipe out all Record pointers
   emptyFrame();

//
// find the RecordProvider and erase it from the list if it exists
//
   Providers::iterator oldProvider = m_providers.find( aStream ) ;
   if( oldProvider != m_providers.end() ) {
      delete (*oldProvider).second ;
      m_providers.erase( oldProvider ) ;
   }
   else {
      report( WARNING ,
	      kFacilityString )
		 << "\nTrying to `erase' a Stream that is not is the Frame"
		 << endl ;
   }
}

void
FrameProvider::eraseAll()
{
   // need to wipe out all Record pointers
   emptyFrame();

   // wipe out providers
   Providers::iterator finished = m_providers.end() ;
   for( Providers::iterator provider( m_providers.begin() ) ;
	provider != finished ;
	++provider ) {
      delete (*provider).second;
   }
   m_providers.erase( m_providers.begin() ,
		      m_providers.end() ) ;
}

//
// const member functions
//

const Frame&
FrameProvider::currentFrame() const
{
   return ( m_frame ) ;
}

DABoolean
FrameProvider::contains( const Stream::Type& aStream ) const
{
   return ( m_providers.end() != m_providers.find( aStream ) ) ;
}

Stream::Set
FrameProvider::contents() const
{
   Stream::Set returnValue;

   Providers::iterator finished = m_providers.end() ;
   for( Providers::iterator provider( m_providers.begin() ) ;
	provider != finished ;
	++provider ) {
      returnValue.insert( (*provider).first );
   }
   return returnValue;
}

//
// static member functions
//
