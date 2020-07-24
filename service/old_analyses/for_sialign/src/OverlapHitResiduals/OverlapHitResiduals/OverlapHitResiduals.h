#if !defined(OVERLAPHITRESIDUALS_OVERLAPHITRESIDUALS_H)
#define OVERLAPHITRESIDUALS_OVERLAPHITRESIDUALS_H
// -*- C++ -*-
//
// Package:     OverlapHitResiduals
// Module:      OverlapHitResiduals
//
/**\class OverlapHitResiduals OverlapHitResiduals.h OverlapHitResiduals/OverlapHitResiduals.h
 
 Description: (dynamically-loadable) module
 Base class for the classes which will compute the chi square values for the
 different silicon components

 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Sat Jun  3 09:58:31 EDT 2000
// $Id: OverlapHitResiduals.h,v 1.1.1.1 2001/03/06 22:55:18 llh14 Exp $
//
// Revision history
//
// $Log: OverlapHitResiduals.h,v $
// Revision 1.1.1.1  2001/03/06 22:55:18  llh14
// imported OverlapHitResiduals
//
//

// system include files

// user include files
#include "ASiStorePro/ASiStore.h"

#include "DualTrackHelices/DualTrackHelices.h"

#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
#include "FrameAccess/FATable.h"

#include "HelixIntersection/HIIntersectionSurface.h"

#include "MinuitInterface/MIFcn.h"

#include "Doit/DoitSeedQuality.h"
// forward declarations

class OverlapHitResiduals : public MIFcn
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum hitRemoveInfo {kEvent, kTrack, kHit};

      enum ntupleContents { kResCurrent, kResNext,
			    kResDiff, kZDiff, kZ, kPhi0Diff, 	    
			    kPhi0, kLadder, kLayer, kEventCtr, kNtupleSize};
     
      enum layers { kLayer1, kLayer2, kLayer3, kLayer4, kNumLayers };

//      enum ntupleContents { kSumResidual, kPhi0, kNtupleSize };


      // ---------- Constructors and destructor ----------------
      OverlapHitResiduals();
      virtual ~OverlapHitResiduals();

      // ---------- member functions ---------------------------
		      
      virtual double iterate(double* values) = 0;
      virtual DABoolean selectComponent(SmallCount sensorNumber,
					FAItem<ASiStore>& iStore) = 0;

      // Optional choice of making histograms
      void calcResiduals(const FIFrameIterator& iBegin, 
			 const FIFrameIterator& iEnd,
			 Count& eventCtr, 
			 double& chisq, 
			 Count maxEvents,
			 float normRphi,
			 float normZ,
			 float cutRphi,
			 float cutZ,
			 DABoolean histogramsOn = false,
			 HINtuple* rphi_TrackHits = 0,
			 HINtuple* z_TrackHits = 0);

      double getSigmaRes(HIIntersectionSurface* surface, 
			 HIHelix& intHelix,
			 FATable< DualTrackHelices >::const_iterator& 
			 dualHelix);

      void matchSVHits(STL_VECTOR(SmallCount)& siSensorsForHits,
		       const SmallCount numHits,
		       FATable< DualTrackHelices >::const_iterator& dualHelix,
		       STL_VECTOR( HIIntersectionSurface* )& surfaces, 
		       CalibratedHit::DeviceType hitType,
		       Count eventCtr, Count trackCtr, Count& residualRphiCtr, 
		       Count& missedHits, Count& totalHits,    
		       FAItem<ASiStore>& iStore, double& chisq,
		       DABoolean histogramsOn, HINtuple* rPhi_TrackHits,
		       DABoolean debugOn
//		       FATable<DoitSeedQuality>::const_iterator 
//		       doitIter
		       );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      OverlapHitResiduals( const OverlapHitResiduals& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const OverlapHitResiduals& operator=( const OverlapHitResiduals& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Count m_iteration;
      Count m_maxEvents;
      Count m_checkEvent;

      float m_normRphi;
      float m_normZ;
      float m_cutRphi;
      float m_cutZ;
	
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "OverlapHitResiduals/Template/OverlapHitResiduals.cc"
//#endif

#endif /* OVERLAPHITRESIDUALS_OVERLAPHITRESIDUALS_H */
