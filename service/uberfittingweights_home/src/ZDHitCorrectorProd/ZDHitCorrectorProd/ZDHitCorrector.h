#if !defined(PACKAGE_ZDHITCORRECTOR_H)
#define PACKAGE_ZDHITCORRECTOR_H
// -*- C++ -*-
//
// Package:     ZDHitCorrector
// Module:      ZDHitCorrector
// 
// Description: contains routines to calculate corrections
//              for ZD hits in time and space domains, along
//              with routine to create CorrectedHit objects
//
// Requires:
//     CalibratedData,Geometry,ZDDriftFunctionProd(ZDDriftFunction),
//     Constants for drift cell topology corrections
//
// Author:      Guangshun Huang
// Created:     Thu Feb 27 2003
// $Id: ZDHitCorrector.h,v 1.1.1.1 2003/08/30 00:17:08 wsun Exp $
//
// Revision history
//
// $Log: ZDHitCorrector.h,v $
// Revision 1.1.1.1  2003/08/30 00:17:08  wsun
// Imported ZDHitCorrectorProd sources.
//
//

// #include "Experiment/Experiment.h"
// //
// // user include files
// //
// #include "CalibratedData/CalibratedZDHit.h"
// #include "DRHitCorrectorProd/HitCorrector.h"

// class KTHelix ;
// class AZDSenseWireStore;
// class ZDDriftFunction;
// class MagneticField;

// class ZDHitCorrector : public HitCorrector
// {
//    public:
//       ZDHitCorrector();
//       ZDHitCorrector(
// 		   const ZDDriftFunction* calibration,
// 		   const AZDSenseWireStore* geometry,
// 		   const MagneticField* field
// 		   );
//       virtual ~ZDHitCorrector();

//       virtual void correctDriftTimeAndDist(
// 	 const KTHelix& xtrack,          // input
// //	 const CalibratedZDHit& hit,     // input
// 	 const CalibratedDRHit& hit,     // input
// 	 GeV     mass,                   // input
// 	 Meters  signeddrift,            // input
// 	 double averagedCharge,          // input
// 	 CalibratedTimeHit::PicoSec& aCorrectedTime,         // OUTPUT
// 	 Meters&  aCorrectedDriftDist,    // OUTPUT
// 	 Meters&  resolutionValue         // OUTPUT
// 	 ) const ;

//    protected:
//       virtual void correctDriftTimeAndDist(
// 	 const KTHelix& xtrack,          // input
// //	 const CalibratedZDHit& hit,     // input
// 	 const CalibratedDRHit& hit,     // input
// 	 GeV     mass,                   // input
// 	 Meters  signeddrift,            // input
// 	 CalibratedTimeHit::PicoSec& aCorrectedTime,         // OUTPUT
// 	 Meters&  aCorrectedDriftDist     // OUTPUT
// 	 ) const ;

//    private:
//       // ---------- Constructors and destructor ----------------
//       ZDHitCorrector( const ZDHitCorrector& ); // stop default

//       // ---------- assignment operator(s) ---------------------
//       const ZDHitCorrector& operator=( const ZDHitCorrector& ); // stop default

//       const AZDSenseWireStore* m_zdGeometry;
//       const ZDDriftFunction* m_zdCalibration;
// };

#include "WireHitCorrection/WHCDeviceHitCorrector.h"
#include "ZDDriftFunctionProd/ZDDriftFunction.h"
#include "AZDGeom/AZDSenseWireStore.h"

typedef
WHCDeviceHitCorrector< ZDDriftFunction, AZDSenseWireStore > ZDHitCorrector ;

#endif /* PACKAGE_ZDHITCORRECTOR_H */
