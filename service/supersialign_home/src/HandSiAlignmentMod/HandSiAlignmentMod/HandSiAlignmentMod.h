// -*- C++ -*-
#if !defined(HANDSIALIGNMENTMOD_HANDSIALIGNMENTMOD_H)
#define HANDSIALIGNMENTMOD_HANDSIALIGNMENTMOD_H
//
// Package:     HandSiAlignmentMod
// Module:      HandSiAlignmentMod
//
/**\class HandSiAlignmentMod HandSiAlignmentMod.h HandSiAlignmentMod/HandSiAlignmentMod.h
 
 Description: Suez Module that allows you to control the "event" loop

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Tue Apr  9 15:11:19 EDT 2002
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "CommandPattern/Module.h"
#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
#include "CommandPattern/Parameter.h"

#include "../../ASiStorePro/ASiStorePro/ASiStore.h"
#include "../../ASiStorePro/ASiStorePro/ASiSensorInfo.h"

#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

#include "../../HelixIntersection/HelixIntersection/HIHelix.h"
#include "../../HelixIntersection/HelixIntersection/HIHelixIntersector.h"
#include "../../HelixIntersection/HelixIntersection/HIIntersectionSurface.h"
#include "../../HelixIntersection/HelixIntersection/HISiSensorPlane.h"
#include "../../HelixIntersection/HelixIntersection/HISiSurfaceFactory.h"
#include "../../HelixIntersection/HelixIntersection/HICompositeSurface.h"

#include "KinematicTrajectory/KTHelix.h"
#include "KinematicTrajectory/KTKinematicData.h"

#include "qapplication.h"
#include "HistogramViewerProc/HVPWindow.h"
#include "JobControl/JobControl.h"
#include "ToolBox/HistogramPackage.h"
#include "HistogramInterface/HIHistDir.h"

#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "FrameAccess/FAConstants.h"
#include "BDLSVclient/DBSVAlignmentGlobal.hh"
#include "BDLSVclient/DBSVAlignmentLayer.hh"
#include "BDLSVclient/DBSVAlignmentLadder.hh"
#include "BDLSVclient/DBSVAlignmentSensor.hh"

#include "HandSiAlignmentMod/DoInitsCommand.h"
#include "HandSiAlignmentMod/ReplotCommand.h"
#include "HandSiAlignmentMod/PickLadderCommand.h"
#include "HandSiAlignmentMod/FakeLadderCommand.h"
#include "HandSiAlignmentMod/MoveLadderCommand.h"
#include "HandSiAlignmentMod/ThreshCommand.h"
#include "HandSiAlignmentMod/RestrictSensorCommand.h"
#include "HandSiAlignmentMod/HSAM_Store.h"

//The data object you wish to change on each iteration
//#include "CleoDB/DBRunHeader.h"  

// forward declarations
class HVPWindow;
class QApplication;
class HSAM_Store;
class HSAM_StoreSVRonly;
class HSAM_StoreSVZonly;
class HSAM_StoreBoth;

class HandSiAlignmentMod : public FrameIteratorModuleBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef enum {
	 kNothing,
	 kTrack,
	 kSVRphiHit,
	 kSVZHit
      } NextInFile;

      typedef enum {
	 kDeltaX,
	 kDeltaY,
	 kDeltaZ,
	 kPhiX,
	 kPhiY,
	 kPhiZ,
	 kSagitta,
	 kNumParams
      } Direction;

      typedef enum {
	 kMove,
	 kReset,
	 kZero,
	 kRound
      } Action;

      // ---------- Constructors and destructor ----------------
      HandSiAlignmentMod();
      virtual ~HandSiAlignmentMod();

      // ---------- member functions ---------------------------

      DABoolean doinits();
      DABoolean replot();
      DABoolean switchToContext( int ladder );
      int context() const { return m_ladder; };
      void fakeContext1( int ladder );
      DABoolean move( Direction dir, double val, Action act );

      double naturalThresh( ThreshCommand::WhichThresh which );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      void deleteTracksHits();
      void fakeContext2( FAItem< ASiStore > store,
			 HISiSurfaceFactory& siSurfaceFactory );
      double residualizer( FAItem< ASiStore > store,
			   HISiSurfaceFactory& siSurfaceFactory,
			   HIHelix* mhelix,
			   CalibratedSVRphiHit* rphi_hit,
			   CalibratedSVZHit* z_hit );
      virtual void iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd );
      void print_out();

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HandSiAlignmentMod( const HandSiAlignmentMod& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HandSiAlignmentMod& operator=( const HandSiAlignmentMod& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      CLEOConstantsModifiable< DBSVAlignmentGlobal > m_const_global;
      CLEOConstantsModifiable< DBSVAlignmentLayer  > m_const_layer;
      CLEOConstantsModifiable< DBSVAlignmentLadder > m_const_ladder;
      CLEOConstantsModifiable< DBSVAlignmentSensor > m_const_sensor;
      FIHolder< CLEOConstants< DBSVAlignmentGlobal > > m_holder_global;
      FIHolder< CLEOConstants< DBSVAlignmentLayer  > > m_holder_layer;
      FIHolder< CLEOConstants< DBSVAlignmentLadder > > m_holder_ladder;
      FIHolder< CLEOConstants< DBSVAlignmentSensor > > m_holder_sensor;
      double m_original_ladder_x[61];
      double m_original_ladder_y[61];
      double m_original_ladder_z[61];
      double m_original_ladder_phix[61];
      double m_original_ladder_phiy[61];
      double m_original_ladder_phiz[61];
      double m_original_sensor_x[447];
      double m_original_sensor_y[447];
      double m_original_sensor_z[447];
      double m_original_sensor_phix[447];
      double m_original_sensor_phiy[447];
      double m_original_sensor_phiz[447];

      double m_sagitta[61];
      double m_original_sagitta[61];

      DoInitsCommand m_doinits;
      ReplotCommand m_replot;
      PickLadderCommand m_pickLadder;
      FakeLadderCommand m_fakeLadder;
      MoveLadderCommand m_moveX;
      MoveLadderCommand m_moveY;
      MoveLadderCommand m_moveZ;
      MoveLadderCommand m_movePhiX;
      MoveLadderCommand m_movePhiY;
      MoveLadderCommand m_movePhiZ;
      MoveLadderCommand m_moveSagitta;
      ThreshCommand m_pthresh;
      ThreshCommand m_nthresh;
      RestrictSensorCommand m_restrictSensor;
      Parameter< string > m_baseDir;
      Parameter< double > m_maxHits;
      Parameter< string > m_globalFile;
      Parameter< string > m_layerFile;
      Parameter< string > m_ladderFile;
      Parameter< string > m_sensorFile;
      Parameter< string > m_sagittaFile;
      Parameter< double > m_rphiCut;
      Parameter< double > m_zCut;
      Parameter< double > m_fakeNumPhi0;
      Parameter< double > m_fakeNumCotTheta;
      int m_ladder;
      DABoolean m_faking;

      HSAM_Store* m_hsam_store;

      JobControl* m_jc;
      HIHistoManager* m_hm;
      QApplication* m_app;
      HVPWindow* m_window;

      HIHistDir* m_align;
      HIHistDir* m_smiles;
      HIHistDir* m_other;
      HIHistDir* m_diagnostic;

      HIHist2D* m_rphiview;
      HIHist1D* m_zview;

      HIHistProf* m_posZ_rphiVx;
      HIHistProf* m_negZ_rphiVx;
      HIHist1D* m_rphi_trackz;
      HIHist1D* m_rphi_matchedz;

      HIHistProf* m_w_posZ_rphiVx;
      HIHistProf* m_w_negZ_rphiVx;

      HIHist2D* m_posZ_rphiVx_scat;
      HIHist2D* m_negZ_rphiVx_scat;
      HIHist1D* m_rphi_hitx;
      HIHistProf* m_rphiVmatchedz;
      HIHist2D* m_rphiVmatchedz_scat;
      HIHistProf* m_rphiVtrackz;
      HIHist2D* m_rphiVtrackz_scat;
      HIHistProf* m_w_rphiVtrackz;
      HIHistProf* m_zresVz;
      HIHist2D* m_zresVz_scat;
      HIHistProf* m_zresVtrackz;
      HIHist2D* m_zresVtrackz_scat;
      HIHistProf* m_w_zresVz;

      HIHistProf* m_zresVphi_layer1;
      HIHistProf* m_zresVphi_layer2;
      HIHistProf* m_zresVphi_layer3;
      HIHistProf* m_zresVphi_layer4;
      HIHist2D* m_zresVphi_layer1_scat;
      HIHist2D* m_zresVphi_layer2_scat;
      HIHist2D* m_zresVphi_layer3_scat;
      HIHist2D* m_zresVphi_layer4_scat;
      HIHistProf* m_w_zresVphi_layer1;
      HIHistProf* m_w_zresVphi_layer2;
      HIHistProf* m_w_zresVphi_layer3;
      HIHistProf* m_w_zresVphi_layer4;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HandSiAlignmentMod/Template/HandSiAlignmentMod.cc"
//#endif

#endif /* HANDSIALIGNMENTMOD_HANDSIALIGNMENTMOD_H */
