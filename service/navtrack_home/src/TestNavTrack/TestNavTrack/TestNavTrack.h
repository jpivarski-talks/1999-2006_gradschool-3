// -*- C++ -*-
#if !defined(TESTNAVTRACK_TESTNAVTRACK_H)
#define TESTNAVTRACK_TESTNAVTRACK_H
//
// Package:     <TestNavTrack>
// Module:      TestNavTrack
//
/**\class TestNavTrack TestNavTrack.h TestNavTrack/TestNavTrack.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Oct  8 16:50:24 EDT 2003
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

class TestNavTrack : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      TestNavTrack( void );                      // anal1 
      virtual ~TestNavTrack();                   // anal5 

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
      TestNavTrack( const TestNavTrack& );

      // ------------ assignment operator(s) ---------------------
      const TestNavTrack& operator=( const TestNavTrack& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (TestNavTrack::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------
      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* TESTNAVTRACK_TESTNAVTRACK_H */
