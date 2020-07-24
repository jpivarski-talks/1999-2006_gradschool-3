// -*- C++ -*-
//
// Package:     TrueMuProc
// Module:      TrueMuProc
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Thu Jun 28 12:22:20 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "TrueMuProc/TrueMuProc.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "CleoDB/DBEventHeader.h"
#include "BinaryDelivery/ByteSwapping.h"

// STL classes
// You may have to uncomment some of these or other stl headers 
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "Processor.TrueMuProc" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.17 2000/12/04 19:11:11 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
TrueMuProc::TrueMuProc( void )               // anal1
   : Processor( "TrueMuProc" ),
     m_cutdir( "cutdir", this, "/cdat/daf9/mccann/truemu/simplecut" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &TrueMuProc::event,    Stream::kEvent );
   //bind( &TrueMuProc::beginRun, Stream::kBeginRun );
   //bind( &TrueMuProc::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

   m_listsize = 0;
   m_index = 0;
   m_listbuffer = NULL;
   m_runnum = 0;
}

TrueMuProc::~TrueMuProc()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
TrueMuProc::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
TrueMuProc::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
   if ( m_listbuffer != NULL )
      delete [] m_listbuffer;
}

// ---------------- standard place to book histograms ---------------
void
TrueMuProc::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
TrueMuProc::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< DBEventHeader > header;
   extract( iFrame.record( Stream::kEvent ), header );
   unsigned int run = header->run();
   unsigned int event = header->number();

   if ( run != m_runnum )
   {
      if ( m_listbuffer != NULL )
      {
	 delete [] m_listbuffer;
	 m_listbuffer = NULL;
      }

      assert( read_run( run ) );
      m_runnum = run;
   }

   // This algorithm allows for missing events in the minipass2 sample
   // that existed in the pass2 sample. Of course it assumes that both
   // lists are sorted--- events are collected in order and they are
   // processed in order, so everything will always remain sorted.

   if ( m_index >= m_listsize )
      return ActionBase::kFailed;

   while ( event != m_listbuffer[ m_index ] )
   {
      if ( event < m_listbuffer[ m_index ] )
	 return ActionBase::kFailed;

      if ( event > m_listbuffer[ m_index ] )
	 m_index++;
   }
   
   return ActionBase::kPassed;
}

DABoolean
TrueMuProc::read_run( unsigned int runnum )
{
   char rundir[5], runfile[4];
   sprintf( rundir, "/%03d", int( floor( runnum / 1000 ) ) );
   sprintf( runfile, "%03d", runnum % 1000 );
   
   char filename[14];
   sprintf( filename, "%s/%s.truemu", rundir, runfile );
   string fullpath( m_cutdir.value() );
   fullpath += filename;

   report( INFO, kFacilityString ) << "Reading file "
				   << fullpath.c_str() << "." << endl;
   
   FILE* inputfile = fopen( fullpath.c_str(), "rb" );
   if ( inputfile == NULL )
   {
      report( NOTICE, kFacilityString )
	 << "Couldn't read from file " << fullpath.c_str()
	 << " so I am skipping this run (rejecting all events)..." << endl;
      m_index = m_listsize = 0;
      return true;
   }

   report( INFO, kFacilityString ) << "Reading headers..." << endl;

   // if "n" future fields are added, buffersize = buffersize + n
   // and indexstart = indexstart + n;
   // then write out the values of those n fields
   if ( fread( &m_listsize, UNSIGNED_INT, 1, inputfile ) != 1 )
      return false;
#if ( AC_BIGENDIAN == 0 )
   m_listsize = swapBytesInUInt32( m_listsize );
#endif // AC_BIGENDIAN
   report( DEBUG, kFacilityString ) << "List size is " << m_listsize << endl;
   m_listbuffer = new unsigned int [ m_listsize ];
   if ( m_listbuffer == NULL )
      return false;
   report( INFO, kFacilityString )
      << "Allocated array of size " << m_listsize << endl;

   unsigned int headers;
   if ( fread( &headers, UNSIGNED_INT, 1, inputfile ) != 1 )
      return false;
#if ( AC_BIGENDIAN == 0 )
   headers = swapBytesInUInt32( headers );
#endif // AC_BIGENDIAN
   report( DEBUG, kFacilityString )
      << "Number of headers is " << headers << endl;

   for ( m_index = 0;  m_index < headers;  m_index++ )
   {
      unsigned int headernum;
      if ( fread( &headernum, UNSIGNED_INT, 1, inputfile ) != 1 )
	 return false;
#if ( AC_BIGENDIAN == 0 )
      headernum = swapBytesInUInt32( headernum );
#endif // AC_BIGENDIAN

      report( INFO, kFacilityString )
	 << "    header[ " << m_index << " ] = "
	 << headernum << endl;
   }

   report( INFO, kFacilityString ) << "Reading event list..." << endl;

   unsigned int checksum = 0;
   for ( m_index = 0;  m_index < m_listsize;  m_index++ )
   {
      if ( fread( &m_listbuffer[ m_index ], UNSIGNED_INT, 1, inputfile ) != 1 )
	 return false;
#if ( AC_BIGENDIAN == 0 )
      m_listbuffer[ m_index ] = swapBytesInUInt32( m_listbuffer[ m_index ] );
#endif // AC_BIGENDIAN

      report( DEBUG, kFacilityString )
	 << "    m_listbuffer[ " << m_index << " ] = "
	 << m_listbuffer[ m_index ] << endl;

      try {
	 checksum += m_listbuffer[ m_index ];
      }
      catch( overflow_error& thisException )
      {
	 report( DEBUG, kFacilityString ) << "checksum rolled over." << endl;
      }
   }
   report( INFO, kFacilityString )
      << "Read whole event list from disk." << endl;

   report( INFO, kFacilityString )
      << "Reading checksum from the file..." << endl;
   unsigned int file_checksum;
   if ( fread( &file_checksum, UNSIGNED_INT, 1, inputfile ) != 1 )
      return false;
#if ( AC_BIGENDIAN == 0 )
   file_checksum = swapBytesInUInt32( file_checksum );
#endif // AC_BIGENDIAN
   if ( checksum == file_checksum )
      report( INFO, kFacilityString )
	 << "Calculated checksum = " << checksum
	 << ", file checksum = " << file_checksum << endl;
   else
   {
      report( EMERGENCY, kFacilityString )
	 << "Error reading event list file: calculated checksum = " << checksum
	 << ", file checksum = " << file_checksum << endl;
      return false;
   }

   report( INFO, kFacilityString ) << "Closing file..." << endl;
   fclose( inputfile );

   m_index = 0;

   return true;
}

/*
ActionBase::ActionResult
TrueMuProc::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
TrueMuProc::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

//
// const member functions
//

//
// static member functions
//
