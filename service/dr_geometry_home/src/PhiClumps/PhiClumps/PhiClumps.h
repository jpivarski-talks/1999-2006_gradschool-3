// -*- C++ -*-
#if !defined(PHICLUMPS_PHICLUMPS_H)
#define PHICLUMPS_PHICLUMPS_H
//
// Package:     <PhiClumps>
// Module:      PhiClumps
//
/**\class PhiClumps PhiClumps.h PhiClumps/PhiClumps.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Sep  3 15:07:59 EDT 2003
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

// forward declarations

class PhiClumps : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      PhiClumps( void );                      // anal1 
      virtual ~PhiClumps();                   // anal5 

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
      PhiClumps( const PhiClumps& );

      // ------------ assignment operator(s) ---------------------
      const PhiClumps& operator=( const PhiClumps& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (PhiClumps::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_ntuple;

      HIHist2D* m_wirenumber;
      HIHist2D* m_wirenumber_dca;
      HIHist2D* m_wirenumber_dca2;
      HIHist2D* m_wirenumber_drift;
      HIHist2D* m_wirenumber_drift2;
      HIHist2D* m_wirenumber_sresid;
      HIHist2D* m_wirenumber_sresid2;
      HIHist2D* m_wirenumber_tresid;
      HIHist2D* m_wirenumber_tresid2;

      HIHist2D* m_xy;
      HIHist2D* m_xy_dca;
      HIHist2D* m_xy_dca2;
      HIHist2D* m_xy_drift;
      HIHist2D* m_xy_drift2;
      HIHist2D* m_xy_sresid;
      HIHist2D* m_xy_sresid2;
      HIHist2D* m_xy_tresid;
      HIHist2D* m_xy_tresid2;

      HIHist1D* m_trackphi0;
      HIHist1D* m_trackphi;
      HIHist2D* m_trackphis;

      HIHist2D* m_notrack_wirenumber;
      HIHist2D* m_notrack_wirenumber_drift;
      HIHist2D* m_notrack_wirenumber_drift2;

      HIHistProf* m_dca_vphi_s1;
      HIHistProf* m_dca_vphi_s2;
      HIHistProf* m_dca_vphi_s3;
      HIHistProf* m_dca_vphi_s4;
      HIHistProf* m_dca_vphi_l1;
      HIHistProf* m_dca_vphi_l2;
      HIHistProf* m_dca_vphi_l3;
      HIHistProf* m_dca_vphi_l4;

      HIHistProf* m_drift_vphi_s1;
      HIHistProf* m_drift_vphi_s2;
      HIHistProf* m_drift_vphi_s3;
      HIHistProf* m_drift_vphi_s4;
      HIHistProf* m_drift_vphi_l1;
      HIHistProf* m_drift_vphi_l2;
      HIHistProf* m_drift_vphi_l3;
      HIHistProf* m_drift_vphi_l4;

      HIHistProf* m_sresid_vphi_s1;
      HIHistProf* m_sresid_vphi_s2;
      HIHistProf* m_sresid_vphi_s3;
      HIHistProf* m_sresid_vphi_s4;
      HIHistProf* m_sresid_vphi_l1;
      HIHistProf* m_sresid_vphi_l2;
      HIHistProf* m_sresid_vphi_l3;
      HIHistProf* m_sresid_vphi_l4;

      HIHistProf* m_tresid_vphi_s1;
      HIHistProf* m_tresid_vphi_s2;
      HIHistProf* m_tresid_vphi_s3;
      HIHistProf* m_tresid_vphi_s4;
      HIHistProf* m_tresid_vphi_l1;
      HIHistProf* m_tresid_vphi_l2;
      HIHistProf* m_tresid_vphi_l3;
      HIHistProf* m_tresid_vphi_l4;

      HIHistProf* m_dca_vphi_s1_spread;
      HIHistProf* m_dca_vphi_s2_spread;
      HIHistProf* m_dca_vphi_s3_spread;
      HIHistProf* m_dca_vphi_s4_spread;
      HIHistProf* m_dca_vphi_l1_spread;
      HIHistProf* m_dca_vphi_l2_spread;
      HIHistProf* m_dca_vphi_l3_spread;
      HIHistProf* m_dca_vphi_l4_spread;

      HIHistProf* m_drift_vphi_s1_spread;
      HIHistProf* m_drift_vphi_s2_spread;
      HIHistProf* m_drift_vphi_s3_spread;
      HIHistProf* m_drift_vphi_s4_spread;
      HIHistProf* m_drift_vphi_l1_spread;
      HIHistProf* m_drift_vphi_l2_spread;
      HIHistProf* m_drift_vphi_l3_spread;
      HIHistProf* m_drift_vphi_l4_spread;

      HIHistProf* m_sresid_vphi_s1_spread;
      HIHistProf* m_sresid_vphi_s2_spread;
      HIHistProf* m_sresid_vphi_s3_spread;
      HIHistProf* m_sresid_vphi_s4_spread;
      HIHistProf* m_sresid_vphi_l1_spread;
      HIHistProf* m_sresid_vphi_l2_spread;
      HIHistProf* m_sresid_vphi_l3_spread;
      HIHistProf* m_sresid_vphi_l4_spread;

      HIHistProf* m_tresid_vphi_s1_spread;
      HIHistProf* m_tresid_vphi_s2_spread;
      HIHistProf* m_tresid_vphi_s3_spread;
      HIHistProf* m_tresid_vphi_s4_spread;
      HIHistProf* m_tresid_vphi_l1_spread;
      HIHistProf* m_tresid_vphi_l2_spread;
      HIHistProf* m_tresid_vphi_l3_spread;
      HIHistProf* m_tresid_vphi_l4_spread;

      HIHist1D* m_dca_hiphi_s1;
      HIHist1D* m_dca_hiphi_s2;
      HIHist1D* m_dca_hiphi_s3;
      HIHist1D* m_dca_hiphi_s4;
      HIHist1D* m_dca_hiphi_l1;
      HIHist1D* m_dca_hiphi_l2;
      HIHist1D* m_dca_hiphi_l3;
      HIHist1D* m_dca_hiphi_l4;

      HIHist1D* m_drift_hiphi_s1;
      HIHist1D* m_drift_hiphi_s2;
      HIHist1D* m_drift_hiphi_s3;
      HIHist1D* m_drift_hiphi_s4;
      HIHist1D* m_drift_hiphi_l1;
      HIHist1D* m_drift_hiphi_l2;
      HIHist1D* m_drift_hiphi_l3;
      HIHist1D* m_drift_hiphi_l4;

      HIHist1D* m_sresid_hiphi_s1;
      HIHist1D* m_sresid_hiphi_s2;
      HIHist1D* m_sresid_hiphi_s3;
      HIHist1D* m_sresid_hiphi_s4;
      HIHist1D* m_sresid_hiphi_l1;
      HIHist1D* m_sresid_hiphi_l2;
      HIHist1D* m_sresid_hiphi_l3;
      HIHist1D* m_sresid_hiphi_l4;

      HIHist1D* m_tresid_hiphi_s1;
      HIHist1D* m_tresid_hiphi_s2;
      HIHist1D* m_tresid_hiphi_s3;
      HIHist1D* m_tresid_hiphi_s4;
      HIHist1D* m_tresid_hiphi_l1;
      HIHist1D* m_tresid_hiphi_l2;
      HIHist1D* m_tresid_hiphi_l3;
      HIHist1D* m_tresid_hiphi_l4;

      HIHist1D* m_dca_lophi_s1;
      HIHist1D* m_dca_lophi_s2;
      HIHist1D* m_dca_lophi_s3;
      HIHist1D* m_dca_lophi_s4;
      HIHist1D* m_dca_lophi_l1;
      HIHist1D* m_dca_lophi_l2;
      HIHist1D* m_dca_lophi_l3;
      HIHist1D* m_dca_lophi_l4;

      HIHist1D* m_drift_lophi_s1;
      HIHist1D* m_drift_lophi_s2;
      HIHist1D* m_drift_lophi_s3;
      HIHist1D* m_drift_lophi_s4;
      HIHist1D* m_drift_lophi_l1;
      HIHist1D* m_drift_lophi_l2;
      HIHist1D* m_drift_lophi_l3;
      HIHist1D* m_drift_lophi_l4;

      HIHist1D* m_sresid_lophi_s1;
      HIHist1D* m_sresid_lophi_s2;
      HIHist1D* m_sresid_lophi_s3;
      HIHist1D* m_sresid_lophi_s4;
      HIHist1D* m_sresid_lophi_l1;
      HIHist1D* m_sresid_lophi_l2;
      HIHist1D* m_sresid_lophi_l3;
      HIHist1D* m_sresid_lophi_l4;

      HIHist1D* m_tresid_lophi_s1;
      HIHist1D* m_tresid_lophi_s2;
      HIHist1D* m_tresid_lophi_s3;
      HIHist1D* m_tresid_lophi_s4;
      HIHist1D* m_tresid_lophi_l1;
      HIHist1D* m_tresid_lophi_l2;
      HIHist1D* m_tresid_lophi_l3;
      HIHist1D* m_tresid_lophi_l4;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* PHICLUMPS_PHICLUMPS_H */
