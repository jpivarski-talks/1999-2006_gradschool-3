#if !defined(CHISQFITPRODUCER_CHISQFITPRODUCER_H)
#define CHISQFITPRODUCER_CHISQFITPRODUCER_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqFitProducer
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 00:06:11 EDT 1999
// $Id: ChisqFitProducer.h,v 1.9 2003/04/17 17:00:42 bkh Exp $
//
// Revision history
//
// $Log: ChisqFitProducer.h,v $
// Revision 1.9  2003/04/17 17:00:42  bkh
// Add ZD options & throw exceptions for wrong config
//
// Revision 1.8  2000/11/18 00:21:42  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.7  2000/09/05 20:43:19  wsun
// Mods to allow user to force fitting weight to a given value.
//
// Revision 1.6  2000/08/16 21:06:24  wsun
// Added usage and production tags for TRSeedTracks.
//
// Revision 1.5  2000/06/10 06:27:47  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.4  2000/01/11 20:18:10  wsun
// Added parameters to control exception handling.
//
// Revision 1.3  1999/07/07 16:45:08  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.2  1999/06/24 22:56:25  wsun
// Added use of HIMagField.
//
// Revision 1.1.1.1  1999/06/16 02:48:25  wsun
// First submission.
//
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "CommandPattern/Parameter.h"
#include "ToolBox/HistogramPackage.h"

// forward declarations

class ChisqFitProducer : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      ChisqFitProducer( void );                      // anal1 
      virtual ~ChisqFitProducer();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( TBHistoManager& );                  

      // ------------ const member functions ----------------------
      DABoolean printDiagnostics() const
      { return m_printDiagnostics.value() ; }

      DABoolean useDRAxialHits() const
      { return m_useDRAxialHits.value() ; }
      DABoolean useDRStereoUHits() const
      { return m_useDRStereoUHits.value() ; }
      DABoolean useDRStereoVHits() const
      { return m_useDRStereoVHits.value() ; }
      DABoolean correctDRHits() const { return m_correctDRHits.value() ; }

      DABoolean useZDStereoUHits() const
      { return m_useZDStereoUHits.value() ; }
      DABoolean useZDStereoVHits() const
      { return m_useZDStereoVHits.value() ; }
      DABoolean correctZDHits() const { return m_correctZDHits.value() ; }

      DABoolean useDRCathodeHits() const
      { return m_useDRCathodeHits.value() ; }
      DABoolean correctDRCathodeHits() const
      { return m_correctDRCathodeHits.value() ; }

      DABoolean useSVRPhiHits() const { return m_useSVRPhiHits.value() ; }
      DABoolean useSVZHits() const { return m_useSVZHits.value() ; }
      DABoolean correctSVRHits() const { return m_correctSVRHits.value() ; }
      DABoolean correctSVZHits() const { return m_correctSVZHits.value() ; }

      double maxHitChisq() const { return m_maxHitChisq.value() ; }
      double maxDeltaChisqNdof() const { return m_maxDeltaChisqNdof.value() ; }
      int maxIterations() const { return m_maxIterations.value() ; }
      double bfieldMagnitude() const { return m_bfieldMagnitude.value() ; }

      DABoolean fixCurvature() const { return m_fixCurv.value() ; }
      DABoolean fixPhi0() const { return m_fixPhi0.value() ; }
      DABoolean fixD0() const { return m_fixD0.value() ; }
      DABoolean fixCotTheta() const { return m_fixCotTh.value() ; }
      DABoolean fixZ0() const { return m_fixZ0.value() ; }

      DABoolean continueIfHitsMissing() const
      { return m_continueIfHitsMissing.value() ; }
      DABoolean continueIfAnyException() const
      { return m_continueIfAnyException.value() ; }

      const string& drHitUsageTag() const {
        return m_drHitUsageTag.value() ; }
      const string& drHitProductionTag() const {
        return m_drHitProdTag.value() ; }
      const string& zdHitUsageTag() const {
        return m_zdHitUsageTag.value() ; }
      const string& zdHitProductionTag() const {
        return m_zdHitProdTag.value() ; }
      const string& cathodeHitUsageTag() const {
        return m_cathodeHitUsageTag.value() ; }
      const string& cathodeHitProductionTag() const {
        return m_cathodeHitProdTag.value() ; }
      const string& svrHitUsageTag() const {
        return m_svrHitUsageTag.value() ; }
      const string& svrHitProductionTag() const {
        return m_svrHitProdTag.value() ; }
      const string& svzHitUsageTag() const {
        return m_svzHitUsageTag.value() ; }
      const string& svzHitProductionTag() const {
        return m_svzHitProdTag.value() ; }
      const string& seedTrackUsageTag() const {
	return m_seedTrackUsageTag.value() ; }
      const string& seedTrackProductionTag() const {
	return m_seedTrackProdTag.value() ; }

      DABoolean forceDRFittingWeight() const
      { return m_forceDRFittingWeight.value() ; }
      double drFittingWeight() const
      { return m_drFittingWeight.value() ; }
      DABoolean forceZDFittingWeight() const
      { return m_forceZDFittingWeight.value() ; }
      double zdFittingWeight() const
      { return m_zdFittingWeight.value() ; }
      DABoolean forceSVRFittingWeight() const
      { return m_forceSVRFittingWeight.value() ; }
      double svrFittingWeight() const
      { return m_svrFittingWeight.value() ; }
      DABoolean forceSVZFittingWeight() const
      { return m_forceSVZFittingWeight.value() ; }
      double svzFittingWeight() const
      { return m_svzFittingWeight.value() ; }
      DABoolean forceCathodeFittingWeight() const
      { return m_forceCathodeFittingWeight.value() ; }
      double cathodeFittingWeight() const
      { return m_cathodeFittingWeight.value() ; }
      DABoolean scaleCathodeResolution() const
      { return m_scaleCathodeResolution.value() ; }
      double cathodeResolutionScale() const
      { return m_cathodeResolutionScale.value() ; }

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      ChisqFitProducer( const ChisqFitProducer& );

      // ------------ assignment operator(s) ----------------------
      const ChisqFitProducer& operator=( const ChisqFitProducer& );

      // ------------ private member functions --------------------
      ProxyBase* makeChisqTrackFitterProxy() ;

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------
      Parameter< DABoolean > m_printDiagnostics ;
      Parameter< DABoolean > m_useDRAxialHits ;
      Parameter< DABoolean > m_useDRStereoUHits ;
      Parameter< DABoolean > m_useDRStereoVHits ;
      Parameter< DABoolean > m_correctDRHits ;

      Parameter< DABoolean > m_useZDStereoUHits ;
      Parameter< DABoolean > m_useZDStereoVHits ;
      Parameter< DABoolean > m_correctZDHits ;

      Parameter< DABoolean > m_useDRCathodeHits ;
      Parameter< DABoolean > m_correctDRCathodeHits ;

      Parameter< DABoolean > m_useSVRPhiHits ;
      Parameter< DABoolean > m_useSVZHits ;
      Parameter< DABoolean > m_correctSVRHits ;
      Parameter< DABoolean > m_correctSVZHits ;

      Parameter< double > m_maxHitChisq ;
      Parameter< double > m_maxDeltaChisqNdof ;
      Parameter< int > m_maxIterations ;
      Parameter< double > m_bfieldMagnitude ;

      Parameter< DABoolean > m_fixCurv ;
      Parameter< DABoolean > m_fixPhi0 ;
      Parameter< DABoolean > m_fixD0 ;
      Parameter< DABoolean > m_fixCotTh ;
      Parameter< DABoolean > m_fixZ0 ;

      Parameter< DABoolean > m_continueIfHitsMissing ;
      Parameter< DABoolean > m_continueIfAnyException ;

      Parameter< string > m_drHitUsageTag ;
      Parameter< string > m_drHitProdTag ;
      Parameter< string > m_zdHitUsageTag ;
      Parameter< string > m_zdHitProdTag ;
      Parameter< string > m_cathodeHitUsageTag ;
      Parameter< string > m_cathodeHitProdTag ;
      Parameter< string > m_svrHitUsageTag ;
      Parameter< string > m_svrHitProdTag ;
      Parameter< string > m_svzHitUsageTag ;
      Parameter< string > m_svzHitProdTag ;
      Parameter< string > m_seedTrackUsageTag ;
      Parameter< string > m_seedTrackProdTag ;

      Parameter< DABoolean > m_forceDRFittingWeight ;
      Parameter< double > m_drFittingWeight ;
      Parameter< DABoolean > m_forceZDFittingWeight ;
      Parameter< double > m_zdFittingWeight ;
      Parameter< DABoolean > m_forceSVRFittingWeight ;
      Parameter< double > m_svrFittingWeight ;
      Parameter< DABoolean > m_forceSVZFittingWeight ;
      Parameter< double > m_svzFittingWeight ;
      Parameter< DABoolean > m_forceCathodeFittingWeight ;
      Parameter< double > m_cathodeFittingWeight ;
      Parameter< DABoolean > m_scaleCathodeResolution ;
      Parameter< double > m_cathodeResolutionScale ;

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* CHISQFITPRODUCER_CHISQFITPRODUCER_H */
