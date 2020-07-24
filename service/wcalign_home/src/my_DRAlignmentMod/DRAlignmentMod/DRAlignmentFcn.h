#if !defined(DRALIGNMENTMOD_DRALIGNMENTFCN_H)
#define DRALIGNMENTMOD_DRALIGNMENTFCN_H
// -*- C++ -*-
//
// Package:     <DRAlignmentMod>
// Module:      DRAlignmentFcn
// 
/**\class DRAlignmentFcn DRAlignmentFcn.h DRAlignmentMod/DRAlignmentFcn.h

 Description: see DRAlignmentMod.cc

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Dec  8 11:50:39 EST 2003
// $Id: 
//
// Revision history
//

// system include files

// user include files

#include "MinuitInterface/MinuitInterface.h"
#include "MinuitInterface/MIFcn.h"
#include "HelixIntersection/HIHelix.h"
#include "CalibratedData/CalibratedDRHit.h"
#include "FrameAccess/FAConstants.h"
#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "BDLDRclient/DBDRGeomAlignment.hh"
#include "DRHitCorrectorProd/HitCorrector.h"
#include "ADRGeom/ADRSenseWireStore.h"
#include "DetectorGeometry/DGConstMaterialPtr.h"
#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
#include "qapplication.h"
#include "qtimer.h"
#include "HistogramViewerProc/HVPWindow.h"
#include "ToolBox/HistogramPackage.h"

// forward declarations
class HVPWindow;
class QApplication;

class DRAlignmentFcn : public MIFcn
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum params {kXEast,
		   kYEast,
		   kPhiEast,
		   kXWest,
		   kYWest,
		   kPhiWest,
		   kT0,
		   kSpeed,

		   kNumParams
      };

      // ---------- Constructors and destructor ----------------
      DRAlignmentFcn(CLEOConstantsModifiable<DBDRGeomAlignment>* geometry);
      virtual ~DRAlignmentFcn();

      // ---------- member functions ---------------------------

      void book(bool show_plots, bool fake_driftfunction, unsigned int ring);
      void reset(const FIFrameIterator& frame);
      bool fill(const KTHelix& track,
		const double charge,
		const vector<const CalibratedDRHit*>& hits,
		const vector<double>& drifts);
      double iterate(double* v);

      // ---------- const member functions ---------------------
      int cake_ring(int layer) const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DRAlignmentFcn( const DRAlignmentFcn& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DRAlignmentFcn& operator=( const DRAlignmentFcn& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      CLEOConstantsModifiable<DBDRGeomAlignment>* m_geometry;

      const FIFrameIterator* m_frame;

      vector<HIHelix*> m_track;
      vector<double> m_charge;
      vector<vector<CalibratedDRHit*>*> m_vect;
      vector<vector<double>*> m_drifts;

      HIHistoManager* m_histman;
      QApplication* m_qapplication;
      HVPWindow* m_hvp_window;

      HIHistProf* m_resid_Vdrift;
      HIHistProf* m_resid_Vlayer;
      HIHistProf* m_resid_Vcotth;
      HIHistProf* m_resid_Vphi_m;
      HIHistProf* m_resid_Vphi_z;
      HIHistProf* m_resid_Vphi_p;
      HIHist1D*   m_resid;

      double m_best_yet;
      bool m_show_plots;
      bool m_fake_driftfunction;
      unsigned int m_ring;

      double m_original_radius[6];

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DRAlignmentMod/Template/DRAlignmentFcn.cc"
//#endif

#endif /* DRALIGNMENTMOD_DRALIGNMENTFCN_H */
