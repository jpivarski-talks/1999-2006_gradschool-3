// -*- C++ -*-
#if !defined(DRWIGGLE_DRWIGGLE_H)
#define DRWIGGLE_DRWIGGLE_H
//
// Package:     <DRWiggle>
// Module:      DRWiggle
//
/**\class DRWiggle DRWiggle.h DRWiggle/DRWiggle.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Nov 19 16:18:56 EST 2002
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
#include "CommandPattern/Parameter.h"

#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiSensorInfo.h"

#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

#include "HelixIntersection/HIHelix.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HISiSensorPlane.h"
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HICompositeSurface.h"

#include "KinematicTrajectory/KTHelix.h"
#include "KinematicTrajectory/KTKinematicData.h"

// forward declarations

class DRWiggle : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      DRWiggle( void );                      // anal1 
      virtual ~DRWiggle();                   // anal5 

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
      DRWiggle( const DRWiggle& );

      // ------------ assignment operator(s) ---------------------
      const DRWiggle& operator=( const DRWiggle& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (DRWiggle::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter< string > m_trackUse;
      Parameter< double > m_crossingAngle;
      Parameter< DABoolean > m_getDuals;
      Parameter< DABoolean > m_getResiduals;

      HIHist1D*   m_d0;
      HIHistProf* m_d0_v_phi0;
      HIHist1D*   m_z0;
      HIHist1D*   m_curvSum;
      HIHist1D*   m_sinPhi0Diff;
      HIHist1D*   m_d0Sum;
      HIHist1D*   m_cotThetaSum;
      HIHist1D*   m_z0Diff;
      HIHist1D*   m_dual_curvSum;
      HIHist1D*   m_dual_sinPhi0Diff;
      HIHist1D*   m_dual_d0Sum;
      HIHist1D*   m_dual_cotThetaSum;
      HIHist1D*   m_dual_z0Diff;

      HIHistProf* m_d0missVSphi0;
      HIHist2D*   m_momentumXY;
      HIHistProf* m_momentumMagVSphi0;
      HIHistProf* m_momentumAngVSphi0;
      HIHist2D*   m_momentumAngVSphi0_2d;

      HIHistProf* m_rphi_vphi_bac_end_pos;
      HIHistProf* m_rphi_vphi_bac_mid_pos;
      HIHistProf* m_rphi_vphi_bac_barrel_pos;
      HIHistProf* m_rphi_vphi_for_barrel_pos;
      HIHistProf* m_rphi_vphi_for_mid_pos;
      HIHistProf* m_rphi_vphi_for_end_pos;

      HIHistProf* m_z_vphi_bac_end_pos;
      HIHistProf* m_z_vphi_bac_mid_pos;
      HIHistProf* m_z_vphi_bac_barrel_pos;
      HIHistProf* m_z_vphi_for_barrel_pos;
      HIHistProf* m_z_vphi_for_mid_pos;
      HIHistProf* m_z_vphi_for_end_pos;

      HIHistProf* m_rphi_vcotTh_pos;
      HIHistProf* m_rphi_vcotTh_pos_left;
      HIHistProf* m_rphi_vcotTh_pos_top;
      HIHistProf* m_rphi_vcotTh_pos_right;
      HIHistProf* m_rphi_vcotTh_pos_bottom;
      HIHistProf* m_z_vcotTh_pos;
      HIHistProf* m_z_vcotTh_pos_left;
      HIHistProf* m_z_vcotTh_pos_top;
      HIHistProf* m_z_vcotTh_pos_right;
      HIHistProf* m_z_vcotTh_pos_bottom;

      HIHistProf* m_rphi_vphi_bac_end_neg;
      HIHistProf* m_rphi_vphi_bac_mid_neg;
      HIHistProf* m_rphi_vphi_bac_barrel_neg;
      HIHistProf* m_rphi_vphi_for_barrel_neg;
      HIHistProf* m_rphi_vphi_for_mid_neg;
      HIHistProf* m_rphi_vphi_for_end_neg;

      HIHistProf* m_z_vphi_bac_end_neg;
      HIHistProf* m_z_vphi_bac_mid_neg;
      HIHistProf* m_z_vphi_bac_barrel_neg;
      HIHistProf* m_z_vphi_for_barrel_neg;
      HIHistProf* m_z_vphi_for_mid_neg;
      HIHistProf* m_z_vphi_for_end_neg;

      HIHistProf* m_rphi_vcotTh_neg;
      HIHistProf* m_rphi_vcotTh_neg_left;
      HIHistProf* m_rphi_vcotTh_neg_top;
      HIHistProf* m_rphi_vcotTh_neg_right;
      HIHistProf* m_rphi_vcotTh_neg_bottom;
      HIHistProf* m_z_vcotTh_neg;
      HIHistProf* m_z_vcotTh_neg_left;
      HIHistProf* m_z_vcotTh_neg_top;
      HIHistProf* m_z_vcotTh_neg_right;
      HIHistProf* m_z_vcotTh_neg_bottom;

      HIHistProf* m_dr17_vcotTh_pos;
      HIHistProf* m_dr18_vcotTh_pos;
      HIHistProf* m_dr19_vcotTh_pos;
      HIHistProf* m_dr20_vcotTh_pos;
      HIHistProf* m_dr21_vcotTh_pos;
      HIHistProf* m_dr22_vcotTh_pos;
      HIHistProf* m_dr23_vcotTh_pos;
      HIHistProf* m_dr24_vcotTh_pos;
      HIHistProf* m_dr25_vcotTh_pos;
      HIHistProf* m_dr26_vcotTh_pos;
      HIHistProf* m_dr27_vcotTh_pos;
      HIHistProf* m_dr28_vcotTh_pos;
      HIHistProf* m_dr29_vcotTh_pos;
      HIHistProf* m_dr30_vcotTh_pos;
      HIHistProf* m_dr31_vcotTh_pos;
      HIHistProf* m_dr32_vcotTh_pos;
      HIHistProf* m_dr33_vcotTh_pos;
      HIHistProf* m_dr34_vcotTh_pos;
      HIHistProf* m_dr35_vcotTh_pos;
      HIHistProf* m_dr36_vcotTh_pos;
      HIHistProf* m_dr37_vcotTh_pos;
      HIHistProf* m_dr38_vcotTh_pos;
      HIHistProf* m_dr39_vcotTh_pos;
      HIHistProf* m_dr40_vcotTh_pos;
      HIHistProf* m_dr41_vcotTh_pos;
      HIHistProf* m_dr42_vcotTh_pos;
      HIHistProf* m_dr43_vcotTh_pos;
      HIHistProf* m_dr44_vcotTh_pos;
      HIHistProf* m_dr45_vcotTh_pos;
      HIHistProf* m_dr46_vcotTh_pos;
      HIHistProf* m_dr47_vcotTh_pos;

      HIHistProf* m_dr17_vcotTh_neg;
      HIHistProf* m_dr18_vcotTh_neg;
      HIHistProf* m_dr19_vcotTh_neg;
      HIHistProf* m_dr20_vcotTh_neg;
      HIHistProf* m_dr21_vcotTh_neg;
      HIHistProf* m_dr22_vcotTh_neg;
      HIHistProf* m_dr23_vcotTh_neg;
      HIHistProf* m_dr24_vcotTh_neg;
      HIHistProf* m_dr25_vcotTh_neg;
      HIHistProf* m_dr26_vcotTh_neg;
      HIHistProf* m_dr27_vcotTh_neg;
      HIHistProf* m_dr28_vcotTh_neg;
      HIHistProf* m_dr29_vcotTh_neg;
      HIHistProf* m_dr30_vcotTh_neg;
      HIHistProf* m_dr31_vcotTh_neg;
      HIHistProf* m_dr32_vcotTh_neg;
      HIHistProf* m_dr33_vcotTh_neg;
      HIHistProf* m_dr34_vcotTh_neg;
      HIHistProf* m_dr35_vcotTh_neg;
      HIHistProf* m_dr36_vcotTh_neg;
      HIHistProf* m_dr37_vcotTh_neg;
      HIHistProf* m_dr38_vcotTh_neg;
      HIHistProf* m_dr39_vcotTh_neg;
      HIHistProf* m_dr40_vcotTh_neg;
      HIHistProf* m_dr41_vcotTh_neg;
      HIHistProf* m_dr42_vcotTh_neg;
      HIHistProf* m_dr43_vcotTh_neg;
      HIHistProf* m_dr44_vcotTh_neg;
      HIHistProf* m_dr45_vcotTh_neg;
      HIHistProf* m_dr46_vcotTh_neg;
      HIHistProf* m_dr47_vcotTh_neg;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* DRWIGGLE_DRWIGGLE_H */
