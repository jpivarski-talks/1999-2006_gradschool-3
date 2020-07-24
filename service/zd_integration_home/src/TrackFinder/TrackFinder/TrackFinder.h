#if !defined(TRACKFINDER_TRACKFINDER_H)
#define TRACKFINDER_TRACKFINDER_H
// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      TrackFinder
// 
// Description: Base class for the Track Finders
//
// Usage:
//    All track finders must inherit from this abstract base class.
//
//    The call sequence is ...
//
//        TrackFinder::event (your track finder inherits this from here - do
//                            not override this function as it defines the
//                            track finding call sequence). 
//
//        event calls...
//
//          * filterXXXHits(), where XXX = DR, SVRphi, SVZ, Cathode.
//                 You may override these functions if you want a different
//                 hit filter. Default is to allow all hits for pattern
//                 recognition
//
//          * findTracks - This is an abstract virtual function that must
//                 be implemented by your track finder
//
//          * insertTracksIntoFrame - Insert the seed tracks into the frame
//                 Abstract virtual function - you must provide this function.
//
//
//
// Author:      Adam Lyon
// Created:     Wed Apr 15 13:42:27 EDT 1998
// $Id: TrackFinder.h,v 1.9 2002/09/27 21:03:06 bkh Exp $
//
// Revision history
//
// $Log: TrackFinder.h,v $
// Revision 1.9  2002/09/27 21:03:06  bkh
// Add ZD capability
//
// Revision 1.8  2000/02/08 18:28:00  lyon
// Add stuff necessary for Storage Helpers
//
// Revision 1.7  2000/01/13 22:14:06  lyon
// Added set functions for the exceptionContinue flags
//
// Revision 1.6  2000/01/11 20:03:32  wsun
// Added flags to control how/if exceptions are handled.
//
// Revision 1.5  1999/06/04 06:53:29  wsun
// Added argument to event() to dis/enable insertion of seeds.
//
// Revision 1.4  1999/04/08 07:35:56  wsun
// Added extraction and filtering of Si and Cathode hits in TrackFitter.
//
// Revision 1.3  1998/10/06 18:17:49  lyon
// Using STL macros
//
// Revision 1.2  1998/07/26 18:56:56  lyon
// Working version
//
// Revision 1.1.1.1  1998/04/30 14:53:03  lyon
// Imported TrackFinder source
//

// system include files

// user include files
#include "Experiment/Experiment.h"
#include "Experiment/types.h"
#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class CalibratedDRHit ;
class CalibratedZDHit ;
class CalibratedSVRphiHit ;
class CalibratedSVZHit ;
class CalibratedCathodeHit ;

class TrackFinder
{
// ---------- friend classes and functions ---------------

   public:
// ---------- constants, enums and typedefs --------------

// ---------- Constructors and destructor ----------------
      TrackFinder( DABoolean aContinueIfHitsMissing = false,
		   DABoolean aContinueIfAnyException = false ) ;
      virtual ~TrackFinder();

      // ---------- member functions ---------------------------

      //////////////////////////////////////////////////////////////////////
      // Do the main track finding loop for this event
      //   Arguents:
      //         const Record& theRecord = Record to get data from
      //
      //   *** DO NOT OVERRIDE THIS FUNCTION UNLESS YOU 
      //                       KNOW WHAT YOU ARE DOING! ***
      //
      DABoolean event( Frame& theFrame,
		       int diagLevel = 0,
		       DABoolean insertTracks = true ) const;

      //////////////////////////////////////////////////////////////////////
      //  Special event routine that does not call the bunch finder
      //
      //   *** DO NOT OVERRIDE THIS FUNCTION UNLESS YOU 
      //                       KNOW WHAT YOU ARE DOING! ***
      //
      DABoolean eventNoBunchFinder( Frame& theFrame,
		       int diagLevel = 0,
		       DABoolean insertTracks = true ) const;


      // Set the exceptionContinuation flags
      void setExceptionContinuationFlags(DABoolean continueIfHitsMissing,
					 DABoolean continueIfAnyException);
					 
	

      //////////////////////////////////////////////////////////////////////
      // Filter the CalibratedDRHits
      //
      //       virtual STL_VECTOR(const CalibratedDRHit*)* filterDRHits(
      // 	 const Record& theRecord ) const;

      virtual STL_VECTOR(const CalibratedDRHit*)* filterDRHits(
	 const Record& theRecord, const char* const drHitUsageTag = 0 ) const ;

      virtual STL_VECTOR(const CalibratedZDHit*)* filterZDHits(
	 const Record& theRecord, const char* const zdHitUsageTag = 0 ) const ;

      virtual STL_VECTOR(const CalibratedSVRphiHit*)* filterSVRphiHits(
	 const Record& theRecord ) const ;

      virtual STL_VECTOR(const CalibratedSVZHit*)* filterSVZHits(
	 const Record& theRecord ) const ;

      virtual STL_VECTOR(const CalibratedCathodeHit*)* filterCathodeHits(
	 const Record& theRecord ) const ;

      
      //////////////////////////////////////////////////////////////////////
      // Find the tracks for this event for the given hits
      //
      virtual DABoolean findTracks(
	 STL_VECTOR(const CalibratedDRHit*)* theDRHits, 
	 STL_VECTOR(const CalibratedSVRphiHit*)* theSVRphiHits, 
	 STL_VECTOR(const CalibratedSVZHit*)* theSVZHits, 
	 STL_VECTOR(const CalibratedCathodeHit*)* theCathodeHits, 
	 const Record& theRecord,
	 int diagLevel = 0,
	 STL_VECTOR(const CalibratedZDHit*)* theZDHits = 0 
	 ) const = 0 ;

      //////////////////////////////////////////////////////////////////////
      // Insert Tracks into the event
      //
      virtual void insertTracksIntoFrame(Frame& theFrame, int diagLevel = 0) 
	 const = 0;

      // ---------- const member functions ---------------------
      

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      TrackFinder( const TrackFinder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TrackFinder& operator=( const TrackFinder& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_continueIfHitsMissing ;
      DABoolean m_continueIfAnyException ;

      // ---------- static data members ------------------------
};


// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "TrackFinder/Template/TrackFinder.cc"
//#endif

#endif /* TRACKFINDER_TRACKFINDER_H */
