// -*- C++ -*-
#if !defined(DIAGNOSEHITS_DIAGNOSEHITS_H)
#define DIAGNOSEHITS_DIAGNOSEHITS_H
//
// Package:     <DiagnoseHits>
// Module:      DiagnoseHits
//
/**\class DiagnoseHits DiagnoseHits.h DiagnoseHits/DiagnoseHits.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri May 31 12:14:03 EDT 2002
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
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HISiSensorPlane.h"
#include "HelixIntersection/HICompositeSurface.h"
#include "KinematicTrajectory/KTHelix.h"
#include "KinematicTrajectory/KTKinematicData.h"

#include "HandSiAlignmentMod/HSAM_Store.h"

// forward declarations

class HSAM_Store;
class HSAM_StoreSVRonly;
class HSAM_StoreSVZonly;
class HSAM_StoreBoth;
class DiagnoseHits : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------
      typedef enum {
	 kNothing,
	 kTrack,
	 kSVRphiHit,
	 kSVZHit
      } NextInFile;

      // ------------ Constructors and destructor ----------------
      DiagnoseHits( void );                      // anal1 
      virtual ~DiagnoseHits();                   // anal5 

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

      DABoolean switchToContext( int ladder );
      void doladder( int ladder, Frame& iFrame );

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------
      void deleteTracksHits();

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      DiagnoseHits( const DiagnoseHits& );

      // ------------ assignment operator(s) ---------------------
      const DiagnoseHits& operator=( const DiagnoseHits& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (DiagnoseHits::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter< string > m_baseDir;
      Parameter< double > m_maxHits;
      Parameter< double > m_rphiCut;
      Parameter< double > m_zCut;
      Parameter< double > m_pthresh;
      Parameter< double > m_nthresh;
      Parameter< double > m_diagLadderStart;
      Parameter< double > m_diagLadderStop;

      HSAM_Store* m_hsam_store;

      HIHist1D* m_rphi_hits_phicell_posZ[61];
      HIHistProf* m_rphi_vs_phicell_posZ[61];
      HIHistProf* m_w_rphi_vs_phicell_posZ[61];
      HIHist2D* m_rphi_vs_phicell_scat_posZ[61];
      HIHist1D* m_rphi_hits_phicell_negZ[61];
      HIHistProf* m_rphi_vs_phicell_negZ[61];
      HIHistProf* m_w_rphi_vs_phicell_negZ[61];
      HIHist2D* m_rphi_vs_phicell_scat_negZ[61];

      HIHist1D* m_rphi_hits_phicell_all_posZ;
      HIHistProf* m_rphi_vs_phicell_all_posZ;
      HIHistProf* m_w_rphi_vs_phicell_all_posZ;
      HIHist2D* m_rphi_vs_phicell_scat_all_posZ;
      HIHist1D* m_rphi_hits_phicell_all_negZ;
      HIHistProf* m_rphi_vs_phicell_all_negZ;
      HIHistProf* m_w_rphi_vs_phicell_all_negZ;
      HIHist2D* m_rphi_vs_phicell_scat_all_negZ;

      DABoolean m_done;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* DIAGNOSEHITS_DIAGNOSEHITS_H */
