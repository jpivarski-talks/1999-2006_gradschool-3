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
// Created:     Fri Sep 14 12:59:14 EDT 2001
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

#include "ASiStorePro/ASiStore.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"
#include "Navigation/NavTrack.h"
#include "HelixIntersection/HIHelix.h"

#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "FrameAccess/FAConstants.h"
#include "BDLSVclient/DBSVAlignmentGlobal.hh"
#include "BDLSVclient/DBSVAlignmentLayer.hh"
#include "BDLSVclient/DBSVAlignmentLadder.hh"
#include "BDLSVclient/DBSVAlignmentSensor.hh"
#include "MinuitInterface/MinuitInterface.h"
#include "MinuitInterface/MIFcn.h"

#include "HandSiAlignmentMod/ChangeConstantsCommand.h"

#include <fstream.h>
#include <iostream.h>

// forward declarations

extern "C" float prob_( const float&, const int& );
class HVPWindow;
class QApplication;

void get_edges( int sensor, double& lower1, double& lower2,
		            double& upper1, double& upper2 );

class HandSiAlignmentMod : public FrameIteratorModuleBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      typedef HIHelix* HelixPtr;
      typedef CalibratedSVRphiHit* SVRPtr;
      typedef CalibratedSVZHit* SVZPtr;
      typedef SVRPtr* SVRPtrPtr;
      typedef SVZPtr* SVZPtrPtr;
      typedef unsigned int uint;

      typedef enum {
	 kDeltaX,
	 kDeltaY,
	 kDeltaZ,
	 kPhiX,
	 kPhiY,
	 kPhiZ,
	 kNumParams
      } Direction;

      typedef enum {
	 kGlobal,
	 kLayer,
	 kLadder,
	 kSensor,
	 kNumSubComponents
      } SubComponent;

      typedef enum {
	 kNothing,
	 kTrack,
	 kSVRphiHit,
	 kSVZHit
      } NextInFile;

      // ---------- Constructors and destructor ----------------
      HandSiAlignmentMod();
      virtual ~HandSiAlignmentMod();

      // ---------- member functions ---------------------------

      double get_constant( SubComponent comp, uint which, Direction dir );
      void set_constant( SubComponent comp, uint which, Direction dir, double value );
      int get_showing();
      double get_cotTheta( int sensor );
      void get_edges( int sensor, double& lower1, double& lower2,
		                  double& upper1, double& upper2 );

      void replot();
      static HandSiAlignmentMod* instance();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      ///override this function to do the actual iterations
      virtual void iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd );

      // ---------- protected const member functions -----------

      static HandSiAlignmentMod* m_instance;

   private:
      // ---------- Constructors and destructor ----------------
      HandSiAlignmentMod( const HandSiAlignmentMod& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HandSiAlignmentMod& operator=( const HandSiAlignmentMod& ); // stop default

      // ---------- private member functions -------------------

      DABoolean read_init_constants();
      void init_hits();
      void switch_files();
      void get_hits( const FIFrameIterator& iBegin,
		     const FIFrameIterator& iEnd );
      DABoolean read_hits();
      void plot();
      void print_out();

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      ChangeConstantsCommand m_command;

      Parameter< string > m_file_global;
      Parameter< string > m_file_layer;
      Parameter< string > m_file_ladder;
      Parameter< string > m_file_sensor;

      Parameter< double > m_rphi_cut;
      Parameter< double > m_z_cut;

      Parameter< string > m_device;
      Parameter< uint > m_number;

      Parameter< string > m_skim_dir;
      string m_skim_in;

      CLEOConstantsModifiable< DBSVAlignmentGlobal > m_const_global;
      CLEOConstantsModifiable< DBSVAlignmentLayer  > m_const_layer;
      CLEOConstantsModifiable< DBSVAlignmentLadder > m_const_ladder;
      CLEOConstantsModifiable< DBSVAlignmentSensor > m_const_sensor;
      FIHolder< CLEOConstants< DBSVAlignmentGlobal > > m_holder_global;
      FIHolder< CLEOConstants< DBSVAlignmentLayer  > > m_holder_layer;
      FIHolder< CLEOConstants< DBSVAlignmentLadder > > m_holder_ladder;
      FIHolder< CLEOConstants< DBSVAlignmentSensor > > m_holder_sensor;

      SubComponent m_comp_dev;
      uint m_comp_num;

      DABoolean m_first_time;
      DABoolean m_const_loaded;
      JobControl* m_jc;
      HIHistoManager* m_hm;
      QApplication* m_app;
      HVPWindow* m_window;

      HIHist1D* m_rphi_resid;
      HIHist1D* m_z_resid;
      HIHistProf* m_rphires_v_x;
      HIHistProf* m_rphires_v_x_negz;
      HIHistProf* m_rphires_v_x_posz;
      HIHistProf* m_rphires_v_z;
      HIHistProf* m_zres_v_z;
      HIHistProf* m_zres_v_z2;
      HIHist2D* m_scat_rphires_v_x;
      HIHist2D* m_scat_rphires_v_z;
      HIHist2D* m_scat_zres_v_z;

      HelixPtr* m_tracks;
      SVRPtrPtr* m_svr_hits;
      SVZPtrPtr* m_svz_hits;

      uint* m_svr_lengths;
      uint* m_svz_lengths;
      int m_num_tracks;
      uint m_num_svr_hits;
      uint m_num_svz_hits;

      const FIFrameIterator* m_begin;

      double m_sensor_cotTheta[447];
      const ASiStore* m_siStore;
      vector< uint > m_sensors_seen;

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* HANDSIALIGNMENTMOD_HANDSIALIGNMENTMOD_H */
