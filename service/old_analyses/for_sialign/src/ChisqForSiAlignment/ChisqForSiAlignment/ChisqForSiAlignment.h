#if !defined(CHISQFORSIALIGNMENT_CHISQFORSIALIGNMENT_H)
#define CHISQFORSIALIGNMENT_CHISQFORSIALIGNMENT_H
// -*- C++ -*-
//
// Package:     ChisqForSiAlignment
// Module:      ChisqForSiAlignment
//
/**\class ChisqForSiAlignment ChisqForSiAlignment.h ChisqForSiAlignment/ChisqForSiAlignment.h
 
 Description: (dynamically-loadable) module
 Base class for the classes which will compute the chi square values for the
 different silicon components

 Usage:
    <usage>

*/
//
// Author:      Lauren Hsu
// Created:     Sat Jun  3 09:58:31 EDT 2000
// $Id: ChisqForSiAlignment.h,v 1.3 2001/03/29 17:00:29 llh14 Exp $
//
// Revision history
//
// $Log: ChisqForSiAlignment.h,v $
// Revision 1.3  2001/03/29 17:00:29  llh14
// Saving Curvature instead of Hybrid Number
//
// Revision 1.2  2001/02/21 16:51:36  llh14
// Added dof as Parameters
//
// Revision 1.1.1.1  2001/02/02 18:03:28  llh14
// imported ChisqForSiAlignment
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

class ChisqForSiAlignment : public MIFcn
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum hitRemoveInfo {kEvent, kTrack, kHit};

      enum ntupleContents { kResidual, kSensor, kCurvature, kLadder, 
			    kClamshell, kZ,
			    //kX, kY, kR, 
			    kPhi0, kCotTheta, kEventCtr, kNtupleSize};
     
//      enum ntupleContents { kSumResidual, kPhi0, kZ, kLayersHit, kNtupleSize };


      // ---------- Constructors and destructor ----------------
      ChisqForSiAlignment();

      virtual ~ChisqForSiAlignment();

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

      DABoolean isHitBad(Count eventCtr, 
			 Count trackCtr,
			 Count hitCtr,
			 CalibratedHit::DeviceType hitType);

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ChisqForSiAlignment( const ChisqForSiAlignment& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ChisqForSiAlignment& operator=( const ChisqForSiAlignment& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      STL_VECTOR(Count) m_badEventsForRphi;
      STL_VECTOR(Count) m_badEventsForZ;
      STL_VECTOR(Count) m_badTracksForRphi;
      STL_VECTOR(Count) m_badTracksForZ;
      STL_VECTOR(Count) m_badHitsForRphi;
      STL_VECTOR(Count) m_badHitsForZ;
      STL_VECTOR( STL_VECTOR(Count) ) m_removeHitsForRphi;
      STL_VECTOR( STL_VECTOR(Count) ) m_removeHitsForZ;

      STL_VECTOR(Count)::const_iterator m_badEventsRphiPtr;
      STL_VECTOR(Count)::const_iterator m_badEventsZPtr;
      STL_VECTOR(Count)::const_iterator m_badTracksRphiPtr;
      STL_VECTOR(Count)::const_iterator m_badTracksZPtr;
      STL_VECTOR(Count)::const_iterator m_badHitsRphiPtr;
      STL_VECTOR(Count)::const_iterator m_badHitsZPtr;

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
//# include "ChisqForSiAlignment/Template/ChisqForSiAlignment.cc"
//#endif

#endif /* CHISQFORSIALIGNMENT_CHISQFORSIALIGNMENT_H */
