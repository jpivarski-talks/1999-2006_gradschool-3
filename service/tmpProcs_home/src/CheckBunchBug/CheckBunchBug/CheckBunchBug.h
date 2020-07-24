// -*- C++ -*-
#if !defined(CHECKBUNCHBUG_CHECKBUNCHBUG_H)
#define CHECKBUNCHBUG_CHECKBUNCHBUG_H
//
// Package:     <CheckBunchBug>
// Module:      CheckBunchBug
//
/**\class CheckBunchBug CheckBunchBug.h CheckBunchBug/CheckBunchBug.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri May  7 13:33:13 EDT 2004
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

class CheckBunchBug : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      CheckBunchBug( void );                      // anal1 
      virtual ~CheckBunchBug();                   // anal5 

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
      CheckBunchBug( const CheckBunchBug& );

      // ------------ assignment operator(s) ---------------------
      const CheckBunchBug& operator=( const CheckBunchBug& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (CheckBunchBug::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      void summarize();

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist1D* m_RedChi;
      HIHist1D* m_ErrCotth;
      HIHist1D* m_ErrZ0;
      HIHist1D* m_RedChi2;

      int m_run;
      double m_sumRedChi;
      double m_sumcutRedChi;
      int m_tailRedChi;
      int m_totalRedChi;
      double m_sumErrCotth;
      double m_sumcutErrCotth;
      int m_tailErrCotth;
      int m_totalErrCotth;
      double m_sumErrZ0;
      double m_sumcutErrZ0;
      int m_tailErrZ0;
      int m_totalErrZ0;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* CHECKBUNCHBUG_CHECKBUNCHBUG_H */
