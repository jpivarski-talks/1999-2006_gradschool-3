#if !defined(KALMANPROD_KALMANPROD_H)
#define KALMANPROD_KALMANPROD_H
// -*- C++ -*-
//
// Package:     <KalmanProd>
// Module:      KalmanProd
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue May 11 02:22:32 EDT 1999
// $Id: KalmanProd.h,v 1.14 2002/02/23 06:51:04 wsun Exp $
//
// Revision history
//
// $Log: KalmanProd.h,v $
// Revision 1.14  2002/02/23 06:51:04  wsun
// Added flag for applying B field correction.
//
// Revision 1.13  2002/02/22 03:34:00  wsun
// Change outwardFitIsInwardSeed to inwardFitIsOutwardSeed.
//
// Revision 1.12  2000/12/13 07:04:19  wsun
// Added option to not have inward fits propagated to the origin.
//
// Revision 1.11  2000/11/18 00:18:37  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.10  2000/09/05 20:48:19  wsun
// Mods to allow user to force fitting weight to a given value.
//
// Revision 1.9  2000/08/16 20:19:12  wsun
// Added usage and production tags for seed tracks.
//
// Revision 1.8  2000/07/27 23:30:05  wsun
// Added parameter for specifying usage and and production tags for hits.
//
// Revision 1.7  2000/06/10 05:56:50  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.6  2000/06/09 00:56:30  wsun
// Added parameters for selecting hypotheses to be fit and which
// hypothesis determines the hit list.
//
// Revision 1.5  2000/06/06 22:17:49  wsun
// Added parameter CorrectDRHits, changed MaxHitsToDelete to unsigned int.
//
// Revision 1.4  2000/02/03 18:54:17  wsun
// Added parameters MaxHitsToDelete and MaxHitResidualPull.
//
// Revision 1.3  2000/01/11 20:39:07  wsun
// Added parameters to control exception handling.
//
// Revision 1.2  1999/10/08 23:23:57  wsun
// * Added param to optimize inward+outward fitting for a single mass hypo.
// * Added param to optimize fitting of all tracks to all hypotheses.
//
// Revision 1.1.1.1  1999/09/10 21:58:57  wsun
// Transfered from KalmanFilter
//
// Revision 1.6  1999/08/30 21:49:33  wsun
// Added parameters for dE/dx straggling.
//
// Revision 1.5  1999/07/07 16:46:26  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.4  1999/06/24 22:44:14  wsun
// Added use of HIMagField and new SiHits.
//
// Revision 1.3  1999/06/18 19:27:36  wsun
// More improvements to low momentum fitting.
//
// Revision 1.2  1999/06/02 23:07:29  wsun
// Improved low-momentum fitting.
//
// Revision 1.1  1999/05/14 01:11:03  wsun
// First submission.
//
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "ToolBox/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class KalmanProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------
      static const STL_VECTOR( string ) AllHypos ;  // empty vector

      // ------------ Constructors and destructor -----------------
      KalmanProd( void );                      // anal1 
      virtual ~KalmanProd();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( TBHistoManager& );                  

      // ------------ const member functions ----------------------
      DABoolean printDiagnostics() const { return m_printDiagnostics.value(); }
      DABoolean useDRAxialHits() const { return m_useDRAxialHits.value(); }
      DABoolean useDRStereoUHits() const { return m_useDRStereoUHits.value(); }
      DABoolean useDRStereoVHits() const { return m_useDRStereoVHits.value(); }
      DABoolean correctDRHits() const { return m_correctDRHits.value() ; }

      DABoolean useDRCathodeHits() const { return m_useDRCathodeHits.value(); }
      DABoolean correctDRCathodeHits() const
      { return m_correctDRCathodeHits.value() ; }

      DABoolean useSVRPhiHits() const { return m_useSVRPhiHits.value() ; }
      DABoolean useSVZHits() const { return m_useSVZHits.value() ; }
      DABoolean correctSVRHits() const { return m_correctSVRHits.value() ; }
      DABoolean correctSVZHits() const { return m_correctSVZHits.value() ; }

      DABoolean applyDedx() const { return m_applyDedx.value() ; }
      DABoolean applyStraggling() const { return m_applyStraggling.value() ; }
      double stragglingCoeff() const { return m_stragglingCoeff.value() ; }
      DABoolean applyMultScat() const { return m_applyMultScat.value() ; }
      DABoolean applyBfieldCorrections() const {
	 return m_applyBfieldCorrections.value() ; }

      double maxHitChisq() const { return m_maxHitChisq.value() ; }
      double updateErr2Z0() const { return m_updateErr2Z0.value() ; }
      double updateErr2CotTheta() const { return m_updateErr2CotTheta.value();}
      double updateDFracCotTheta() const
      { return m_updateDFracCotTheta.value() ; }
      double maxFracDeltaP() const { return m_maxFracDeltaP.value() ; }
      double bfieldMagnitude() const { return m_bfieldMagnitude.value() ; }

      DABoolean fixCurvature() const { return m_fixCurv.value() ; }
      DABoolean fixPhi0() const { return m_fixPhi0.value() ; }
      DABoolean fixD0() const { return m_fixD0.value() ; }
      DABoolean fixCotTheta() const { return m_fixCotTh.value() ; }
      DABoolean fixZ0() const { return m_fixZ0.value() ; }

      DABoolean optimizeForAllHypos() const
      { return m_optimizeForAllHypos.value() ; }
      DABoolean inwardFitIsOutwardSeed() const
      { return m_inwardFitIsOutwardSeed.value() ; }

      DABoolean continueIfHitsMissing() const
      { return m_continueIfHitsMissing.value() ; }
      DABoolean continueIfAnyException() const
      { return m_continueIfAnyException.value() ; }

      int maxHitsToDelete() const { return m_maxHitsToDelete.value() ; }
      double maxHitResidualPull() const { return m_maxHitResidualPull.value();}

      // An empty list means all hypos are to be fit.
      const STL_VECTOR( string )& hyposToFit() const
      { return m_hyposToFit.value() ;}

      DABoolean hitListFromOneHypo() const
      { return m_hitListFromOneHypo.value() ; }
      const string& hitListHypo() const { return m_hitListHypo.value() ; }

      const string& drHitUsageTag() const {
	return m_drHitUsageTag.value() ; }
      const string& drHitProductionTag() const {
	return m_drHitProdTag.value() ; }
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

      DABoolean inwardFitsAtLastHit() const
      { return m_inwardFitsAtLastHit.value() ; }

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      KalmanProd( const KalmanProd& );

      // ------------ assignment operator(s) ----------------------
      const KalmanProd& operator=( const KalmanProd& );

      // ------------ private member functions --------------------
      ProxyBase* makeKalmanTrackFitterProxy() ;

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------
      Parameter< DABoolean > m_printDiagnostics ;
      Parameter< DABoolean > m_useDRAxialHits ;
      Parameter< DABoolean > m_useDRStereoUHits ;
      Parameter< DABoolean > m_useDRStereoVHits ;
      Parameter< DABoolean > m_correctDRHits ;

      Parameter< DABoolean > m_useDRCathodeHits ;
      Parameter< DABoolean > m_correctDRCathodeHits ;

      Parameter< DABoolean > m_useSVRPhiHits ;
      Parameter< DABoolean > m_useSVZHits ;
      Parameter< DABoolean > m_correctSVRHits ;
      Parameter< DABoolean > m_correctSVZHits ;

      Parameter< DABoolean > m_applyDedx ;
      Parameter< DABoolean > m_applyStraggling ;
      Parameter< double >    m_stragglingCoeff ;
      Parameter< DABoolean > m_applyMultScat ;
      Parameter< DABoolean > m_applyBfieldCorrections ;

      Parameter< double >    m_maxHitChisq ;
      Parameter< double >    m_updateErr2Z0 ;
      Parameter< double >    m_updateErr2CotTheta ;
      Parameter< double >    m_updateDFracCotTheta ;
      Parameter< double >    m_maxFracDeltaP ;
      Parameter< double >    m_bfieldMagnitude ;

      Parameter< DABoolean > m_fixCurv ;
      Parameter< DABoolean > m_fixPhi0 ;
      Parameter< DABoolean > m_fixD0 ;
      Parameter< DABoolean > m_fixCotTh ;
      Parameter< DABoolean > m_fixZ0 ;

      Parameter< DABoolean > m_optimizeForAllHypos ;
      Parameter< DABoolean > m_inwardFitIsOutwardSeed ;

      Parameter< DABoolean > m_continueIfHitsMissing ;
      Parameter< DABoolean > m_continueIfAnyException ;

      //      Parameter< int > m_maxHitsToDelete ;
      Parameter< unsigned int > m_maxHitsToDelete ;
      Parameter< double > m_maxHitResidualPull ;

      // An empty vector means all hypos are to be fit.
      Parameter< STL_VECTOR( string ) > m_hyposToFit ;

      Parameter< DABoolean > m_hitListFromOneHypo ;
      Parameter< string > m_hitListHypo ;

      Parameter< string > m_drHitUsageTag ;
      Parameter< string > m_drHitProdTag ;
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
      Parameter< DABoolean > m_forceSVRFittingWeight ;
      Parameter< double > m_svrFittingWeight ;
      Parameter< DABoolean > m_forceSVZFittingWeight ;
      Parameter< double > m_svzFittingWeight ;
      Parameter< DABoolean > m_forceCathodeFittingWeight ;
      Parameter< double > m_cathodeFittingWeight ;
      Parameter< DABoolean > m_scaleCathodeResolution ;
      Parameter< double > m_cathodeResolutionScale ;

      Parameter< DABoolean > m_inwardFitsAtLastHit ;

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* KALMANPROD_KALMANPROD_H */
