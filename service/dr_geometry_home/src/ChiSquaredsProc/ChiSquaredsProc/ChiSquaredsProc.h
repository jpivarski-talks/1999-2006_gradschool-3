// -*- C++ -*-
#if !defined(CHISQUAREDSPROC_CHISQUAREDSPROC_H)
#define CHISQUAREDSPROC_CHISQUAREDSPROC_H
//
// Package:     <ChiSquaredsProc>
// Module:      ChiSquaredsProc
//
/**\class ChiSquaredsProc ChiSquaredsProc.h ChiSquaredsProc/ChiSquaredsProc.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Oct 20 10:40:38 EDT 2003
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

class ChiSquaredsProc : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      ChiSquaredsProc( void );                      // anal1 
      virtual ~ChiSquaredsProc();                   // anal5 

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
      ChiSquaredsProc( const ChiSquaredsProc& );

      // ------------ assignment operator(s) ---------------------
      const ChiSquaredsProc& operator=( const ChiSquaredsProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (ChiSquaredsProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist1D* m_chi2pos;
      HIHist1D* m_chi2neg;
      HIHistProf* m_chi2pos_vcotTh;
      HIHistProf* m_chi2neg_vcotTh;
      HIHistProf* m_chi2pos_vcotTh_spread;
      HIHistProf* m_chi2neg_vcotTh_spread;
      HIHist1D* m_probpos;
      HIHist1D* m_probneg;
      HIHist1D* m_pperppos;
      HIHist1D* m_pperpneg;
      HIHistProf* m_pperppos_vcotTh;
      HIHistProf* m_pperpneg_vcotTh;
      HIHistProf* m_pperppos_vcotTh_spread;
      HIHistProf* m_pperpneg_vcotTh_spread;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* CHISQUAREDSPROC_CHISQUAREDSPROC_H */
