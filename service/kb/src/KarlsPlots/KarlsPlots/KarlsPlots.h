// -*- C++ -*-
#if !defined(KARLSPLOTS_KARLSPLOTS_H)
#define KARLSPLOTS_KARLSPLOTS_H
//
// Package:     <KarlsPlots>
// Module:      KarlsPlots
//
/**\class KarlsPlots KarlsPlots.h KarlsPlots/KarlsPlots.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Jan 28 14:30:53 EST 2003
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

class KarlsPlots : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      KarlsPlots( void );                      // anal1 
      virtual ~KarlsPlots();                   // anal5 

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
      KarlsPlots( const KarlsPlots& );

      // ------------ assignment operator(s) ---------------------
      const KarlsPlots& operator=( const KarlsPlots& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (KarlsPlots::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      // declare histogram variables
      HIHist1D*   m_p;
      HIHist1D*   m_pz;
      HIHist1D*   m_pperp;
      HIHist1D*   m_pcosth;
      HIHist2D*   m_p_pcosth;
      HIHistProf* m_p_pcosth_prof;

      HIHist1D*   m_e;
      HIHist2D*   m_e_ecosth;
      HIHistProf* m_e_ecosth_prof;

      HIHist1D*   m_total_mult;
      HIHist1D*   m_charged_mult;
      HIHist1D*   m_neutral_mult;
      HIHist1D*   m_total_energy;
      HIHist1D*   m_charged_energy;
      HIHist1D*   m_neutral_energy;
      HIHist1D*   m_big_hemisphere;
      HIHist1D*   m_little_hemisphere;
      HIHist1D*   m_total_scalarsum_pt;
      HIHist1D*   m_charged_scalarsum_pt;
      HIHist1D*   m_neutral_scalarsum_pt;
      HIHist1D*   m_track_pmag;
      HIHist1D*   m_track_pt;
      HIHist1D*   m_track_pz;
      HIHist1D*   m_shower_pmag;
      HIHist1D*   m_shower_pt;
      HIHist1D*   m_shower_pz;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* KARLSPLOTS_KARLSPLOTS_H */
