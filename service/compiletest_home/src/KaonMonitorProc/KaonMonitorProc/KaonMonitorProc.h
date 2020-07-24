// -*- C++ -*-
#if !defined(KAONMONITORPROC_KAONMONITORPROC_H)
#define KAONMONITORPROC_KAONMONITORPROC_H
//
// Package:     <KaonMonitorProc>
// Module:      KaonMonitorProc
//
/**\class KaonMonitorProc KaonMonitorProc.h KaonMonitorProc/KaonMonitorProc.h
 
 Description: Finds Kaons without the vertexing package

 Usage:
    In a job which has fitted tracks and beam energy,
    include "proc sel KaonMonitorProc"

*/
//
// Author:      Jim Pivarski
// Created:     Thu Oct 30 17:46:23 EST 2003
// $Id: KaonMonitorProc.h,v 1.1.1.1 2003/11/18 22:04:18 mccann Exp $
//
// Revision history
//
// $Log: KaonMonitorProc.h,v $
// Revision 1.1.1.1  2003/11/18 22:04:18  mccann
// imported KaonMonitorProc sources
//
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "HistogramInterface/HINumber.h"
#include "HistogramInterface/HINumberWithErrors.h"
#include "CommandPattern/Parameter.h"
#include "KinematicTrajectory/KTHelix.h"
#include "CLHEP/Geometry/Point3D.h"
#include "Utility/Severity.h"

// forward declarations

class KaonMonitorProc : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
         kRa, kXa, kYa,
         kRb, kXb, kYb,
         kNumCirc
      };
      enum {
         kCurva, kD0a, kPhi0a,
         kCurvb, kD0b, kPhi0b,
         kNumTrack
      };

      // ------------ Constructors and destructor ----------------
      KaonMonitorProc( void );                      // anal1 
      virtual ~KaonMonitorProc();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
      virtual ActionBase::ActionResult beginRun( Frame& iFrame);
      virtual ActionBase::ActionResult endRun( Frame& iFrame);

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      void printout(Severity severity);

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      KaonMonitorProc( const KaonMonitorProc& );

      // ------------ assignment operator(s) ---------------------
      const KaonMonitorProc& operator=( const KaonMonitorProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (KaonMonitorProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      HepPoint3D event_vertex(vector<KTHelix>& good_tracks);
      DABoolean calc_intersection(HepPoint3D& intersection,
				  double& zsep, double& sigz,
				  double& aphi, double& bphi,
				  KTHelix& a, KTHelix& b);
      DABoolean arclength_test(double x, double y, KTHelix& a, KTHelix& b,
			       double& aarclength, double& barclength);
      Hep3Vector rotate_momentum(Hep3Vector momentum, double angle);

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter<unsigned int> m_printoutFrequency;
      Parameter<unsigned int> m_startSkip;

      double m_beamX_sum;
      unsigned int m_beamX_N;
      double m_beamY_sum;
      unsigned int m_beamY_N;
      double m_beamZ_sum;
      unsigned int m_beamZ_N;

      HIHist1D* m_direction;
      HIHist1D* m_mass;
      HIHist1D* m_lifetime;
      HIHist1D* m_radial_efficiency;
      HIHist1D* m_phi;
      HIHist1D* m_cosTheta;

      int m_kaons;
      int m_events;
      int m_hadronic;
      int m_beamgas;
      int m_cosmic;

      HINumber* m_kaon_count;
      HINumber* m_event_count;
      HINumber* m_hadronic_count;
      HINumber* m_beamgas_count;
      HINumber* m_cosmic_count;
      HINumberWithErrors* m_kaons_per_event;
      HINumberWithErrors* m_kaons_per_hadronic;
      HINumberWithErrors* m_beamgas_per_hadronic;
      HINumberWithErrors* m_cosmic_per_hadronic;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* KAONMONITORPROC_KAONMONITORPROC_H */
