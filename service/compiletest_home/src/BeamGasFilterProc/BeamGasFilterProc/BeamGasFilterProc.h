// -*- C++ -*-
#if !defined(BEAMGASFILTERPROC_BEAMGASFILTERPROC_H)
#define BEAMGASFILTERPROC_BEAMGASFILTERPROC_H
//
// Package:     <BeamGasFilterProc>
// Module:      BeamGasFilterProc
//
/**\class BeamGasFilterProc BeamGasFilterProc.h BeamGasFilterProc/BeamGasFilterProc.h
 
 Description: Cuts out cosmics and beamwall events, and some beamgas (keeps 99.4% of signal, 43% of beamgas/beamwall/cosmics, AFTER hadronic subcollection cuts)

 Usage:
    
    Include this as a "proc sel" at the appropriate point in your suez
    job to filter out beamgas/beamwall/cosmic rays.  Naturally,
    it is the hadrons that will be ActionBase::kPassed.

    There are seven parameters:

        closestIntersectionCut (default 0.005)    cut value in meters
	weightedZCut (default 0.05)               cut value in meters
	weightedZCleverness (default 0.)          a mode number for
                                                  choosing the method
                                  of weighted_z calculation.  The only
                                  other option is -1, which isn't very
                                  clever at all.

	explicitBeamSpot (default false)          if false, get
                                                  beamspot from Roy's
			          FAItem<BeamSpot>.  Otherwise, it
			          comes from the following:
	beamX (default 0.)
	beamY (default 0.)
	beamZ (default 0.)

	Note that Monte Carlo is generated with a beamspot of (0,0,0).

    An efficiency calculation is given in the SYSTEM stream at the end
    of processing, and intermittently in the INFO stream.

    If you're using the default cuts, BeamGasFilterProc will also
    estimate how many beamgas events are passing the cuts, with
    statistaical and correlated uncertainties.

*/
//
// Author:      Jim Pivarski
// Created:     Wed Dec 18 10:02:27 EST 2002
// $Id: BeamGasFilterProc.h,v 1.7 2003/03/10 17:54:24 mccann Exp $
//
// Revision history
//
// $Log: BeamGasFilterProc.h,v $
// Revision 1.7  2003/03/10 17:54:24  mccann
// included check on beamspot determination and fixed another inconsistancy between cuts in analyzed sample VS cuts in BeamGasFilterProc (BGFP now consistently subtracts m_beam_z from weighted_z)
//
// Revision 1.6  2003/03/10 15:36:31  mccann
// Added a cosmics count and more parameter control over output; fixed a bug in beamgas estimation (previously forgot to apply RPHI cut to shoulder events)
//
// Revision 1.5  2003/03/05 21:09:08  mccann
// Fixed version control problem with IntersectionInfo.h (not yet officially existant).
//
// Revision 1.4  2003/03/05 01:21:50  mccann
// Removed neutral energy cut, added run-time estimation of how much beamgas PASSES cuts.
//
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"
#include "KinematicTrajectory/KTHelix.h"
#include "Experiment/report.h"

// forward declarations

class BeamGasFilterProc : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      BeamGasFilterProc( void );                      // anal1 
      virtual ~BeamGasFilterProc();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
      virtual ActionBase::ActionResult beginRun( Frame& iFrame);
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
      BeamGasFilterProc( const BeamGasFilterProc& );

      // ------------ assignment operator(s) ---------------------
      const BeamGasFilterProc& operator=( const BeamGasFilterProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (BeamGasFilterProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      void printout(Severity severity, DABoolean runOnly = false);

      DABoolean calc_intersection(
	 double& x, double& y, double& z, double& sig2z, double& zsep,
	 KTHelix& a, KTHelix& b );
      DABoolean arclength_test( double x, double y, KTHelix& a, KTHelix& b );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter< double > m_closestIntersectionCut;
      Parameter< double > m_weightedZCut;
      Parameter< double > m_weightedZCleverness;
      Parameter< DABoolean > m_explicitBeamSpot;
      Parameter< double > m_beamX;
      Parameter< double > m_beamY;
      Parameter< double > m_beamZ;
      Parameter< double > m_centerFactor;
      Parameter< double > m_centerFactorUncert;
      Parameter< double > m_cosmicCenterFactor;
      Parameter< double > m_cosmicCenterFactorUncert;
      Parameter< DABoolean > m_noExplaination;
      Parameter< double > m_cumulativeFrequency;
      int m_passed;
      int m_shoulder;
      int m_cosmic_shoulder;
      int m_total;
      int m_run;
      int m_run_passed;
      int m_run_shoulder;
      int m_run_cosmic_shoulder;
      int m_run_total;
      double m_run_rphi_mean;
      double m_run_z_mean;
      double m_beam_x;
      double m_beam_y;
      double m_beam_z;

      HIHist1D* m_hist_closest_inter;
      HIHist1D* m_histclose_closest_inter;
      HIHist1D* m_hist_weighted_z;
      HIHist1D* m_histclose_weighted_z;
      HIHist1D* m_hist_neutral_energy;
      HIHist1D* m_histclose_neutral_energy;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* BEAMGASFILTERPROC_BEAMGASFILTERPROC_H */
