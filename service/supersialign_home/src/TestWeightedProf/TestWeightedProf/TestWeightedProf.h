// -*- C++ -*-
#if !defined(TESTWEIGHTEDPROF_TESTWEIGHTEDPROF_H)
#define TESTWEIGHTEDPROF_TESTWEIGHTEDPROF_H
//
// Package:     <TestWeightedProf>
// Module:      TestWeightedProf
//
/**\class TestWeightedProf TestWeightedProf.h TestWeightedProf/TestWeightedProf.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri May  3 11:30:51 EDT 2002
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

// forward declarations

class TestWeightedProf : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      TestWeightedProf( void );                      // anal1 
      virtual ~TestWeightedProf();                   // anal5 

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
      //virtual ActionBase::ActionResult geometry( Frame& iFrame);
      //virtual ActionBase::ActionResult hardware( Frame& iFrame);
      //virtual ActionBase::ActionResult user( Frame& iFrame);

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      TestWeightedProf( const TestWeightedProf& );

      // ------------ assignment operator(s) ---------------------
      const TestWeightedProf& operator=( const TestWeightedProf& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (TestWeightedProf::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHistProf* m_unweighted_err;
      HIHistProf* m_unweighted_spread;
      HIHistProf* m_weighted_err;
      HIHistProf* m_weighted_spread;
      HIHistProf* m_weighted;
      HIHist1D* m_oned;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* TESTWEIGHTEDPROF_TESTWEIGHTEDPROF_H */
