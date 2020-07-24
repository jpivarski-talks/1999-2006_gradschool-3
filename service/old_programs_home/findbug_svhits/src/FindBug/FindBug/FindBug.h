// -*- C++ -*-
#if !defined(FINDBUG_FINDBUG_H)
#define FINDBUG_FINDBUG_H
//
// Package:     <FindBug>
// Module:      FindBug
//
/**\class FindBug FindBug.h FindBug/FindBug.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Tue Nov  6 14:43:29 EST 2001
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
#include "Lattice/Lattice.h"
#include "CommandPattern/Parameter.h"
#include "ASiStorePro/ASiStore.h"
#include "ASiStorePro/ASiSensorInfo.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"
#include "TrackFinder/SeedTrackSVRHitLattice.h"
#include "TrackFinder/SeedTrackSVZHitLattice.h"
#include "HelixIntersection/HIHelix.h"
#include "TrackRoot/TRSeedTrack.h"
#include "TrackRoot/TRHelixMuonFit.h"
#include "TrackRoot/TRTrackMuonFitQuality.h"
#include "KalmanProd/KalmanFitSVRHitLatticeProxy.h"
#include "KalmanProd/KalmanFitSVZHitLatticeProxy.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HICompositeSurface.h"
#include "KinematicTrajectory/KTHelix.h"
#include "KinematicTrajectory/KTKinematicData.h"

// forward declarations

class FindBug : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum NextType {
	 kError,
	 kHit,
	 kTrack,
	 kEvent,
	 kEnd
      };

      // ------------ Constructors and destructor ----------------
      FindBug( void );                      // anal1 
      virtual ~FindBug();                   // anal5 

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
      FindBug( const FindBug& );

      // ------------ assignment operator(s) ---------------------
      const FindBug& operator=( const FindBug& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (FindBug::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist2D* m_both_rphi_residual;
      HIHist2D* m_both_z_residual;
      HIHist1D* m_rphi_difference;
      HIHist1D* m_z_difference;
      HIHist1D* m_rphi_ratio;
      HIHist1D* m_z_ratio;
      HIHistProf* m_rphi_diff_v_phi;
      HIHistProf* m_z_diff_v_phi;
      HIHistProf* m_rphi_diff_v_phi0;
      HIHistProf* m_z_diff_v_phi0;
      HIHistProf* m_rphi_diff_v_cotTheta;
      HIHistProf* m_z_diff_v_cotTheta;

      HIHistProf* m_rphi_ratio_v_cotTheta;
      HIHistProf* m_z_ratio_v_cotTheta;
      HIHistProf* m_rphi_ratio_v_z0;
      HIHistProf* m_z_ratio_v_z0;
      HIHistProf* m_rphi_ratio_v_z;
      HIHistProf* m_rphi_ratio_v_z_layer1;
      HIHistProf* m_rphi_ratio_v_z_layer2;
      HIHistProf* m_rphi_ratio_v_z_layer3;
      HIHistProf* m_rphi_ratio_v_z_layer4;
      HIHistProf* m_z_ratio_v_z;
      HIHistProf* m_z_ratio_v_z_layer1;
      HIHistProf* m_z_ratio_v_z_layer2;
      HIHistProf* m_z_ratio_v_z_layer3;
      HIHistProf* m_z_ratio_v_z_layer4;

      HIHist1D* m_z_atan_ratio;
      HIHistProf* m_z_atan_ratio_v_z;
      HIHistProf* m_z_atan_ratio_v_z_layer1;
      HIHistProf* m_z_atan_ratio_v_z_layer2;
      HIHistProf* m_z_atan_ratio_v_z_layer3;
      HIHistProf* m_z_atan_ratio_v_z_layer4;

      HIHist1D* m_fitter_rphi_residual;
      HIHist1D* m_fitter_z_residual;

      HIHistProf* m_fitter_layer1right1;
      HIHistProf* m_fitter_layer1right1_1;
      HIHistProf* m_fitter_layer1right1_2;
      HIHistProf* m_fitter_layer1right1_3;
      HIHistProf* m_fitter_layer1right1_4;

      HIHistProf* m_fitter_layer1middle;
      HIHistProf* m_fitter_layer1middle_1;
      HIHistProf* m_fitter_layer1middle_2;
      HIHistProf* m_fitter_layer1middle_3;
      HIHistProf* m_fitter_layer1middle_4;

      HIHistProf* m_fitter_layer1left1;
      HIHistProf* m_fitter_layer1left1_1;
      HIHistProf* m_fitter_layer1left1_2;
      HIHistProf* m_fitter_layer1left1_3;
      HIHistProf* m_fitter_layer1left1_4;

      HIHistProf* m_fitter_layer2left2;
      HIHistProf* m_fitter_layer2left1;
      HIHistProf* m_fitter_layer2right1;
      HIHistProf* m_fitter_layer2right2;

      HIHistProf* m_fitter_layer3left2;
      HIHistProf* m_fitter_layer3left1;
      HIHistProf* m_fitter_layer3middle;
      HIHistProf* m_fitter_layer3right1;
      HIHistProf* m_fitter_layer3right2;

      HIHistProf* m_fitter_layer4left4;
      HIHistProf* m_fitter_layer4left3;
      HIHistProf* m_fitter_layer4left2;
      HIHistProf* m_fitter_layer4left1;
      HIHistProf* m_fitter_layer4right1;
      HIHistProf* m_fitter_layer4right2;
      HIHistProf* m_fitter_layer4right3;
      HIHistProf* m_fitter_layer4right4;

      HIHist1D* m_my_rphi_residual;
      HIHist1D* m_my_z_residual;

      HIHistProf* m_my_layer1right1;
      HIHistProf* m_my_layer1right1_1;
      HIHistProf* m_my_layer1right1_2;
      HIHistProf* m_my_layer1right1_3;
      HIHistProf* m_my_layer1right1_4;

      HIHistProf* m_my_layer1middle;
      HIHistProf* m_my_layer1middle_1;
      HIHistProf* m_my_layer1middle_2;
      HIHistProf* m_my_layer1middle_3;
      HIHistProf* m_my_layer1middle_4;

      HIHistProf* m_my_layer1left1;
      HIHistProf* m_my_layer1left1_1;
      HIHistProf* m_my_layer1left1_2;
      HIHistProf* m_my_layer1left1_3;
      HIHistProf* m_my_layer1left1_4;

      HIHistProf* m_my_layer2left2;
      HIHistProf* m_my_layer2left1;
      HIHistProf* m_my_layer2right1;
      HIHistProf* m_my_layer2right2;

      HIHistProf* m_my_layer3left2;
      HIHistProf* m_my_layer3left1;
      HIHistProf* m_my_layer3middle;
      HIHistProf* m_my_layer3right1;
      HIHistProf* m_my_layer3right2;

      HIHistProf* m_my_layer4left4;
      HIHistProf* m_my_layer4left3;
      HIHistProf* m_my_layer4left2;
      HIHistProf* m_my_layer4left1;

      HIHistProf* m_my_layer4right1;
      HIHistProf* m_my_layer4right2;
      HIHistProf* m_my_layer4right3;
      HIHistProf* m_my_layer4right4;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* FINDBUG_FINDBUG_H */
