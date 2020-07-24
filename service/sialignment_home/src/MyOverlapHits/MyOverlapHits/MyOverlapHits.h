// -*- C++ -*-
#if !defined(MYOVERLAPHITS_MYOVERLAPHITS_H)
#define MYOVERLAPHITS_MYOVERLAPHITS_H
//
// Package:     <MyOverlapHits>
// Module:      MyOverlapHits
//
/**\class MyOverlapHits MyOverlapHits.h MyOverlapHits/MyOverlapHits.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Sun Nov 25 16:04:05 EST 2001
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
#include "CleoDB/DBEventHeader.h"

// forward declarations

class MyOverlapHits : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kLayer,
	 kLadLe, kSenLe, kLadGr, kSenGr,
	 kLocZLe, kLocZGr,
	 kResLe, kResGr, kResDiff,
	 kWorldXLe, kWorldYLe, kWorldZLe,
	 kWorldXGr, kWorldYGr, kWorldZGr,

	 kNumVars,
	 kLargeRecord = 262144
      };

      // ------------ Constructors and destructor ----------------
      MyOverlapHits( void );                      // anal1 
      virtual ~MyOverlapHits();                   // anal5 

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
      MyOverlapHits( const MyOverlapHits& );

      // ------------ assignment operator(s) ---------------------
      const MyOverlapHits& operator=( const MyOverlapHits& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (MyOverlapHits::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      unsigned int m_run;

      HINtuple* m_rphi;
      HINtuple* m_z;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* MYOVERLAPHITS_MYOVERLAPHITS_H */
