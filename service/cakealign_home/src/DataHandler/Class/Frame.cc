// -*- C++ -*-
//
// Package:     DataHandler
// Module:      Frame
// 
// Description: A Container of Records
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Simon Patton
// Created:     Thu Sep 26 10:53:53 EDT 1996
// $Id: Frame.cc,v 1.18 2001/11/06 18:45:35 cdj Exp $
//
// Revision history
//
// $Log: Frame.cc,v $
// Revision 1.18  2001/11/06 18:45:35  cdj
// can now iterator over Records in a Frame
//
// Revision 1.17  1999/12/31 19:01:06  cdj
// - record() method can now thrown a DHNoRecordException
// - replaced C casts with const_cast<>
//
// Revision 1.16  1997/11/19 15:02:01  sjp
// Updated doc and interface for FrameProvider use
//
// Revision 1.15  1997/09/18 21:10:14  cdj
// changed Boolean to DABoolean
//
// Revision 1.14  1997/09/03 20:06:09  sjp
// Uses CPP macros for STL containers
//
// Revision 1.13  1997/09/01 15:35:16  sjp
// Changed report include to be in Experiment
//
// Revision 1.12  1997/08/27 03:39:03  sjp
// Updated to new name for bug flags
//
// Revision 1.11  1997/08/14 11:42:39  sjp
// Mods to run under CW Pro 1
//
// Revision 1.10  1997/07/24 16:14:17  sjp
// Changed call from Record::xxxContainer to Record::xxxFrame
//
// Revision 1.9  1997/07/09 17:59:07  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.8  1997/06/27 18:35:55  sjp
// Modified so DataField knows in which Frame it is living
//
// Revision 1.7  1997/05/07 19:00:18  sjp
// Fixed less<Stream::Type> so can be used as usual
//
// Revision 1.6  1997/04/30 19:40:38  sjp
// Fixed equality test in debug code
//
// Revision 1.5  1997/04/29 19:48:48  cdj
// replaced less<Stream::Type> with Stream::TypeLess and changed initial value
// of m_stopStream to Stream::kNone
//
// Revision 1.4  1997/04/11 18:57:36  sjp
// Cleaned up message issuing
//
// Revision 1.3  1997/04/01 16:01:19  sjp
// renamed flush function to be removeAll
//
// Revision 1.2  1997/03/29 16:20:34  sjp
// Renamed some functions, improved docuemtation
//
// Revision 1.1  1997/03/19 22:20:12  sjp
// New definitions for class
//

#include "Experiment/Experiment.h"
// system include files
#include <stdlib.h>  // For 'exit'
#include <string>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h" // For error report
#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "DataHandler/DHNoRecordException.h"

#include "DataHandler/FrameRecordItr.h"

// STL classes
#include <map> // for map

//
// constants, enums and typedefs
//

const string kFacilityString = "DataHandler.Frame" ;

typedef _frame_records_ RecordMap ;

//
// static data member definitions
//

//
// constructors and destructor
//
Frame::Frame() :
   m_stop( Stream::kNone ) ,
   m_records( *new RecordMap ) ,
   m_latestRecord( 0 )
{
   if ( 0 == &m_records ) {
      report( EMERGENCY ,
	      kFacilityString )
		 << "Unable to allocate memory"
		 << endl ;
      exit( 1 ) ;
   }
}

// Frame::Frame( const Frame& )
// {
// }

Frame::~Frame()
{
   delete &m_records ;
}

//
// assignment operators
//
// const Frame& Frame::operator=( const Frame& )
// {
// }

//
// member functions
//

Record&
Frame::record( const Stream::Type& aRecordStream )
{
   return  ( *const_cast<Record*>(&(*(const Frame*)this).record( aRecordStream ) ) ) ;
}

void
Frame::add( Record& aRecord )
{
   Stream::Type recordStream( aRecord.stream() ) ;
#if defined(CLEO_DEBUG)
//
// It should be impossible to have a record with no stream, as a Records
//    are should be created by a RecordProvider which ensures that it
//    has a Stream.  Therefore this is only here to check programming
//    flow.
//
   if ( Stream::kNone == recordStream ) {
      report( ERROR ,
	      kFacilityString )
		 << "\nProgramming error!! "
		 << "Record is not associated with a Stream."
		 << endl ;
      exit( 1 ) ;
   }
//
// It is FrameProvider's responsibility to make sure there is only one
//    RecordProvider (and this one Record) per Stream.  Therefore it
//    should be impossible for two Records from one stream to be added
//    to a Frame.  Therefore this is only here to check programming
//    flow.
//
   if ( contains( recordStream ) ) {
      report( ERROR ,
	      kFacilityString )
		 << "\nDuplicated record added to Frame!"
		 << "\nThis means there is a programming error "
		 << "in FrameDeliverer"
		 << endl ;
      exit( 1 ) ;
   }
#endif
//
// The FrameProvider is responsible for adding a removing Records from a
//    Frame.  It should be impossible for a Record to appear in two
//    Frames.  Therefore this is only here to check programming flow.
//
   if ( !aRecord.setFrame( *this ) ) {
      report( ERROR ,
	      kFacilityString )
		 << "\nTrying to associate Record with a second Frame"
		 << endl ;
      exit( 1 ) ;
   }
   m_records.insert( RecordMap::value_type( recordStream , &aRecord ) ) ;
}

void
Frame::setStop( const Stream::Type& aStop )
{
   m_stop = aStop ;
}

void
Frame::setSyncValue( const SyncValue& aSyncValue )
{
   m_syncValue = aSyncValue ;
}

void
Frame::removeAll()
{
   m_latestRecord = 0 ;
   RecordMap::iterator finished( m_records.end() ) ;
   for ( RecordMap::iterator record( m_records.begin() ) ;
	 record != finished ;
	 ++record ) {
      (*((*record).second)).clearFrame() ;
   }
   m_records.erase( m_records.begin() , m_records.end() ) ;
}

//
// const member functions
//

const Stream::Type&
Frame::stopStream() const
{
   return ( m_stop ) ;
}

const SyncValue&
Frame::syncValue() const
{
   return ( m_syncValue ) ;
}

DABoolean
Frame::contains( const Stream::Type& aRecordStream ) const
{
   if ( ! ( ( 0 != m_latestRecord ) &&
	    ( aRecordStream == (*m_latestRecord).stream() ) ) ) {
      RecordMap::const_iterator noEntry( m_records.end() ) ;
      RecordMap::const_iterator entry( m_records.find( aRecordStream ) ) ;
      if ( entry != noEntry ) {
//
// until `mutable' in generally available have to cast off const
//
	 const_cast<Frame*>(this)->m_latestRecord = (*entry).second ;
      }
      else {
//
// until `mutable' in generally available have to cast off const
//
	 const_cast<Frame*>(this)->m_latestRecord = 0 ;
      }
   }
   return ( 0 != m_latestRecord ) ;
}

const Record&
Frame::record( const Stream::Type& aRecordStream ) const
{
   if ( ! contains( aRecordStream ) ) {
      throw DHNoRecordException( aRecordStream );
   }
   return ( *m_latestRecord ) ;
}

FrameRecordItr
Frame::begin() const
{
   return FrameRecordItr( m_records.begin() );
}

FrameRecordItr
Frame::end() const
{
   return FrameRecordItr( m_records.end() );
}
//
// static member functions
//
