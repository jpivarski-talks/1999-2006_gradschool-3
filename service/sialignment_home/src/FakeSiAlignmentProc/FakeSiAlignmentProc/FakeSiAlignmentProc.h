// -*- C++ -*-
#if !defined(FAKESIALIGNMENTPROC_FAKESIALIGNMENTPROC_H)
#define FAKESIALIGNMENTPROC_FAKESIALIGNMENTPROC_H
//
// Package:     <FakeSiAlignmentProc>
// Module:      FakeSiAlignmentProc
//
/**\class FakeSiAlignmentProc FakeSiAlignmentProc.h FakeSiAlignmentProc/FakeSiAlignmentProc.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri Feb 22 09:13:04 EST 2002
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

class FakeSiAlignmentProc : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      FakeSiAlignmentProc( void );                      // anal1 
      virtual ~FakeSiAlignmentProc();                   // anal5 

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
      FakeSiAlignmentProc( const FakeSiAlignmentProc& );

      // ------------ assignment operator(s) ---------------------
      const FakeSiAlignmentProc& operator=( const FakeSiAlignmentProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (FakeSiAlignmentProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      double my_abs( double x );

      // ------------ data members -------------------------------

      int m_tracks;
      int m_rphi;
      int m_z;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* FAKESIALIGNMENTPROC_FAKESIALIGNMENTPROC_H */
