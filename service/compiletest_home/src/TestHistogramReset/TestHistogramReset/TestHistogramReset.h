// -*- C++ -*-
#if !defined(TESTHISTOGRAMRESET_TESTHISTOGRAMRESET_H)
#define TESTHISTOGRAMRESET_TESTHISTOGRAMRESET_H
//
// Package:     <TestHistogramReset>
// Module:      TestHistogramReset
//
/**\class TestHistogramReset TestHistogramReset.h TestHistogramReset/TestHistogramReset.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Oct 31 15:30:24 EST 2003
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

class TestHistogramReset : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      TestHistogramReset( void );                      // anal1 
      virtual ~TestHistogramReset();                   // anal5 

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
      TestHistogramReset( const TestHistogramReset& );

      // ------------ assignment operator(s) ---------------------
      const TestHistogramReset& operator=( const TestHistogramReset& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (TestHistogramReset::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist1D* m_hist1d;
      HIHistProf* m_histprof;
      HIHist2D* m_hist2d;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* TESTHISTOGRAMRESET_TESTHISTOGRAMRESET_H */
