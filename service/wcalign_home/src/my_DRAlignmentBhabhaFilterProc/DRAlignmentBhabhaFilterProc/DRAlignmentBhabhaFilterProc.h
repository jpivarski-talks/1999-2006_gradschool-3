// -*- C++ -*-
#if !defined(DRALIGNMENTBHABHAFILTERPROC_DRALIGNMENTBHABHAFILTERPROC_H)
#define DRALIGNMENTBHABHAFILTERPROC_DRALIGNMENTBHABHAFILTERPROC_H
//
// Package:     <DRAlignmentBhabhaFilterProc>
// Module:      DRAlignmentBhabhaFilterProc
//
/**\class DRAlignmentBhabhaFilterProc DRAlignmentBhabhaFilterProc.h DRAlignmentBhabhaFilterProc/DRAlignmentBhabhaFilterProc.h
 
 Description: Selects bhabha events for DRAlignmentMod

 Usage:
    see Test/sample.tcl

*/
//
// Author:      Jim Pivarski
// Created:     Fri Dec  5 12:08:40 EST 2003
// $Id: DRAlignmentBhabhaFilterProc.h,v 1.1.1.1 2003/12/05 17:54:58 mccann Exp $
//
// Revision history
//
// $Log: DRAlignmentBhabhaFilterProc.h,v $
// Revision 1.1.1.1  2003/12/05 17:54:58  mccann
// imported DRAlignmentBhabhaFilterProc sources
//
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

// forward declarations

class DRAlignmentBhabhaFilterProc : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      DRAlignmentBhabhaFilterProc( void );                      // anal1 
      virtual ~DRAlignmentBhabhaFilterProc();                   // anal5 

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
      DRAlignmentBhabhaFilterProc( const DRAlignmentBhabhaFilterProc& );

      // ------------ assignment operator(s) ---------------------
      const DRAlignmentBhabhaFilterProc& operator=( const DRAlignmentBhabhaFilterProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (DRAlignmentBhabhaFilterProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter<double> m_crossingangle_x;
      Parameter<double> m_crossingangle_y;
      Parameter<double> m_asymmetric_beamz;

      HIHist1D* m_good_tracks;
      HIHist1D* m_bad_tracks;
      HIHist1D* m_charge;
      HIHist1D* m_d0miss;
      HIHist1D* m_z0miss;
      HIHist1D* m_tracks_momentum_phi;
      HIHist1D* m_tracks_momentum_perp;
      HIHist1D* m_tracks_momentum_z;

      HIHist1D* m_pertrack_fit;
      HIHist1D* m_pertrack_reduced_chi2;
      HIHist1D* m_pertrack_hitfraction;
      HIHist1D* m_pertrack_d0;
      HIHist1D* m_pertrack_z0;
      HIHist1D* m_pertrack_p;
      HIHist1D* m_pertrack_cotTheta;
      HIHist1D* m_pertrack_err_cT;
      HIHist1D* m_pertrack_err_z0;
      HIHist1D* m_pertrack_numdrhits;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* DRALIGNMENTBHABHAFILTERPROC_DRALIGNMENTBHABHAFILTERPROC_H */
