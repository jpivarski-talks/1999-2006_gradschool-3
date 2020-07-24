// -*- C++ -*-
#if !defined(TESTTRACKING_TESTTRACKING_H)
#define TESTTRACKING_TESTTRACKING_H
//
// Package:     <TestTracking>
// Module:      TestTracking
//
/**\class TestTracking TestTracking.h TestTracking/TestTracking.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Jul 21 11:47:49 EDT 2003
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

class TestTracking : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kSeedCurvature,
	 kSeedPhi0,
	 kSeedD0,
	 kSeedCotTheta,
	 kSeedZ0,
	 kPionCurvature,
	 kPionPhi0,
	 kPionD0,
	 kPionCotTheta,
	 kPionZ0,
	 kPionMomentumX,
	 kPionMomentumY,
	 kPionMomentumZ,
	 kChiSquare,
	 kDOF,
	 kHitsExpected,
	 kHitRatio,
	 kHitsInput,
	 kFitAbort,
	 kHitsDropped,
	 kIterations,
	 kL1Wire,
	 kL1Charge,
	 kL1Drift,
	 kL1DriftError,
	 kL1DCA,
	 kL1DCAError,
	 kL1SinAlpha,
	 kL1Used,
	 kL1SeedDrift,
	 kL2Wire,
	 kL2Charge,
	 kL2Drift,
	 kL2DriftError,
	 kL2DCA,
	 kL2DCAError,
	 kL2SinAlpha,
	 kL2Used,
	 kL2SeedDrift,
	 kL3Wire,
	 kL3Charge,
	 kL3Drift,
	 kL3DriftError,
	 kL3DCA,
	 kL3DCAError,
	 kL3SinAlpha,
	 kL3Used,
	 kL3SeedDrift,
	 kL4Wire,
	 kL4Charge,
	 kL4Drift,
	 kL4DriftError,
	 kL4DCA,
	 kL4DCAError,
	 kL4SinAlpha,
	 kL4Used,
	 kL4SeedDrift,
	 kL5Wire,
	 kL5Charge,
	 kL5Drift,
	 kL5DriftError,
	 kL5DCA,
	 kL5DCAError,
	 kL5SinAlpha,
	 kL5Used,
	 kL5SeedDrift,
	 kL6Wire,
	 kL6Charge,
	 kL6Drift,
	 kL6DriftError,
	 kL6DCA,
	 kL6DCAError,
	 kL6SinAlpha,
	 kL6Used,
	 kL6SeedDrift,
	 kMCMomentumX,
	 kMCMomentumY,
	 kMCMomentumZ,

	 kNumEntries
      };

      // ------------ Constructors and destructor ----------------
      TestTracking( void );                      // anal1 
      virtual ~TestTracking();                   // anal5 

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
      TestTracking( const TestTracking& );

      // ------------ assignment operator(s) ---------------------
      const TestTracking& operator=( const TestTracking& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (TestTracking::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_ntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* TESTTRACKING_TESTTRACKING_H */
