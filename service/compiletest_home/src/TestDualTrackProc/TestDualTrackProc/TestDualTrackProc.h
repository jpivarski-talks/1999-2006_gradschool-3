// -*- C++ -*-
#if !defined(TESTDUALTRACKPROC_TESTDUALTRACKPROC_H)
#define TESTDUALTRACKPROC_TESTDUALTRACKPROC_H
//
// Package:     <TestDualTrackProc>
// Module:      TestDualTrackProc
//
/**\class TestDualTrackProc TestDualTrackProc.h TestDualTrackProc/TestDualTrackProc.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 17:15:21 EST 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "HistogramInterface/HINtuple.h"

// forward declarations

class TestDualTrackProc : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kId,
	 kOldCurv,
	 kOldPhi0,
	 kOldD0,
	 kOldCotTh,
	 kOldZ0,
	 kCurv,
	 kPhi0,
	 kD0,
	 kCotTh,
	 kZ0,
	 kResid,
	 kResidErr,
	 kP,
	 kPointX,
	 kPointY,
	 kPointZ,
	 kDca,
	 kDcaErr,
	 kSinAlph,
	 kTime,
	 kFW,
	 kDrift,
	 kOldResid,
	 kOldResidErr,
	 kOldP,
	 kOldPointX,
	 kOldPointY,
	 kOldPointZ,
	 kOldDca,
	 kOldDcaErr,
	 kOldSinAlph,

	 kNumEntries
      };

      // ------------ Constructors and destructor ----------------
      TestDualTrackProc( void );                      // anal1 
      virtual ~TestDualTrackProc();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
      //virtual ActionBase::ActionResult beginRun( Frame& iFrame);
      //virtual ActionBase::ActionResult endRun( Frame& iFrame);

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      TestDualTrackProc( const TestDualTrackProc& );

      // ------------ assignment operator(s) ---------------------
      const TestDualTrackProc& operator=( const TestDualTrackProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (TestDualTrackProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      int m_id;
      HINtuple* m_ntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* TESTDUALTRACKPROC_TESTDUALTRACKPROC_H */
