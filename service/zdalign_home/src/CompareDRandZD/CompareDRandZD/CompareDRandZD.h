// -*- C++ -*-
#if !defined(COMPAREDRANDZD_COMPAREDRANDZD_H)
#define COMPAREDRANDZD_COMPAREDRANDZD_H
//
// Package:     <CompareDRandZD>
// Module:      CompareDRandZD
//
/**\class CompareDRandZD CompareDRandZD.h CompareDRandZD/CompareDRandZD.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 10:45:18 EDT 2003
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
#include "DataHandler/Frame.h"

// forward declarations

class CompareDRandZD : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kCurv,
	 kD,
	 kPhi,
	 kCt,
	 kZ,
	 kDelD,
	 kDelPhi,
	 kDelCt,
	 kDelZ,

	 kNumEntries
      };

      enum {
	 kLayer,
	 kWire,
	 kZHit,
	 kPhiHit,
	 kDrift,
	 kDCA,
	 kResidual,

	 kNumHitEntries
      };

      // ------------ Constructors and destructor ----------------
      CompareDRandZD( void );                      // anal1 
      virtual ~CompareDRandZD();                   // anal5 

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

      DABoolean notbhabha(Frame& iFrame);

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      CompareDRandZD( const CompareDRandZD& );

      // ------------ assignment operator(s) ---------------------
      const CompareDRandZD& operator=( const CompareDRandZD& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (CompareDRandZD::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist1D*   m_delphi0;
      HIHistProf* m_deld0_vphi0;
      HIHist1D*   m_deld0;
      HIHist1D*   m_delz0;
      HIHistProf* m_delct_vphi0;
      HIHist1D*   m_delct;

      HIHist1D*   m_drhits;
      HIHist1D*   m_zdhits;

      HIHistProf* m_resid_v_phi_1;
      HIHistProf* m_resid_v_phi_2;
      HIHistProf* m_resid_v_phi_3;
      HIHistProf* m_resid_v_phi_4;
      HIHistProf* m_resid_v_phi_5;
      HIHistProf* m_resid_v_phi_6;

      HIHistProf* m_resid_v_phi_uplus;
      HIHistProf* m_resid_v_phi_vplus;
      HIHistProf* m_resid_v_phi_uminus;
      HIHistProf* m_resid_v_phi_vminus;

      HINtuple* m_ntuple;
      HINtuple* m_hitntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* COMPAREDRANDZD_COMPAREDRANDZD_H */
