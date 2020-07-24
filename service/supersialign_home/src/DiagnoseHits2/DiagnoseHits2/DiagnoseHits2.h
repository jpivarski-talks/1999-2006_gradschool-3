// -*- C++ -*-
#if !defined(DIAGNOSEHITS2_DIAGNOSEHITS2_H)
#define DIAGNOSEHITS2_DIAGNOSEHITS2_H
//
// Package:     <DiagnoseHits2>
// Module:      DiagnoseHits2
//
/**\class DiagnoseHits2 DiagnoseHits2.h DiagnoseHits2/DiagnoseHits2.h
 
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
class DiagnoseHits2 : public Processor
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
      DiagnoseHits2( void );                      // anal1 
      virtual ~DiagnoseHits2();                   // anal5 

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
      DiagnoseHits2( const DiagnoseHits2& );

      // ------------ assignment operator(s) ---------------------
      const DiagnoseHits2& operator=( const DiagnoseHits2& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (DiagnoseHits2::*iMethod)( Frame& ),
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

      HIHistProf* m_posZ_rphires_v_phi[61];
      HIHistProf* m_negZ_rphires_v_phi[61];
      HIHistProf* m_posZ_zres_v_phi[61];
      HIHistProf* m_negZ_zres_v_phi[61];

      HIHistProf* m_posZ_rphires_v_phi_all;
      HIHistProf* m_negZ_rphires_v_phi_all;
      HIHistProf* m_posZ_zres_v_phi_all;
      HIHistProf* m_negZ_zres_v_phi_all;

      DABoolean m_done;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* DIAGNOSEHITS2_DIAGNOSEHITS2_H */
