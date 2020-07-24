#if !defined(DRHITCORRECTORPROD_DRHITCORRECTORPROD_H)
#define DRHITCORRECTORPROD_DRHITCORRECTORPROD_H
// -*- C++ -*-
//
// Package:     <DRHitCorrectorProd>
// Module:      DRHitCorrectorProd
// 
// Description:
//
// Usage:
//   
//
// Author:      Ken McLean
// Created:     Wed Jul 28 16:40:54 EDT 1999
// $Id: DRHitCorrectorProd.h,v 1.6 2000/10/31 22:56:38 mclean Exp $
//
// Revision history
//
// $Log: DRHitCorrectorProd.h,v $
// Revision 1.6  2000/10/31 22:56:38  mclean
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
// Revision 1.5  2000/06/29 00:04:28  wsun
// Added parameter for signal propagation speed in DR wires.
//
// Revision 1.4  2000/06/27 03:16:01  wsun
// Added parameters for controlling each correction individually.
//
// Revision 1.3  1999/08/26 21:19:56  mclean
// Add signeddriftdistance to argument list. This should be passed in
// to give CorrectedDRHit the hit ambiguity as determined by the Finder.
//
// Revision 1.2  1999/08/21 14:40:51  mclean
// Add corrections for flight time and signal propagation over sense wires.
//
//
// system include files
//
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
//
// user include files
//
#include "Processor/Producer.h"
#include "ToolBox/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

// forward declarations
#include "STLUtility/fwd_vector.h"
class HitCorrectorProxy ;

class DRHitCorrectorProd : public Producer
{
   public:
      // ------------ Constructors and destructor -----------------
      DRHitCorrectorProd( void );                      // anal1 
      virtual ~DRHitCorrectorProd();                   // anal5 

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( TBHistoManager& );                  

      DABoolean applyStandardCorrections() const
      { return m_applyStandardCorrections.value() ; }

      DABoolean disableAllCorrections() const
      { return m_disableAllCorrections.value() ; }

      DABoolean applySignalPropCorr() const
      { return m_applySignalPropCorr.value() ; }

      DABoolean applyTimeOfFlightCorr() const
      { return m_applyTimeOfFlightCorr.value() ; }

      DABoolean applyPulseHeightCorr() const
      { return m_applyPulseHeightCorr.value() ; }

      DABoolean applyEntranceAngleCorr() const
      { return m_applyEntranceAngleCorr.value() ; }

   protected:
      ProxyBase* makeProxy();

   private:
      // ------------ Constructors and destructor -----------------
      DRHitCorrectorProd( const DRHitCorrectorProd& );

      // ------------ assignment operator(s) ----------------------
      const DRHitCorrectorProd& operator=( const DRHitCorrectorProd& );

      // ------------ data members --------------------------------
      HitCorrectorProxy*  m_HitCorrectorProxy ;  

      Parameter< DABoolean > m_applyStandardCorrections ;
      Parameter< DABoolean > m_disableAllCorrections ;
      Parameter< DABoolean > m_applySignalPropCorr ;
      Parameter< DABoolean > m_applyTimeOfFlightCorr ;
      Parameter< DABoolean > m_applyPulseHeightCorr ;
      Parameter< DABoolean > m_applyEntranceAngleCorr ;
};
#endif /* DRHITCORRECTORPROD_DRHITCORRECTORPROD_H */
