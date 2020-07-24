#if !defined(FITTERDIAGNOSTICSPROC_FITTERDIAGNOSTICSPROC_H)
#define FITTERDIAGNOSTICSPROC_FITTERDIAGNOSTICSPROC_H
// -*- C++ -*-
//
// Package:     <FitterDiagnosticsProc>
// Module:      FitterDiagnosticsProc
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Wed Apr 21 16:57:07 EDT 1999
// $Id: FitterDiagnosticsProc.h,v 1.5 2002/02/25 21:52:12 wsun Exp $
//
// Revision history
//
// $Log: FitterDiagnosticsProc.h,v $
// Revision 1.5  2002/02/25 21:52:12  wsun
// Added Kalman quality info.
//
// Revision 1.4  2000/12/20 22:33:18  wsun
// Parameter to set mass nypothesis to extract.
//
// Revision 1.3  2000/06/30 18:12:21  wsun
// Added ntuple fields for drift distance.
//
// Revision 1.2  2000/02/09 03:18:22  wsun
// Certain exceptions now caught.
//
// Revision 1.1.1.1  1999/06/07 21:26:37  wsun
// First submission.
//
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "ToolBox/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

// forward declarations

class FitterDiagnosticsProc : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum ntupleFields {
	 kResid1, kResid2, kResid3, kResid4,
	 kResid5, kResid6, kResid7, kResid8,
	 kResid9, kResid10, kResid11, kResid12,
	 kResid13, kResid14, kResid15, kResid16,
	 kResid17, kResid18, kResid19, kResid20,
	 kResid21, kResid22, kResid23, kResid24,
	 kResid25, kResid26, kResid27, kResid28,
	 kResid29, kResid30, kResid31, kResid32,
	 kResid33, kResid34, kResid35, kResid36,
	 kResid37, kResid38, kResid39, kResid40,
	 kResid41, kResid42, kResid43, kResid44,
	 kResid45, kResid46, kResid47,
	 kResErr1, kResErr2, kResErr3, kResErr4,
	 kResErr5, kResErr6, kResErr7, kResErr8,
	 kResErr9, kResErr10, kResErr11, kResErr12,
	 kResErr13, kResErr14, kResErr15, kResErr16,
	 kResErr17, kResErr18, kResErr19, kResErr20,
	 kResErr21, kResErr22, kResErr23, kResErr24,
	 kResErr25, kResErr26, kResErr27, kResErr28,
	 kResErr29, kResErr30, kResErr31, kResErr32,
	 kResErr33, kResErr34, kResErr35, kResErr36,
	 kResErr37, kResErr38, kResErr39, kResErr40,
	 kResErr41, kResErr42, kResErr43, kResErr44,
	 kResErr45, kResErr46, kResErr47,
	 kDriftDist1, kDriftDist2, kDriftDist3, kDriftDist4,
	 kDriftDist5, kDriftDist6, kDriftDist7, kDriftDist8,
	 kDriftDist9, kDriftDist10, kDriftDist11, kDriftDist12,
	 kDriftDist13, kDriftDist14, kDriftDist15, kDriftDist16,
	 kDriftDist17, kDriftDist18, kDriftDist19, kDriftDist20,
	 kDriftDist21, kDriftDist22, kDriftDist23, kDriftDist24,
	 kDriftDist25, kDriftDist26, kDriftDist27, kDriftDist28,
	 kDriftDist29, kDriftDist30, kDriftDist31, kDriftDist32,
	 kDriftDist33, kDriftDist34, kDriftDist35, kDriftDist36,
	 kDriftDist37, kDriftDist38, kDriftDist39, kDriftDist40,
	 kDriftDist41, kDriftDist42, kDriftDist43, kDriftDist44,
	 kDriftDist45, kDriftDist46, kDriftDist47,
	 kSVR1, kSVR2, kSVR3, kSVR4,
	 kSVRErr1, kSVRErr2, kSVRErr3, kSVRErr4,
	 kSVZ1, kSVZ2, kSVZ3, kSVZ4,
	 kSVZErr1, kSVZErr2, kSVZErr3, kSVZErr4,
	 kCathode, kCathodeErr,
	 kMomTrue, kCurvTrue, kPhi0True, kD0True, kCotthTrue, kZ0True,
	 kCosthTrue, kNhitTrue,
	 kMomFit, kCurvFit, kPhi0Fit, kD0Fit, kCotthFit, kZ0Fit, kCosthFit,
	 kMomErr, kCurvErr, kPhi0Err, kD0Err, kCotthErr, kZ0Err,
	 kChisq, kNdof, kCL, kTrack, kEvent, kSeedsEv, kDeltaE, kArcSwum,
	 kTracksNtupleSize } ;

      // ------------ Constructors and destructor ----------------
      FitterDiagnosticsProc( void );                      // anal1 
      virtual ~FitterDiagnosticsProc();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( TBHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
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
      FitterDiagnosticsProc( const FitterDiagnosticsProc& );

      // ------------ assignment operator(s) ---------------------
      const FitterDiagnosticsProc& operator=( const FitterDiagnosticsProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (FitterDiagnosticsProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------
      TBNtuple* m_fittedTrackNtuple ;

      Parameter< string > m_fitHypo ;
      Parameter< double > m_beamSpotX ;
      Parameter< double > m_beamSpotY ;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* FITTERDIAGNOSTICSPROC_FITTERDIAGNOSTICSPROC_H */
