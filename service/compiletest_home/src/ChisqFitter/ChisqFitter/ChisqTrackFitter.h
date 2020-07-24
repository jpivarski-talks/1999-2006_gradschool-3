#if !defined(CHISQFITTER_CHISQTRACKFITTER_H)
#define CHISQFITTER_CHISQTRACKFITTER_H
// -*- C++ -*-
//
// Package:     <ChisqFitter>
// Module:      ChisqTrackFitter
// 
// Description: <one line class summary>
//
// Any mention of fit direction is meaningless for chisq fits -- these
// variables are ignored by the fitter.
//
// Same for mass -- at least for now.
//
//
// Usage:
//
// If the user wishes to refit tracks, s/he should load ChisqFitProducer
// and extract ChisqTrackFitter from the Frame, because the proxy initializes
// the HIMagField singleton, which is used everywhere.  However, the
// extracted ChisqTrackFitter will have to be cast into a non-const version:
//
//     FAItem< ChisqTrackFitter > fitter ;
//     extract( eventRecord, fitter ) ;
//     ChisqTrackFitter* nonConstFitter =
//        const_cast< ChisqTrackFitter* >( &*fitter ) ;
//
// Alternatively, the user could initialize the magnetic field by hand with
//     HIMagField::instance()->setBfield( const Record& aBeginRunRecord )
// or
//     HIMagField::instance()->setBfield( double bfieldMagnitudeInKGauss )
//
//
// Author:      Werner Sun
// Created:     Wed Jun  9 02:10:32 EDT 1999
// $Id: ChisqTrackFitter.h,v 1.16 2003/07/25 23:24:43 mccann Exp $
//
// Revision history
//
// $Log: ChisqTrackFitter.h,v $
// Revision 1.16  2003/07/25 23:24:43  mccann
// Added the ZD to track fitting
//
// Revision 1.15  2003/04/17 16:59:05  bkh
// Add ZD options
//
// Revision 1.14  2000/11/18 00:20:14  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.13  2000/09/05 20:37:00  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.12  2000/08/16 20:30:39  wsun
// * Bug fix: hits were not really being dropped before.
// * Update to new TRTrackFitQuality.
// * Added usage and production tags for hits.
//
// Revision 1.11  2000/06/10 06:26:59  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.10  2000/02/28 23:14:25  lyon
// Add member functions to check exception and device parameter statuses
//
// Revision 1.9  2000/01/11 20:15:55  wsun
// Added flags to control exception handling.
//
// Revision 1.8  1999/10/05 22:57:40  wsun
// Implemented parameters for deselecting hits from a subdetector.
//
// Revision 1.7  1999/09/11 19:31:05  wsun
// Added DR cathodes.
//
// Revision 1.6  1999/08/03 22:41:44  wsun
// Add flag to disable extraction of hits attached to seeds.
//
// Revision 1.5  1999/07/07 16:44:05  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.4  1999/06/24 22:52:14  wsun
// * Added use of HIMagField and new SiHits.
// * Added ability to fix any combination of parameters in track fitting.
//
// Revision 1.3  1999/06/20 21:16:54  wsun
// Allow user to fix parameters.
//
// Revision 1.2  1999/06/18 19:16:22  wsun
// Allow user to fix parameters.
//
// Revision 1.1.1.1  1999/06/16 02:46:47  wsun
// First submission.
//

// system include files

// user include files
#include "TrackFitter/TrackFitter.h"
#include "HelixIntersection/HIDRSurfaceFactory.h"
#include "HelixIntersection/HIZDSurfaceFactory.h"

#include "ChisqFitter/ChisqHelix.h"
#include "ChisqFitter/ChisqAddHitOperation.h"

// forward declarations
#include "STLUtility/fwd_vector.h"

class CalibratedCathodeHit ;
class CalibratedSVRphiHit ;
class CalibratedSVZHit ;
class HIFitConstraint ;
class HITrackParameterConstraint ;
class HICompositeFitConstraint ;

class ChisqTrackFitter : public TrackFitter
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ChisqTrackFitter( DABoolean includeInitialErrorMatrix = false,
			double maxDeltaChisqNdof = 1.e10,
			int    maxIterations = 10,
			double maxHitChisq = 50.,
			HIFitConstraint* aConstraint = 0 ) ;
      virtual ~ChisqTrackFitter();

      // ---------- member functions ---------------------------

      // This function is called by event(), and, in turn, calls fittedTrack().
      // The new fitted track is kept in the TrackFitter's cache, which is
      // emptied by clearCache().
      virtual DABoolean fitTrack(
         const TRSeedTrack* theSeed,
         Frame& theFrame,
         DBCandidate::Hypo aHypo = DBCandidate::kChargedPion ) ;

      // The following four functions return a new KTHelix which the user
      // (or the Frame) is responsible for deleting.  The returned KTHelix will
      // hold the fitted track parameters, and the reference point will be
      // the current reference point of the seed.  The fitted helix is NOT
      // inserted into TrackFitter's internal vector.
      //
      // If the constraint pointer is non-null, it will be applied to the
      // helix parameters after the unconstrained fit has been performed
      // successfully.  The HIFitConstraint object is modified in the process.
      virtual KTHelix* fittedTrack(
	 const TRSeedTrack* theSeed,
	 Frame& theFrame,
         DBCandidate::Hypo aHypo = DBCandidate::kChargedPion ) ; // ignored

      virtual KTHelix* fittedTrack(
	 const TRSeedTrack* theSeed,
	 Frame& theFrame,
	 GeV aMass = 0.13957,   // ignored
	 FitDirection aFitDirection = kInwardFit ) ; // ignored

      // If the user supplies the hits by hand, the fitter will ignore
      // the ones attached to the seeds (via Lattice).
      //
      // If a hit vector pointer is null (default), then if
      // aExtractHitsIfNull is:
      //   * true, the Lattice-connected hits will be used.
      //   * false, then no hits of that type will be included in the fit.
      virtual KTHelix* fittedTrack(
	 const TRSeedTrack* theSeed,
	 Frame& theFrame,
	 const STL_VECTOR(
	    HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector = 0,
	 const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector = 0,
	 const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector = 0,
	 const STL_VECTOR( const CalibratedSVZHit* )* aZVector = 0,
	 const STL_VECTOR(
	    HIZDSurfaceFactory::ZDHitAndDriftDistance )* aZDVector = 0,
         DBCandidate::Hypo aHypo = DBCandidate::kChargedPion,  // ignored
	 DABoolean aExtractHitsIfNull = true ) ;

      virtual KTHelix* fittedTrack(
	 const TRSeedTrack* theSeed,
	 Frame& theFrame,
	 const STL_VECTOR(
	    HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector = 0,
	 const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector = 0,
	 const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector = 0,
	 const STL_VECTOR( const CalibratedSVZHit* )* aZVector = 0,
	 const STL_VECTOR(
	    HIZDSurfaceFactory::ZDHitAndDriftDistance )* aZDVector = 0,
	 GeV aMass = 0.13957,   // ignored
	 FitDirection aFitDirection = kInwardFit,  // ignored
	 DABoolean aExtractHitsIfNull = true ) ;

      void clearCache() ;

      // Set this flag to "true" if the seed tracks given to fittedTrack(...)
      // have an error matrix from a previous fit and the current fit adds
      // more hits (e.g. take a track fitted with DR hits only and add on
      // Si hits).  The error matrix of the seed will be saved and included
      // in the chisq to be minimized.
      void setIncludeInitErrorMatrix( DABoolean aFlag )
      { m_includeInitErrorMatrix = aFlag ; }

      // Set the direction to propagate the helix -- must be uniform
      // throughout the fit.  Default is KTMoveControl::kDirectionForward,
      // which is the correct setting for fitting hits on a single track
      // with the results reported at the production (or decay) point.
      // If the results are to be reported somewhere in the middle of the
      // trajectory, or for dual track fits, set this flag to
      // KTMoveControl::kDirectionEither.  This setting is NOT recommended
      // for curlers because the fitter will take the SHORTEST path from
      // the initial ref pt to the hit, which is incorrect for hits on the
      // back side of a curler.
      void setHelixMoveDirection( KTMoveControl::MoveDirection aDir )
      { m_moveDirection = aDir ; }

      // When the change in chisq/ndof dips below this number, consider the
      // fit to have converged.
      void setMaxDeltaChisqNdof( double aNumber )
      { m_maxDeltaChisqNdof = aNumber ; }

      // Maximum number of fit iterations.
      void setMaxIterations( int aMax )
      { m_maxIterations = aMax ; }

      // Don't add a hit if its chisq exceeds the max.
      void setMaxHitChisq( double aMax )
      { m_addHitOperation->setMaxHitChisq( aMax ) ; }

      // Use these functions to constrain a parameter to its initial value.
      void fixParameter( int aParam ) { m_fixParameter[ aParam ] = true ; }
      void fixCurvature()             { fixParameter( HIHelix::kCurvature ) ; }
      void fixPhi0()                  { fixParameter( HIHelix::kPhi0 ) ; }
      void fixD0()                    { fixParameter( HIHelix::kD0 ) ; }
      void fixCotTheta()              { fixParameter( HIHelix::kCotTheta ) ; }
      void fixZ0()                    { fixParameter( HIHelix::kZ0 ) ; }

      // Use these functions to release a constrained parameter.
      void releaseParameter( int aParam ) { m_fixParameter[ aParam ] = false; }
      void releaseCurvature()     { releaseParameter( HIHelix::kCurvature ) ; }
      void releasePhi0()          { releaseParameter( HIHelix::kPhi0 ) ; }
      void releaseD0()            { releaseParameter( HIHelix::kD0 ) ; }
      void releaseCotTheta()      { releaseParameter( HIHelix::kCotTheta ) ; }
      void releaseZ0()            { releaseParameter( HIHelix::kZ0 ) ; }

      void setPrintDiagnostics( DABoolean flag )
      { m_printDiagnostics = flag ; }

      void setUseDRAxialHits( DABoolean flag ) { m_useDRAxialHits = flag ; }
      void setUseDRStereoUHits( DABoolean flag ) { m_useDRStereoUHits = flag ;}
      void setUseDRStereoVHits( DABoolean flag ) { m_useDRStereoVHits = flag ;}
      void setCorrectDRHits( DABoolean flag )    { m_correctDRHits = flag ; }

      void setUseZDStereoUHits( DABoolean flag ) { m_useZDStereoUHits = flag ;}
      void setUseZDStereoVHits( DABoolean flag ) { m_useZDStereoVHits = flag ;}
      void setCorrectZDHits( DABoolean flag )    { m_correctZDHits = flag ; }

      void setUseDRCathodeHits( DABoolean flag ) { m_useDRCathodeHits = flag ;}
      void setCorrectDRCathodeHits( DABoolean flag )
      { m_correctDRCathodeHits = flag ; }

      void setUseSVRPhiHits( DABoolean flag ) { m_useSVRPhiHits = flag ; }
      void setUseSVZHits( DABoolean flag ) { m_useSVZHits = flag ; }
      void setCorrectSVRHits( DABoolean flag )   { m_correctSVRHits = flag ; }
      void setCorrectSVZHits( DABoolean flag )   { m_correctSVZHits = flag ; }

      void setContinueIfHitsMissing( DABoolean aFlag )
      { m_continueIfHitsMissing = aFlag ; }
      void setContinueIfAnyException( DABoolean aFlag )
      { m_continueIfAnyException = aFlag ; }

      void setZDHitUsageTag( const string& aTag )
      { m_zdHitUsageTag = UsageTag( aTag.c_str() ) ; }
      void setZDHitProductionTag( const string& aTag )
      { m_zdHitProdTag = ProductionTag( aTag.c_str() ) ; }

      void setDRHitUsageTag( const string& aTag )
      { m_drHitUsageTag = UsageTag( aTag.c_str() ) ; }
      void setDRHitProductionTag( const string& aTag )
      { m_drHitProdTag = ProductionTag( aTag.c_str() ) ; }
      void setCathodeHitUsageTag( const string& aTag )
      { m_cathodeHitUsageTag = UsageTag( aTag.c_str() ) ; }
      void setCathodeHitProductionTag( const string& aTag )
      { m_cathodeHitProdTag = ProductionTag( aTag.c_str() ) ; }
      void setSVRHitUsageTag( const string& aTag )
      { m_svrHitUsageTag = UsageTag( aTag.c_str() ) ; }
      void setSVRHitProductionTag( const string& aTag )
      { m_svrHitProdTag = ProductionTag( aTag.c_str() ) ; }
      void setSVZHitUsageTag( const string& aTag )
      { m_svzHitUsageTag = UsageTag( aTag.c_str() ) ; }
      void setSVZHitProductionTag( const string& aTag )
      { m_svzHitProdTag = ProductionTag( aTag.c_str() ) ; }

      void setZDFittingWeight( double aWeight )
      { m_forceZDFittingWeight = true ; m_zdFittingWeight = aWeight ; }

      void setDRFittingWeight( double aWeight )
      { m_forceDRFittingWeight = true ; m_drFittingWeight = aWeight ; }
      void setSVRFittingWeight( double aWeight )
      { m_forceSVRFittingWeight = true ; m_svrFittingWeight = aWeight ; }
      void setSVZFittingWeight( double aWeight )
      { m_forceSVZFittingWeight = true ; m_svzFittingWeight = aWeight ; }
      void setCathodeFittingWeight( double aWeight )
      { m_forceCathodeFittingWeight = true ; m_cathodeFittingWeight=aWeight ; }
      void setCathodeFittingWeightScale( double aScale )
      {
	 m_scaleCathodeFittingWeight = true ;
	 m_cathodeFittingWeightScale = aScale ;
      }

      // ---------- const member functions ---------------------
      DABoolean continueIfHitsMissing() const {
	 return m_continueIfHitsMissing; }

      DABoolean continueIfAnyException() const {
	 return m_continueIfAnyException; }

      DABoolean useDRAxialHits() const   { return  m_useDRAxialHits; }
      DABoolean useDRStereoUHits() const { return m_useDRStereoUHits; }
      DABoolean useDRStereoVHits() const { return m_useDRStereoVHits; }
      DABoolean correctDRHits() const    { return m_correctDRHits ; }

      DABoolean useZDStereoUHits() const { return m_useZDStereoUHits; }
      DABoolean useZDStereoVHits() const { return m_useZDStereoVHits; }
      DABoolean correctZDHits() const    { return m_correctZDHits ; }

      DABoolean useDRCathodeHits() const { return m_useDRCathodeHits; }
      DABoolean correctDRCathodeHits() const
      { return m_correctDRCathodeHits ; }

      DABoolean useSVRPhiHits() const    { return m_useSVRPhiHits; }
      DABoolean useSVZHits() const       { return m_useSVZHits; }
      DABoolean correctSVRHits() const   { return m_correctSVRHits ; }
      DABoolean correctSVZHits() const   { return m_correctSVZHits ; }

      STL_VECTOR( ChisqHelix* )* fittedHelices() const
      { return m_fittedHelices ; }

      virtual TDKinematicFit* hypothesis( DBCandidate::Hypo hypo ) const
      { return 0 ; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ChisqTrackFitter( const ChisqTrackFitter& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ChisqTrackFitter& operator=( const ChisqTrackFitter& );
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      DABoolean m_printDiagnostics ;

      DABoolean m_useDRAxialHits ;
      DABoolean m_useDRStereoUHits ;
      DABoolean m_useDRStereoVHits ;
      DABoolean m_correctDRHits ;

      DABoolean m_useZDStereoUHits ;
      DABoolean m_useZDStereoVHits ;
      DABoolean m_correctZDHits ;

      DABoolean m_useDRCathodeHits ;
      DABoolean m_correctDRCathodeHits ;

      DABoolean m_useSVRPhiHits ;
      DABoolean m_useSVZHits ;
      DABoolean m_correctSVRHits ;
      DABoolean m_correctSVZHits ;

      STL_VECTOR( ChisqHelix* )* m_fittedHelices ;
      ChisqAddHitOperation* m_addHitOperation ;

      DABoolean m_includeInitErrorMatrix ;

      double m_maxDeltaChisqNdof ;
      int    m_maxIterations ;

      KTMoveControl::MoveDirection m_moveDirection ;

      DABoolean m_fixParameter[ HIHelix::kZ0 + HIHelix::kCurvature ] ;

      HIFitConstraint*            m_fitConstraint ;
      HITrackParameterConstraint* m_fixParamsConstraint ;
      HICompositeFitConstraint*   m_compositeConstraint ;

      DABoolean m_continueIfHitsMissing ;
      DABoolean m_continueIfAnyException ;

      UsageTag m_zdHitUsageTag ;
      ProductionTag m_zdHitProdTag ;

      UsageTag m_drHitUsageTag ;
      ProductionTag m_drHitProdTag ;
      UsageTag m_cathodeHitUsageTag ;
      ProductionTag m_cathodeHitProdTag ;
      UsageTag m_svrHitUsageTag ;
      ProductionTag m_svrHitProdTag ;
      UsageTag m_svzHitUsageTag ;
      ProductionTag m_svzHitProdTag ;

      DABoolean m_forceZDFittingWeight ;
      double m_zdFittingWeight ;

      DABoolean m_forceDRFittingWeight ;
      double m_drFittingWeight ;
      DABoolean m_forceSVRFittingWeight ;
      double m_svrFittingWeight ;
      DABoolean m_forceSVZFittingWeight ;
      double m_svzFittingWeight ;
      DABoolean m_forceCathodeFittingWeight ;
      double m_cathodeFittingWeight ;
      DABoolean m_scaleCathodeFittingWeight ;
      double m_cathodeFittingWeightScale ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ChisqFitter/Template/ChisqTrackFitter.cc"
//#endif

#endif /* CHISQFITTER_CHISQTRACKFITTER_H */
