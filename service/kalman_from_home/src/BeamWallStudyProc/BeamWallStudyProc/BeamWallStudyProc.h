// -*- C++ -*-
#if !defined(BEAMWALLSTUDYPROC_BEAMWALLSTUDYPROC_H)
#define BEAMWALLSTUDYPROC_BEAMWALLSTUDYPROC_H
//
// Package:     <BeamWallStudyProc>
// Module:      BeamWallStudyProc
//
/**\class BeamWallStudyProc BeamWallStudyProc.h BeamWallStudyProc/BeamWallStudyProc.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Oct 29 13:58:18 EST 2002
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
#include "KinematicTrajectory/KTHelix.h"

// forward declarations

class BeamWallStudyProc : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      BeamWallStudyProc( void );                      // anal1 
      virtual ~BeamWallStudyProc();                   // anal5 

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

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      BeamWallStudyProc( const BeamWallStudyProc& );

      // ------------ assignment operator(s) ---------------------
      const BeamWallStudyProc& operator=( const BeamWallStudyProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (BeamWallStudyProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      double get_weighted_z0( Frame& iFrame );
      double get_closest_intersection( Frame& iFrame );
      double get_weighted_ciz( Frame& iFrame );
      DABoolean calc_intersection(
	 double& x, double& y, double& z, double& sig2z, double& zsep,
	 KTHelix& a, KTHelix& b );
      DABoolean arclength_test( double x, double y, KTHelix& a, KTHelix& b );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      double m_beam_x;
      double m_beam_y;
      double m_beam_z;

      HIHist1D* m_ci_close;
      HIHist1D* m_ci_far;
      HIHist1D* m_d0_close_pos;
      HIHist1D* m_d0_far_pos;
      HIHist1D* m_d0_close_neg;
      HIHist1D* m_d0_far_neg;
      HIHist1D* m_zdiff_close_pos;
      HIHist1D* m_zdiff_closer_pos;
      HIHist1D* m_zdiff_far_pos;
      HIHist1D* m_zdiff_close_neg;
      HIHist1D* m_zdiff_closer_neg;
      HIHist1D* m_zdiff_far_neg;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* BEAMWALLSTUDYPROC_BEAMWALLSTUDYPROC_H */
