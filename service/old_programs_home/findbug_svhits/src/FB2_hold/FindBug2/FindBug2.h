// -*- C++ -*-
#if !defined(FINDBUG2_FINDBUG2_H)
#define FINDBUG2_FINDBUG2_H
//
// Package:     <FindBug2>
// Module:      FindBug2
//
/**\class FindBug2 FindBug2.h FindBug2/FindBug2.h
 
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
#include "HbookHistogram/HbookNtuple.h"
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

#define NULL 0

// forward declarations

class FindBug2 : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kCotTh, kPhi0, kZ0,
	 kLayer, kLadder, kSensor, kHybrid,
	 kLocX, kLocY, kLocZ,
	 kTLocX, kTLocY, kTLocZ,
	 kWorldX, kWorldY, kWorldZ,
	 kTrackX, kTrackY, kTrackZ, kTrackPhi,
	 kResidual,

	 kNumVars,
	 kLargeRecord = 262144,
	 kNumSensors = 447
      };

      // ------------ Constructors and destructor ----------------
      FindBug2( void );                      // anal1 
      virtual ~FindBug2();                   // anal5 

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
      FindBug2( const FindBug2& );

      // ------------ assignment operator(s) ---------------------
      const FindBug2& operator=( const FindBug2& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (FindBug2::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_rphi;
      HINtuple* m_z;

      unsigned int m_num_rphi;
      unsigned int m_num_z;

      HepTransform3D* m_world2local;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* FINDBUG2_FINDBUG2_H */
