// -*- C++ -*-
#if !defined(HIGHCOTTHETATRACKSPLITTING_HIGHCOTTHETATRACKSPLITTING_H)
#define HIGHCOTTHETATRACKSPLITTING_HIGHCOTTHETATRACKSPLITTING_H
//
// Package:     <HighCotThetaTrackSplitting>
// Module:      HighCotThetaTrackSplitting
//
/**\class HighCotThetaTrackSplitting HighCotThetaTrackSplitting.h HighCotThetaTrackSplitting/HighCotThetaTrackSplitting.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Mon Feb 26 10:28:13 EST 2001
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

#include "HbookHistogram/HbookNtuple.h"
#include "Navigation/NavTrack.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "C3cc/CcEventSummary.h"

#include "Navigation/NavShower.h"

#include "C3cc/CcShowerAttributes.h"
#include "Navigation/NavConReg.h"
#include "KinematicTrajectory/KTKinematicData.h"
#include "Navigation/NavTkShMatch.h"
#include "C3cc/CcAssignedEnergyHit.h"

// external functions
extern "C" float prob_( const float&, const int& );

// forward declarations

class HighCotThetaTrackSplitting : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      HighCotThetaTrackSplitting( void );                      // anal1 
      virtual ~HighCotThetaTrackSplitting();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
      //virtual ActionBase::ActionResult beginRun( Frame& iFrame);
      virtual ActionBase::ActionResult startRun( Frame& iFrame);
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
      HighCotThetaTrackSplitting( const HighCotThetaTrackSplitting& );

      // ------------ assignment operator(s) ---------------------
      const HighCotThetaTrackSplitting& operator=(
	 const HighCotThetaTrackSplitting& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (HighCotThetaTrackSplitting::*iMethod)(
	    Frame& ), const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      DABoolean m_track_finished;
      int m_tracks_seen;
      HINtuple* m_trackvars;
      HINtuple* m_eventvars;

      HepVector3D m_bField;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* HIGHCOTTHETATRACKSPLITTING_HIGHCOTTHETATRACKSPLITTING_H */
