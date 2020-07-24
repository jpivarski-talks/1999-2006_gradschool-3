// -*- C++ -*-
#if !defined(WHERESTHEBEAMSPOT_WHERESTHEBEAMSPOT_H)
#define WHERESTHEBEAMSPOT_WHERESTHEBEAMSPOT_H
//
// Package:     <WheresTheBeamspot>
// Module:      WheresTheBeamspot
//
/**\class WheresTheBeamspot WheresTheBeamspot.h WheresTheBeamspot/WheresTheBeamspot.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Mar 19 17:15:15 EST 2003
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

class WheresTheBeamspot : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      WheresTheBeamspot( void );                      // anal1 
      virtual ~WheresTheBeamspot();                   // anal5 

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
      WheresTheBeamspot( const WheresTheBeamspot& );

      // ------------ assignment operator(s) ---------------------
      const WheresTheBeamspot& operator=( const WheresTheBeamspot& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (WheresTheBeamspot::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      void calc_terms(int run, HIHistProf* normal, HIHistProf* shifted, int n);

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      int         m_run;
      double      m_sum_z0;
      double      m_sum_shifted_z0;
      double      m_sqrsum_z0;
      double      m_sqrsum_shifted_z0;
      int         m_tracks;
      int         m_shifted_tracks;

      double      m_indiv_sum_z0;
      double      m_indiv_sum_shifted_z0;
      double      m_indiv_sqrsum_z0;
      double      m_indiv_sqrsum_shifted_z0;
      int         m_indiv_tracks;
      int         m_indiv_shifted_tracks;

      HIHist2D*   m_d0VSphi;
      HIHistProf* m_d0VSphi_prof;
      HIHist1D*   m_z0;
      HIHist2D*   m_shifted_d0VSphi;
      HIHistProf* m_shifted_d0VSphi_prof;
      HIHist1D*   m_shifted_z0;

      HIHistProf* m_indiv_d0VSphi;
      HIHistProf* m_indiv_shifted_d0VSphi;
      HIHist1D*   m_indiv_z0;
      HIHist1D*   m_indiv_shifted_z0;

      HIHistoManager* m_hm;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* WHERESTHEBEAMSPOT_WHERESTHEBEAMSPOT_H */
