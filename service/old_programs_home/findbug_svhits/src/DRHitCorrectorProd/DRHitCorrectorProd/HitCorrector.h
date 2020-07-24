#if !defined(PACKAGE_HITCORRECTOR_H)
#define PACKAGE_HITCORRECTOR_H
// -*- C++ -*-
//
// Package:     DRHitCorrector
// Module:      HitCorrector
// 
// Description: contains routines to calculate corrections
//              for DR hits in time and space domains, along
//              with routine to create CorrectedHit objects
//
// Requires:
//     CalibratedData,Geometry,CDOffCal(DriftFunction),
//     Constants for drift cell topology corrections
//
// Author:      Ken McLean
// Created:     Wed Jul 28 16:55:15 EDT 1999
// $Id: HitCorrector.h,v 1.12 2001/11/02 20:25:33 dubrovin Exp $
//
// Revision history
//
// $Log: HitCorrector.h,v $
// Revision 1.12  2001/11/02 20:25:33  dubrovin
// Move internal correctDriftTimeAndDist to private
//
// Revision 1.11  2000/12/13 22:33:18  urner
// change default of standard correction parameter and entrance angle parameter to true
//
// Revision 1.10  2000/12/02 02:24:26  urner
// added functionality to correct the drift time of hits with no charge measurement for the average charge fo the hits on track with valid charge
//
// Revision 1.9  2000/11/01 22:49:09  mclean
// Switched include of CorrectedDRHit.h to CalibratedData/CalibratedDRHit.h
// the former was deleted because we never used CorrectedDRHit's: I didn't
// noticed the dependence (which was solely thru its existence as an
// #include line in HitCorrector.h) because when I deleted the local copy
// it was resolved from the dlib release then extant.
//
// Revision 1.8  2000/10/31 22:56:38  mclean
// CorrectedDRHit* used to "make" CorrectedDRHit's, we never actually used
// these, relying on storing the corrected values in link data, so now
// its gone forever!
//
// Deleted sigPropBeta parameter from DRHitCorrectorProd,HitCorrector
// and HitCorrectorProxy, this now comes from CDOffCal via the call to
// ZCorr added to HitCorrector.
//
// In HitCorrector.cc, correctedDriftTimeAndDist is overloaded with
// one additional argument the resolution, if this form of the
// function is called by the track fitter then the corrected resolution
// will be returned (fitting weight implimentation).
//
// The code is slightly reorganized and the user is blessed with rare
// informational messages about  which corrections are being applied.
//
// Additional error checking for failed track swims is included.
// The old drift function call is replaced by time2SignedDrift which
// will, if available return values for a two sided drift function.
// Less arguments are required since CDOffCal access the geometry from
// the frame internally.
//
// If, as when entrance angle corrections are enabled,  a 4th and 5th argument
// is added (entrance angle and zhit) then entrance angle corrections are added
// inside CDOffCal by overloading time2SignedDrift.
//
// Revision 1.7  2000/06/29 00:05:36  wsun
// STL includes for OSF1 compilation, new data member m_signalPropBeta.
//
// Revision 1.6  2000/06/27 03:16:02  wsun
// Added parameters for controlling each correction individually.
//
// Revision 1.5  2000/06/22 14:27:45  cdj
// removed TBThreeVector/Point, made constant variables nanames begin with k and removed unnecessary includes
//
// Revision 1.4  2000/06/09 00:15:26  wsun
// Added correctDriftTimeAndDist().
//
// Revision 1.3  1999/08/26 21:19:56  mclean
// Add signeddriftdistance to argument list. This should be passed in
// to give CorrectedDRHit the hit ambiguity as determined by the Finder.
//
// Revision 1.2  1999/08/21 14:40:52  mclean
// Add corrections for flight time and signal propagation over sense wires.
//
#include "Experiment/Experiment.h"
//
// user include files
//
#include "CalibratedData/CalibratedDRHit.h"

class KTHelix ;
class ADRSenseWireStore;
class DriftFunction;
class MagneticField;

class HitCorrector
{
   public:
      HitCorrector();
      HitCorrector(
		   const DriftFunction* calibration,
		   const ADRSenseWireStore* geometry,
		   const MagneticField* field
		   );
      virtual ~HitCorrector();

      void correctDriftTimeAndDist(
	 const KTHelix& xtrack,          // input
	 const CalibratedDRHit& hit,     // input
	 GeV     mass,                   // input
	 Meters  signeddrift,            // input
	 double averagedCharge,          // input
	 CalibratedTimeHit::PicoSec& aCorrectedTime,         // OUTPUT
	 Meters&  aCorrectedDriftDist,    // OUTPUT
	 Meters&  resolutionValue         // OUTPUT
	 ) const ;

      // This overrides all other parameters except m_disableAllCorrections.
      void setApplyStandardCorrections()
      {
	 m_applySignalPropCorr    = true ;
	 m_applyTimeOfFlightCorr  = true ;
	 m_applyPulseHeightCorr   = true ;
	 m_applyEntranceAngleCorr = true ;
      }

      // This overrides ALL other parameters.
      void setDisableAllCorrections( DABoolean aFlag )
      { m_disableAllCorrections = aFlag ; }

      void setApplySignalPropCorr( DABoolean aFlag )
      { m_applySignalPropCorr = aFlag ; }

      void setApplyTimeOfFlightCorr( DABoolean aFlag )
      { m_applyTimeOfFlightCorr = aFlag ; }

      void setApplyPulseHeightCorr( DABoolean aFlag )
      { m_applyPulseHeightCorr = aFlag ; }

      void setApplyEntranceAngleCorr( DABoolean aFlag )
      { m_applyEntranceAngleCorr = aFlag ; }

   protected:
   private:
      // ---------- Constructors and destructor ----------------
      HitCorrector( const HitCorrector& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HitCorrector& operator=( const HitCorrector& ); // stop default

      void correctDriftTimeAndDist(
	 const KTHelix& xtrack,          // input
	 const CalibratedDRHit& hit,     // input
	 GeV     mass,                   // input
	 Meters  signeddrift,            // input
	 CalibratedTimeHit::PicoSec& aCorrectedTime,         // OUTPUT
	 Meters&  aCorrectedDriftDist     // OUTPUT
	 ) const ;


      const MagneticField* m_field;
      const ADRSenseWireStore* m_geometry;
      const DriftFunction* m_calibration;

      DABoolean m_disableAllCorrections ;
      DABoolean m_applySignalPropCorr ;
      DABoolean m_applyTimeOfFlightCorr ;
      DABoolean m_applyPulseHeightCorr ;
      DABoolean m_applyEntranceAngleCorr ;

};
#endif /* PACKAGE_HITCORRECTOR_H */









