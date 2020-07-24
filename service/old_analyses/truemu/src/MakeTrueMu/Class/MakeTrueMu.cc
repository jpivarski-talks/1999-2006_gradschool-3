// -*- C++ -*-
//
// Package:     MakeTrueMu
// Module:      MakeTrueMu
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim McCann
// Created:     Thu Jun 21 12:26:00 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "MakeTrueMu/MakeTrueMu.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "CleoDB/DBEventHeader.h"
#include "Navigation/NavTrack.h"
#include "EventType/EventType.h"
#include "C3cc/CcQedEvent.h"
#include "CLHEP/Vector/ThreeVector.h"

//  //// Temoporary code to emulate TracksForSiAlignment /////////////////////////////////////////////
//  #include "TrackRoot/TRSubdetectorLists.h"
//  //////////////////////////////////////////////////////////////////////////////////////////////////

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
static const char* const kFacilityString = "Processor.MakeTrueMu" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.17 2000/12/04 19:11:11 cdj Exp $";
static const char* const kTagString = "$Name: v03_06_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
MakeTrueMu::MakeTrueMu( void )               // anal1
   : Processor( "MakeTrueMu" ),
     m_dir_simplecut( "simplecut_dir", this, "/cdat/daf9/mccann/truemu/simplecut" ),
     m_d0cut( "d0cut", this, 0.0015 ),
     m_z0cut( "z0cut", this, 0.04 )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &MakeTrueMu::event,    Stream::kEvent );
   //bind( &MakeTrueMu::beginRun, Stream::kBeginRun );
   //bind( &MakeTrueMu::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)

   m_prevrun = 0;
}

MakeTrueMu::~MakeTrueMu()                    // anal5
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
MakeTrueMu::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
MakeTrueMu::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)

   if ( m_list_simplecut.size() != 0 )
   {
      report( INFO, kFacilityString )
	 << "Writing out list of events that passed simplecut..." << endl;
      assert( write_run( m_dir_simplecut.value().c_str(), m_list_simplecut.size(),
			 m_list_simplecut.begin(), m_list_simplecut.end() ) );
      report( INFO, kFacilityString ) << "Deleting list from memory..." << endl;
      m_list_simplecut.erase( m_list_simplecut.begin(), m_list_simplecut.end() );
   }
}

// ---------------- standard place to book histograms ---------------
void
MakeTrueMu::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

   m_all_d0vphi0 = iHistoManager.histogram(
      100, "d0 VS. phi0", 157, 0., 6.2831852, 200, -0.010, 0.010 );
   m_all_z0 = iHistoManager.histogram(
      120, "z0", 200, -0.12, 0.12 );
   m_all_phi0 = iHistoManager.histogram(
      130, "phi0", 157, 0., 6.2831852 );

   m_simplecut_d0vphi0 = iHistoManager.histogram(
      200, "cut d0 VS. phi0", 157, 0., 6.2831852, 200, -0.004, 0.004 );
   m_simplecut_z0 = iHistoManager.histogram(
      220, "cut z0", 200, -0.04, 0.04 );
   m_simplecut_phi0 = iHistoManager.histogram(
      230, "cut phi0", 157, 0., 6.2831852 );

   m_momentum = iHistoManager.histogram(
      500, "momentum", 1000, 0., 7. );

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
MakeTrueMu::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;

   FAItem< EventType > eventType;
   extract( iFrame.record( Stream::kEvent ), eventType );
   if ( !eventType->muPair() ) //  && !eventType->radMuP() )
   {
      report( DEBUG, kFacilityString )
	 << "EventType says this isn't a mupair!" << endl;
      return ActionBase::kFailed;
   }

   FAItem< DBEventHeader > header;
   extract( iFrame.record( Stream::kEvent ), header );
   unsigned int run = header->run();
   unsigned int event = header->number();

   if ( m_prevrun == 0 )
      m_prevrun = run;
   else if ( run != m_prevrun )
   {
      report( INFO, kFacilityString )
	 << "Writing out list of events that passed simplecut..." << endl;
      assert( write_run( m_dir_simplecut.value().c_str(), m_list_simplecut.size(),
			 m_list_simplecut.begin(), m_list_simplecut.end() ) );
      report( INFO, kFacilityString ) << "Deleting list from memory..." << endl;
      m_list_simplecut.erase( m_list_simplecut.begin(), m_list_simplecut.end() );

      m_prevrun = run;
   }

   unsigned int num_good_tracks = 0;

//  //// Temoporary code to emulate TracksForSiAlignment /////////////////////////////////////////////
// 
//     double event_curvature = 1.;
//     HepVector3D event_momentum = HepVector3D( 0., 0., 0. );
// 
// //////////////////////////////////////////////////////////////////////////////////////////////////

   FATable< NavTrack > navtracks;
   extract( iFrame.record( Stream::kEvent ), navtracks );
   FATable< NavTrack >::const_iterator navtracks_iterator;
   FATable< NavTrack >::const_iterator navtracks_begin = navtracks.begin();
   FATable< NavTrack >::const_iterator navtracks_end = navtracks.end();

   // I want exactly two good tracks
   if ( navtracks.size() != 2 )
      return ActionBase::kFailed;

   for( navtracks_iterator = navtracks_begin;
	navtracks_iterator != navtracks_end;
	navtracks_iterator++ )
   {
      // Get the Kalman-fitted track
      FAItem< TDKinematicFit > trackFit = navtracks_iterator->muonFit();
      if ( trackFit == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "muonFit is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }

      // and the Kalman-fitted helix
      FAItem< TRHelixFit > trackHelix = navtracks_iterator->muonHelix();
      if ( trackHelix == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "muonHelix is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }

      // Explicitly set this to the beamspot for a better D0 cut.
      TRHelixFit loc_trackHelix = (* trackHelix );
      Meters movelength;
      KTHelix::MoveStatus status =
	 loc_trackHelix.moveToReferencePoint( HepPoint3D( -0.00160, 0., 0. ),
					       movelength );
      if ( status != KTMoveControl::kMoveOK )
      {
	 report( EMERGENCY, kFacilityString )
	    << "trackHelix.moveToReferencePoint returned "
	    << status << " (not kMoveOK, as it should)." << endl;
	 return ActionBase::kFailed;
      }

      // Get its quality object
      FAItem< TRTrackFitQuality > trackQuality = navtracks_iterator->muonQuality();
      if ( trackQuality == NULL )
      {
	 report( EMERGENCY, kFacilityString )
	    << "muonQuality is missing from the NavTrack!" << endl;
	 return ActionBase::kFailed;
      }
   
      double momentum = navtracks_iterator->muonFit()->momentum().mag();
      m_momentum->fill( momentum );

      double d0 = loc_trackHelix.d0();
      double z0 = loc_trackHelix.z0();
      double phi0 = loc_trackHelix.phi0();

      m_all_d0vphi0->fill( phi0, d0 );
      m_all_z0->fill( z0 );
      m_all_phi0->fill( phi0 );

//  //// Temoporary code to emulate TracksForSiAlignment /////////////////////////////////////////////
// 
//        // event curvature is negative (opposite charges)
//        event_curvature *= trackHelix->curvature();
//        // event momentum cut is 0.300
//        event_momentum += trackFit->momentum();
// 
//        // both tracks satisfy:
//        // mindr layers is 30
//        // min rphi layers is 2
//        // min z layers is 2
//        // cotTheta cut is 1.25
//        if ( ( trackQuality->numberSubdetectorHitLayers( TRSubdetectorLists::kDR3Axial ) +
//  	     trackQuality->numberSubdetectorHitLayers( TRSubdetectorLists::kDR3Stereo ) ) < 30  ||
//  	   trackQuality->numberSubdetectorHitLayers( TRSubdetectorLists::kSVR ) < 2             ||
//  	   trackQuality->numberSubdetectorHitLayers( TRSubdetectorLists::kSVZ ) < 2             ||
//  	   trackHelix->cotTheta() > 1.25                                                           )
//  	 return ActionBase::kFailed;
// 
//  //////////////////////////////////////////////////////////////////////////////////////////////////
	 
      char d0str[10], z0str[10], phi0str[10], sinstr[10];
      sprintf( d0str, "%9.6f", d0 );
      sprintf( z0str, "%9.6f", z0 );
      sprintf( phi0str, "%9.6f", phi0 );

      report( DEBUG, kFacilityString )
	 << "Track d0 = " << d0str << ", z0 = " << z0str
	 << " and phi0 = " << phi0str
	 << " and momentum = " << momentum << endl;

      if ( ! trackQuality->fitAbort()       &&
	   abs( d0 ) < m_d0cut.value()      &&
	   abs( z0 ) < m_z0cut.value()      &&
	   4. < momentum && momentum < 6.5     )
      {
	 num_good_tracks++;

	 m_simplecut_d0vphi0->fill( phi0, d0 );
	 m_simplecut_z0->fill( z0 );
	 m_simplecut_phi0->fill( phi0 );
      }
      else
	 return ActionBase::kFailed;

   } // end navtracks loop

//  //// Temoporary code to emulate TracksForSiAlignment /////////////////////////////////////////////
// 
//     // event curvature is negative (opposite charges)
//     if ( event_curvature > 0. )
//        return ActionBase::kFailed;
// 
//     // event momentum cut is 0.300
//     double xdiff2 = sqr( event_momentum.x() + 0.0261 );
//     double ydiff2 = sqr( event_momentum.y() + 0.0003 );
//     if ( ( xdiff2 + ydiff2 ) > 0.300 )
//        return ActionBase::kFailed;
// 
//  //////////////////////////////////////////////////////////////////////////////////////////////////

   m_list_simplecut.push_back( event );
   report( DEBUG, kFacilityString ) << "simplecut takes event number "
				    << event << " (" << m_list_simplecut.size()
				    << " so far)." << endl;

   return ActionBase::kPassed;
}

// we have completed a run, time to write it all out to the file
DABoolean
MakeTrueMu::write_run( const char* iDir, int iList_size,
		       vector< unsigned int >::const_iterator iList_begin,
		       vector< unsigned int >::const_iterator iList_end ) 
{
   char rundir[5], runfile[4];
   sprintf( rundir, "/%03d", int( floor( m_prevrun / 1000 ) ) );
   sprintf( runfile, "%03d", m_prevrun % 1000 );

   // check to see if a directory exists for the first three digits of
   // the run number
   string partpath( iDir );
   partpath += rundir;
   struct stat statbuf;
   if ( stat( partpath.c_str(), &statbuf ) != 0 )
   {
      if ( errno == ENOENT )
      {
	 report( INFO, kFacilityString )
	    << "The directory " << partpath.c_str()
	    << " doesn't yet exist." << endl;

	 // fork a "mkdir" process to make the directory
	 int cpid, status;
	 cpid = fork();
	 switch ( cpid )
	 {
	    case -1:
	       report( EMERGENCY, kFacilityString )
		  << "Cannot fork a \"mkdir\" process." << endl;
	       return false;
	       break; // (for style only)

	    case 0:
	       if ( execl( "/usr/bin/mkdir", "mkdir",
			   partpath.c_str(), (char *)( 0 ) ) != 0 )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "Failed to execute \"/usr/bin/mkdir "
		     << partpath.c_str()
		     << "\" with error " << errno << endl;
		  report( EMERGENCY, kFacilityString )
		     << "Note: "
		     << "E2BIG = " << E2BIG << ", "
		     << "EACCES = " << EACCES << ", "
		     << "EAGAIN = " << EAGAIN << ", "
		     << "EFAULT = " << EFAULT << ", "
		     << "ELOOP = " << ELOOP << ", "
		     << "ENAMETOOLONG = " << ENAMETOOLONG << ", "
		     << "ENOENT = " << ENOENT << ", "
		     << "ENOMEM = " << ENOMEM << ", "
		     << "ENOTDIR = " << ENOTDIR << ", "
		     << "ENOEXEC = " << ENOEXEC << ", "
		     << "EWOULDBLOCK = " << EWOULDBLOCK << endl;
		  return false;
	       }
	       break;

	    default:
	       while( wait( &status ) != cpid );  // wait for mkdir to finish
	       if ( status & 0xFFFF )
	       {
		  report( EMERGENCY, kFacilityString )
		     << "child \"mkdir\" process returned "
		     << ( status & 0xFFFF0000 ) << " (high bytes), "
		     << ( status & 0x0000FFFF ) << " (low bytes)" << endl;
		  return false;
	       }
	       break;
	 }

// Don't worry about setting permissions for now--- the permissions
// are automatically the right thing
// 
//  	 if ( execl( "/usr/bin/chmod", "chmod",
//  		     "755", partpath.c_str(), (char *)( 0 ) ) != 0 )
//  	    {
//  	       report( EMERGENCY, kFacilityString )
//  		  << "Failed to execute \"/usr/bin/chmod 755 "
//  		  << partpath.c_str() << "\" with error " << errno << endl;
//  	    }

	 report( INFO, kFacilityString )
	    << "Successfully created " // and set permissions \"755\" to "
	    << partpath.c_str() << endl;
      }
      else
      {
	 report( EMERGENCY, kFacilityString )
	    << "Looked at path \"" << partpath.c_str() << "\" and got "
	    << errno << " != " << ENOENT << " (ENOENT) from stat()!" << endl;
	 report( EMERGENCY, kFacilityString )
	    << "Note: "
	    << "EACCES = " << EACCES << ", "
	    << "EFAULT = " << EFAULT << ", "
	    << "ELOOP = " << ELOOP << ", "
	    << "ENAMETOOLONG = " << ENAMETOOLONG << ", "
	    << "ENOENT = " << ENOENT << ", "
	    << "ENOTDIR = " << ENOTDIR << ", "
	    << "EINVAL = " << EINVAL << ", "
	    << "EISDIR = " << EISDIR << ", "
	    << "ENFILE = " << ENFILE << ", "
	    << "ESTALE = " << ESTALE << ", "
	    << "EBADF = " << EBADF << ", "
	    << "EFAULT = " << EFAULT << endl;
	 return false;
      }
   }
   else
      report( INFO, kFacilityString )
	 << "The directory " << partpath.c_str() << " already exists." << endl;

   char filename[14];
   sprintf( filename, "%s/%s.truemu", rundir, runfile );
   string fullpath( iDir );
   fullpath += filename;

   report( INFO, kFacilityString ) << "Creating/writing file "
				   << fullpath.c_str() << "." << endl;
   
   FILE* outputfile = fopen( fullpath.c_str(), "wb" );
   if ( outputfile == NULL )
      return false;

   report( INFO, kFacilityString ) << "Writing headers..." << endl;

   // if "n" future fields are added, buffersize = buffersize + n
   // and indexstart = indexstart + n;
   // then write out the values of those n fields
   // two so far: d0cut and z0cut
   unsigned int buffersize = iList_size;
   report( INFO, kFacilityString )
      << "    buffersize = " << buffersize << endl;
#if ( AC_BIGENDIAN == 0 )
   buffersize = swapBytesInUInt32( buffersize );
#endif // AC_BIGENDIAN
   if ( fwrite( &buffersize, UNSIGNED_INT, 1, outputfile ) != 1 )
      return false;

   unsigned int headers = 2;
   report( INFO, kFacilityString )
      << "    headers = " << headers << endl;
#if ( AC_BIGENDIAN == 0 )
   headers = swapBytesInUInt32( headers );
#endif // AC_BIGENDIAN
   if ( fwrite( &headers, UNSIGNED_INT, 1, outputfile ) != 1 )
      return false;

   unsigned int d0cut = int( floor( m_d0cut.value() * 1E6 ) );
   report( INFO, kFacilityString )
      << "    d0cut = " << d0cut << endl;
#if ( AC_BIGENDIAN == 0 )
   d0cut = swapBytesInUInt32( d0cut );
#endif // AC_BIGENDIAN
   if ( fwrite( &d0cut, UNSIGNED_INT, 1, outputfile ) != 1 )
      return false;

   unsigned int z0cut = int( floor( m_z0cut.value() * 1E6 ) );
   report( INFO, kFacilityString )
      << "    z0cut = " << z0cut << endl;
#if ( AC_BIGENDIAN == 0 )
   z0cut = swapBytesInUInt32( z0cut );
#endif // AC_BIGENDIAN
   if ( fwrite( &z0cut, UNSIGNED_INT, 1, outputfile ) != 1 )
      return false;

   report( INFO, kFacilityString ) << "Writing event list..." << endl;
   vector< unsigned int >::const_iterator iter;
   unsigned int checksum = 0;
   for ( iter = iList_begin;  iter != iList_end;  iter++ )
   {
      unsigned int event = (* iter );
      try {
	 checksum += event;
      }
      catch( overflow_error& thisException )
      {
	 report( DEBUG, kFacilityString ) << "checksum rolled over." << endl;
      }
      report( DEBUG, kFacilityString )
	 << "event = " << event << ", checksum = " << checksum
	 << ", 0xFFFFFFFF = " << 0xFFFFFFFF << endl;

#if ( AC_BIGENDIAN == 0 )
      event = swapBytesInUInt32( event );
#endif // AC_BIGENDIAN
      if ( fwrite( &event, UNSIGNED_INT, 1, outputfile ) != 1 )
	 return false;
   }

   report( INFO, kFacilityString )
      << "Putting an amateur checksum (value = " << checksum
      << ") at the end for redundancy..." << endl;
#if ( AC_BIGENDIAN == 0 )
   checksum = swapBytesInUInt32( checksum );
#endif // AC_BIGENDIAN
   if ( fwrite( &checksum, UNSIGNED_INT, 1, outputfile ) != 1 )
      return false;

   report( INFO, kFacilityString ) << "Closing file..." << endl;
   fclose( outputfile );

// Don't worry about setting permissions for now--- the permissions
// are automatically the right thing
// 
//     if ( execl( "/usr/bin/chmod", "chmod",
//  	       "644", fullpath.c_str(), (char *)( 0 ) ) != 0 )
//     {
//        report( EMERGENCY, kFacilityString )
//  	 << "Failed to execute \"/usr/bin/chmod 644 "
//  	 << fullpath.c_str() << "\" with error " << errno << endl;
//     }
//     report( INFO, kFacilityString )
//        << "Successfully set file permissions to \"644\"!" << endl;

   return true;
}

/*
ActionBase::ActionResult
MakeTrueMu::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
MakeTrueMu::endRun( Frame& iFrame )         // anal4 equiv.
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
