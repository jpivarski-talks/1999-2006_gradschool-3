// -*- C++ -*-
#if !defined(SQUISHCHECKER_SQUISHCHECKER_H)
#define SQUISHCHECKER_SQUISHCHECKER_H
//
// Package:     <SquishChecker>
// Module:      SquishChecker
//
/**\class SquishChecker SquishChecker.h SquishChecker/SquishChecker.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Feb  4 10:10:07 EST 2003
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

class SquishChecker : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      SquishChecker( void );                      // anal1 
      virtual ~SquishChecker();                   // anal5 

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
      SquishChecker( const SquishChecker& );

      // ------------ assignment operator(s) ---------------------
      const SquishChecker& operator=( const SquishChecker& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (SquishChecker::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      DABoolean arclength_test( double x, double y, KTHelix& a, KTHelix& b );
      DABoolean calc_intersection( double& x, double& y, KTHelix& a, KTHelix& b );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter<double> m_crossingAngle;
      Parameter<DABoolean> m_requireZInfo;
      vector<KTHelix> m_positives;
      vector<KTHelix> m_negatives;

      HIHist1D*   m_curvmiss;
      HIHist1D*   m_z0miss;

      HIHist2D*   m_all_intersections;
      HIHist2D*   m_positives_intersections;
      HIHist2D*   m_negatives_intersections;

      HIHist2D*   m_d0miss;
      HIHistProf* m_d0miss_prof;
      HIHist2D*   m_d0miss_midtheta;
      HIHistProf* m_d0miss_midtheta_prof;
      HIHist2D*   m_d0miss_fartheta;
      HIHistProf* m_d0miss_fartheta_prof;
      HIHist2D*   m_d0miss1;
      HIHistProf* m_d0miss1_prof;
      HIHist2D*   m_pmag;
      HIHistProf* m_pmag_prof;
      HIHist2D*   m_pmag1;
      HIHistProf* m_pmag1_prof;
      HIHist2D*   m_phi0miss;
      HIHistProf* m_phi0miss_prof;
      HIHist2D*   m_phi0miss1;
      HIHistProf* m_phi0miss1_prof;

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

#endif /* SQUISHCHECKER_SQUISHCHECKER_H */
