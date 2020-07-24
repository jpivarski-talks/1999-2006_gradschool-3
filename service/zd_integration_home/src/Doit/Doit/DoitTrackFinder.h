#if !defined(DOIT_DOITTRACKFINDER_H)
#define DOIT_DOITTRACKFINDER_H
// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      DoitTrackFinder
// 
// Description: Wrapping of the DOIT track finder.
//
// Usage:
//
// Author:      Adam Lyon
// Created:     Thu Apr 16 16:13:56 EDT 1998
// $Id: DoitTrackFinder.h,v 1.16 2002/09/27 21:01:25 bkh Exp $
//
// Revision history
//
// $Log: DoitTrackFinder.h,v $
// Revision 1.16  2002/09/27 21:01:25  bkh
// Add ZD capability and compatibility with new TrackFinder interface
//
// Revision 1.15  2001/04/05 02:01:41  lyon
// Pass SV Hybrid efficiencies to Fortran
//
// Revision 1.14  2001/01/24 21:32:14  cdj
// fixed memory leak
//
// Revision 1.13  2000/06/09 00:35:37  lkg
// New flag "m_useSilicon" added as a member datum.  No silicon information,
// constants or hits, are extracted and passed into Fortran-land when
// this flag is false.
//
// Revision 1.12  2000/05/30 20:28:21  pg
// Aded code creating Si lattices to seed tracks
//
// Revision 1.11  2000/05/27 05:22:29  lyon
// Handle cathode clusters
//
// Revision 1.10  2000/03/28 15:59:28  pg
// Now filling Si common blocks
//
// Revision 1.9  2000/02/08 18:33:46  lyon
// Allow for no bunch finder running and DoitModeNumber
//
// Revision 1.8  1999/12/01 22:14:57  lyon
// allow for gaps in identifier numbers
//
// Revision 1.7  1999/11/04 17:32:15  lyon
// Oops - for processors do not delete memory -- added zeroCache
//
// Revision 1.6  1999/10/31 15:13:27  lyon
// Added clearCache to fix memory leaks
//
// Revision 1.5  1999/08/31 21:17:10  lyon
// Internalizes seeds and lattices
//
// Revision 1.4  1999/05/28 03:17:36  lyon
// Passed frame to beginrun
//
// Revision 1.3  1999/05/20 13:06:18  lyon
// Added Frame to arguments of fillFortranCommons
//
// Revision 1.2  1999/04/08 07:39:28  wsun
// Added extraction and filtering of Si and Cathode hits in TrackFitter.
//
// Revision 1.1.1.1  1998/11/19 03:48:35  lyon
// Doit code moved from TrackFinder
//
//
// -- Old revision history when Doit was part of TrackFinder
// Revision 1.2  1998/07/26 18:56:54  lyon
// Working version
//
// Revision 1.1.1.1  1998/04/30 14:53:03  lyon
// Imported TrackFinder source
//

// system include files

// user include files
#include "TrackFinder/TrackFinder.h"
#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "TrackRoot/TRSeedTrack.h"
#include "FrameAccess/FAPtrTable.h"
#include "Lattice/Lattice.h"
#include "TrackFinder/SeedDRHitLink.h"
#include "TrackFinder/SeedZDHitLink.h"
#include "TrackFinder/SeedTrackSVRHitLattice.h"
#include "TrackFinder/SeedTrackSVZHitLattice.h"
#include "TrackFinder/SeedTrackCathodeHitLattice.h"
#include "Doit/DoitSeedQuality.h"


// forward declarations

/// Wraps the Doit Fortran code. Inherits from <emph>TrackFinder</emph>.
/// The two member data m_doitSeedTracks and m_doitLattice are used to hold
/// the results. DoitProcessor or DoitProd uses them to make this information
/// available in the Frame (either by insert or via a proxy).
class DoitTrackFinder : public TrackFinder
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DoitTrackFinder();

      virtual ~DoitTrackFinder();

      // ---------- member functions ---------------------------
      
      //////////////////////////////////////////////////////////////////////
      // initialize - initilzation code needed to use DOIT
      //
      void initialize() const;

      //////////////////////////////////////////////////////////////////////
      // beginRun - code to run at the start of each new run for DOIT
      //
      void beginRun(Frame& theFrame) const;
      
      //////////////////////////////////////////////////////////////////////
      // fillSVEfficiencyConstants - Fill new SV efficiency constants
      //
      void fillSVEfficiencyConstants(const Record& theRecord) const;


      //////////////////////////////////////////////////////////////////////
      // Find the tracks for this event for the given hits with DOIT
      //
      virtual DABoolean findTracks(
	 STL_VECTOR(const CalibratedDRHit*)* theDRHits, 
	 STL_VECTOR(const CalibratedSVRphiHit*)* theSVRphiHits, 
	 STL_VECTOR(const CalibratedSVZHit*)* theSVZHits, 
	 STL_VECTOR(const CalibratedCathodeHit*)* theCathodeHits, 
	 const Record& theRecord,
	 int diagLevel = 0 ,
	 STL_VECTOR(const CalibratedZDHit*)* theZDHits = 0
	 ) const ;

      //////////////////////////////////////////////////////////////////////
      // Fill Common blocks
      //
      void fillFortranCommons( STL_VECTOR(const CalibratedDRHit*)* theDRHits,
			       const Frame& theFrame ) const;

      void fillFortranCommons( STL_VECTOR(const CalibratedZDHit*)* theZDHits,
			       const Frame& theFrame ) const;

      void fillFortranCommons( STL_VECTOR(const CalibratedSVRphiHit*)* 
			       theSVRphiHits,
			       const Frame& theFrame ) const;

      void fillFortranCommons( STL_VECTOR(const CalibratedSVZHit*)* 
			       theSVZHits,
			       const Frame& theFrame ) const;
      //////////////////////////////////////////////////////////////////////
      // Insert tracks into the event
      //
      void insertTracksIntoFrame(Frame& theframe, int diagLevel) const;
  
      //////////////////////////////////////////////////////////////////////
      // Get track from Fortran
      //
      void getTracksFromFortran(Frame& theframe, int diagLevel, 
				const char* const drHitUsageTag = 0);

      //////////////////////////////////////////////////////////////////////
      // Clear the cache
      void clearCache();

  
      //////////////////////////////////////////////////////////////////////
      // Zero the cache
      void zeroCache();

      //////////////////////////////////////////////////////////////////////
      // Get the list of seed tracks
      FAPtrTable<TRSeedTrack>* getSeedTracks() const;

      //////////////////////////////////////////////////////////////////////
      // Get the Lattice linking Seed Tracks to Hits
      SeedTrackDRHitLattice* getLattice() const;

      //////////////////////////////////////////////////////////////////////
      // Get the Lattice linking Seed Tracks to Hits
      SeedTrackZDHitLattice* getZDLattice() const;

      //////////////////////////////////////////////////////////////////////
      // Get the Lattice linking Seed Tracks to SVR Hits
      SeedTrackSVRHitLattice* getSVRLattice() const;

      //////////////////////////////////////////////////////////////////////
      // Get the Lattice linking Seed Tracks to SVZ Hits
      SeedTrackSVZHitLattice* getSVZLattice() const;

      //////////////////////////////////////////////////////////////////////
      // Get the Lattice linking Seed Tracks to cathode Hits
      SeedTrackCathodeHitLattice* getCathodeLattice() const;

      //////////////////////////////////////////////////////////////////////
      // Get the Seed quality list for these tracks
      FAPtrTable<DoitSeedQuality>* getSeedQualities() const;

      //////////////////////////////////////////////////////////////////////
      // Get the mode number
      inline int modeNumber() const;

      //////////////////////////////////////////////////////////////
      // Set the mode number
      void setModeNumber(int modeNumber);
  
      //////////////////////////////////////////////////////////////
      // Set whether silicon is used
      void setSiliconUsage(DABoolean useSilicon);
      void setZDUsage(DABoolean useZD);
  
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      
      // No Default constructor
      DoitTrackFinder( const DoitTrackFinder& );
      
      // ---------- assignment operator(s) ---------------------

      // No Default assignment operator
      const DoitTrackFinder& operator=( const DoitTrackFinder& );


      // ---------- private member functions -------------
      void fillCache( FAPtrTable<TRSeedTrack>* iTracks,
		      FAPtrTable<DoitSeedQuality>*  iSeedQualities,
		      SeedTrackDRHitLattice* iDRLattice,
		      SeedTrackZDHitLattice* iZDLattice,
		      SeedTrackSVRHitLattice* iSVRLattice,
		      SeedTrackSVZHitLattice* iSVZLattice,
		      SeedTrackCathodeHitLattice* iCathodeLattice ) {
	 m_doitSeedTracks     = iTracks;
	 m_doitLattice        = iDRLattice;
	 m_doitZDLattice      = iZDLattice;
	 m_doitSVRLattice     = iSVRLattice;
	 m_doitSVZLattice     = iSVZLattice;
	 m_doitCathodeLattice = iCathodeLattice;
	 m_doitSeedQualities  = iSeedQualities;
      }

      // ---------- data members -------------------------------
      // Holders for the Seed tracks, Lattice and quality info.
      FAPtrTable<TRSeedTrack>*      m_doitSeedTracks;
      SeedTrackDRHitLattice*        m_doitLattice;
      SeedTrackZDHitLattice*        m_doitZDLattice;
      SeedTrackSVRHitLattice*       m_doitSVRLattice;
      SeedTrackSVZHitLattice*       m_doitSVZLattice;
      SeedTrackCathodeHitLattice*   m_doitCathodeLattice;
      FAPtrTable<DoitSeedQuality>*  m_doitSeedQualities;

      STL_VECTOR(int) m_hitIdentifiers;
      STL_VECTOR(int) m_ZDhitIdentifiers;

      int m_modeNumber;
      DABoolean m_useSilicon;
      DABoolean m_useZD;

      // ---------- static data members ------------------------

};

// inline function definitions
inline
int DoitTrackFinder::modeNumber() const 
{ 
  return m_modeNumber;
}

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "doit/Template/DoitTrackFinder.cc"
//#endif

#endif /* DOIT_DOITTRACKFINDER_H */
