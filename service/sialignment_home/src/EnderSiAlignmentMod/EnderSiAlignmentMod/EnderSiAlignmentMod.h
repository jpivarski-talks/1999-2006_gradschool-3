// -*- C++ -*-
#if !defined(ENDERSIALIGNMENTMOD_ENDERSIALIGNMENTMOD_H)
#define ENDERSIALIGNMENTMOD_ENDERSIALIGNMENTMOD_H
//
// Package:     EnderSiAlignmentMod
// Module:      EnderSiAlignmentMod
//
/**\class EnderSiAlignmentMod EnderSiAlignmentMod.h EnderSiAlignmentMod/EnderSiAlignmentMod.h
 
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

#include "JobControl/JobControl.h"
#include "ToolBox/HistogramPackage.h"

#include <fstream.h>
#include <iostream.h>

// forward declarations

extern "C" float prob_( const float&, const int& );

class EnderSiAlignmentMod : public FrameIteratorModuleBase, public MIFcn
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

      enum {
	 kDeltaX,
	 kDeltaY,
	 kDeltaZ,
	 kPhiX,
	 kPhiY,
	 kPhiZ,
	 kNumParams
      };

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
      EnderSiAlignmentMod();
      virtual ~EnderSiAlignmentMod();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      ///override this function to do the actual iterations
      virtual void iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd );
      double iterate( double* values );

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      EnderSiAlignmentMod( const EnderSiAlignmentMod& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const EnderSiAlignmentMod& operator=( const EnderSiAlignmentMod& ); // stop default

      // ---------- private member functions -------------------

      DABoolean read_init_constants();
      void init_hits();
      void get_hits( const FIFrameIterator& iBegin,
		     const FIFrameIterator& iEnd );
      DABoolean read_hits();

      double get_deltaX();
      double get_deltaY();
      double get_deltaZ();
      double get_phiX();
      double get_phiY();
      double get_phiZ();

      void set_deltaX( double value );
      void set_deltaY( double value );
      void set_deltaZ( double value );
      void set_phiX( double value );
      void set_phiY( double value );
      void set_phiZ( double value );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      Parameter< uint > m_target_hits;

      Parameter< string > m_file_global;
      Parameter< string > m_file_layer;
      Parameter< string > m_file_ladder;
      Parameter< string > m_file_sensor;

      Parameter< string > m_file_output;

      Parameter< string > m_device;
      Parameter< uint > m_number;

      Parameter< string > m_skim_in;

      Parameter< double > m_rphi_norm;
      Parameter< double > m_z_norm;
      Parameter< double > m_rphi_cut;
      Parameter< double > m_z_cut;

      Parameter< double > m_deltaX_step;
      Parameter< double > m_deltaY_step;
      Parameter< double > m_deltaZ_step;
      Parameter< double > m_phiX_step;
      Parameter< double > m_phiY_step;
      Parameter< double > m_phiZ_step;

      Parameter< double > m_deltaX_limit;
      Parameter< double > m_deltaY_limit;
      Parameter< double > m_deltaZ_limit;
      Parameter< double > m_phiX_limit;
      Parameter< double > m_phiY_limit;
      Parameter< double > m_phiZ_limit;

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

      FILE* m_output;

      HelixPtr* m_tracks;
      SVRPtrPtr* m_svr_hits;
      SVZPtrPtr* m_svz_hits;
      uint* m_svr_lengths;
      uint* m_svz_lengths;
      int m_num_tracks;
      uint m_num_svr_hits;
      uint m_num_svz_hits;

      const FIFrameIterator* m_begin;

      uint m_minuit_iteration;

      double m_first_params[ kNumParams ];
      double m_first_chisq;
      double m_first_rphi_mean;
      double m_first_rphi_spread;
      double m_first_z_mean;
      double m_first_z_spread;

      double m_last_params[ kNumParams ];
      double m_last_chisq;
      double m_last_rphi_mean;
      double m_last_rphi_spread;
      double m_last_z_mean;
      double m_last_z_spread;

      HIHist1D* m_rphi_resid_before;
      HIHist1D* m_z_resid_before;
      HIHist1D* m_rphi_resid_after;
      HIHist1D* m_z_resid_after;

      DABoolean m_plot_before;
      DABoolean m_plot_after;

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* ENDERSIALIGNMENTMOD_ENDERSIALIGNMENTMOD_H */
