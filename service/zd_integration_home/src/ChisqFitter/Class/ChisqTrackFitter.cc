// -*- C++ -*-
//
// Package:     <ChisqFitter>
// Module:      ChisqTrackFitter
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Wed Jun  9 02:11:20 EDT 1999
// $Id: ChisqTrackFitter.cc,v 1.22 2003/07/28 15:07:12 mccann Exp $
//
// Revision history
//
// $Log: ChisqTrackFitter.cc,v $
// Revision 1.22  2003/07/28 15:07:12  mccann
// fitter now checks DetectorConfiguration before asking for SI/ZD objects
//
// Revision 1.21  2003/07/25 23:24:45  mccann
// Added the ZD to track fitting
//
// Revision 1.20  2003/04/17 16:59:07  bkh
// Add ZD options
//
// Revision 1.19  2001/11/12 18:53:19  llh14
// Make HISiSurfaceFactory with infinite surfaces
//
// Revision 1.18  2000/11/18 00:20:12  wsun
// Allow cathode fitting weight to be scaled by user-defined parameter.
//
// Revision 1.17  2000/10/29 02:33:32  wsun
// Get only hits to be fit from HIDRSurfaceFactory.
//
// Revision 1.16  2000/09/05 20:36:57  wsun
// Mods for forcing fitting weight to user-specified value.
//
// Revision 1.15  2000/08/16 20:30:50  wsun
// * Bug fix: hits were not really being dropped before.
// * Update to new TRTrackFitQuality.
// * Added usage and production tags for hits.
//
// Revision 1.14  2000/06/10 06:26:56  wsun
// Added parameters to dis/enable corrections to DR, SVR, SVZ, DRCA hits.
//
// Revision 1.13  2000/06/10 00:31:44  wsun
// Apply hit corrections.
//
// Revision 1.12  2000/06/09 01:40:57  wsun
// Added FrameAccess includes.
//
// Revision 1.11  2000/02/22 17:23:26  wsun
// Don't discard hits on first iteration.
//
// Revision 1.10  2000/01/11 20:13:39  wsun
// setTrackParamsAtInitRefPt() now resets error matrix as well.
//
// Revision 1.9  1999/10/05 22:57:43  wsun
// Implemented parameters for deselecting hits from a subdetector.
//
// Revision 1.8  1999/09/11 19:31:07  wsun
// Added DR cathodes.
//
// Revision 1.7  1999/08/27 21:02:43  wsun
// Reset surfaces before each iteration.
//
// Revision 1.6  1999/08/03 22:42:11  wsun
// Mods for fitting curler back-halves.
//
// Revision 1.5  1999/07/07 16:44:15  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.4  1999/06/24 22:53:07  wsun
// * Added use of HIMagField and new SiHits.
// * Added ability to fix any combination of parameters in track fitting.
//
// Revision 1.3  1999/06/20 21:17:10  wsun
// Allow user to fix parameters.
//
// Revision 1.2  1999/06/18 19:16:30  wsun
// Allow user to fix parameters.
//
// Revision 1.1.1.1  1999/06/16 02:46:47  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"

#include "TrackRoot/TRSeedTrack.h"

#include "ChisqFitter/ChisqTrackFitter.h"

#include "HelixIntersection/HICathodeSurfaceFactory.h"
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIFitConstraint.h"
#include "HelixIntersection/HITrackParameterConstraint.h"
#include "HelixIntersection/HICompositeFitConstraint.h"

#include "CalibratedData/CalibratedCathodeHit.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

#include "DAException/DANoDataException.h"

#include "DetectorConfiguration/DetectorConfiguration.h"

// // TEMPORARY
// #include "HelixIntersection/HIMomentumConstraint.h"

#include "CLHEP/Matrix/Matrix.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "ChisqTrackFitter" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqTrackFitter.cc,v 1.22 2003/07/28 15:07:12 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ChisqTrackFitter::ChisqTrackFitter(
   DABoolean includeInitialErrorMatrix,
   double maxDeltaChisqNdof,
   int    maxIterations,
   double maxHitChisq,
   HIFitConstraint* aConstraint )
   : m_fittedHelices( new STL_VECTOR( ChisqHelix* ) ),
     m_printDiagnostics( false ),
     m_useDRAxialHits( true ),
     m_useDRStereoUHits( true ),
     m_useDRStereoVHits( true ),
     m_correctDRHits( true ),
     m_useZDStereoUHits( true ),
     m_useZDStereoVHits( true ),
     m_correctZDHits( true ),
     m_useDRCathodeHits( true ),
     m_correctDRCathodeHits( true ),
     m_useSVRPhiHits( true ),
     m_useSVZHits( true ),
     m_correctSVRHits( true ),
     m_correctSVZHits( true ),
     m_addHitOperation( new ChisqAddHitOperation( maxHitChisq ) ),
     m_includeInitErrorMatrix( includeInitialErrorMatrix ),
     m_maxDeltaChisqNdof( maxDeltaChisqNdof ),
     m_maxIterations( maxIterations ),
     m_moveDirection( KTMoveControl::kDirectionForward ),
     m_fitConstraint( aConstraint ),
     m_fixParamsConstraint( 0 ),
     m_compositeConstraint( 0 ),
     m_continueIfHitsMissing( false ),
     m_continueIfAnyException( false ),
     m_drHitUsageTag( UsageTag() ),
     m_drHitProdTag( ProductionTag() ),
     m_zdHitUsageTag( UsageTag() ),
     m_zdHitProdTag( ProductionTag() ),
     m_cathodeHitUsageTag( UsageTag() ),
     m_cathodeHitProdTag( ProductionTag() ),
     m_svrHitUsageTag( UsageTag() ),
     m_svrHitProdTag( ProductionTag() ),
     m_svzHitUsageTag( UsageTag() ),
     m_svzHitProdTag( ProductionTag() ),
     m_forceDRFittingWeight( false ),
     m_drFittingWeight( 0. ),
     m_forceZDFittingWeight( false ),
     m_zdFittingWeight( 0. ),
     m_forceSVRFittingWeight( false ),
     m_svrFittingWeight( 0. ),
     m_forceSVZFittingWeight( false ),
     m_svzFittingWeight( 0. ),
     m_forceCathodeFittingWeight( false ),
     m_cathodeFittingWeight( 0. ),
     m_scaleCathodeFittingWeight( false ),
     m_cathodeFittingWeightScale( 1. )
{
//    // DONT FORGET TO DELETE STUFF IN DESTRUCTOR
//    m_fitConstraint = new HIMomentumConstraint ;

   if( m_fitConstraint != 0 )
   {
      m_fixParamsConstraint = new HITrackParameterConstraint ;
      m_compositeConstraint = new HICompositeFitConstraint ;
      m_compositeConstraint->setDtorDeletesChildren( false ) ;
      m_compositeConstraint->addChild( m_fitConstraint ) ;
      m_compositeConstraint->addChild( m_fixParamsConstraint ) ;
   }

   for( int i = 0 ; i < HIHelix::kZ0 + HIHelix::kCurvature ; ++i )
   {
      m_fixParameter[ i ] = false ;
   }
}

// ChisqTrackFitter::ChisqTrackFitter( const ChisqTrackFitter& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ChisqTrackFitter::~ChisqTrackFitter()
{ 
   clearCache() ;
   delete m_fittedHelices ;
   delete m_addHitOperation ;
   delete m_fixParamsConstraint ;
   delete m_compositeConstraint ;

//    // TEMPORARY!!!!!
//    delete m_fitConstraint ;
//    m_fitConstraint = 0 ;
}

//
// assignment operators
//
// const ChisqTrackFitter& ChisqTrackFitter::operator=( const ChisqTrackFitter& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

DABoolean
ChisqTrackFitter::fitTrack( const TRSeedTrack* theSeed,
			    Frame& theFrame,
			    DBCandidate::Hypo aHypo )
{
   FitDirection aFitDirection = aHypo >= DBCandidate::kExitElectron ?
      kOutwardFit : kInwardFit ;

   ChisqHelix* fittedHelix =
      ( ChisqHelix* ) fittedTrack( theSeed,
				   theFrame,
				   DBCandidate::mass( aHypo ),
				   aFitDirection ) ;
   if( fittedHelix == 0 )
   {
      return false ;
   }
 
   m_fittedHelices->push_back( fittedHelix ) ;

   // NavTrack matches seeds and fits by identifier.
   fittedHelix->setIdentifier( theSeed->identifier() ) ;

   return true ;
}


KTHelix*
ChisqTrackFitter::fittedTrack( const TRSeedTrack* theSeed,
			       Frame& theFrame,
			       DBCandidate::Hypo aHypo )
{
   return fittedTrack( theSeed, theFrame, 0, 0, 0, 0, 0, aHypo ) ;
}

KTHelix*
ChisqTrackFitter::fittedTrack(
   const TRSeedTrack* theSeed,
   Frame& theFrame,
   const STL_VECTOR(
      HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector,
   const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector,
   const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector,
   const STL_VECTOR( const CalibratedSVZHit* )* aZVector,
   const STL_VECTOR(
      HIZDSurfaceFactory::ZDHitAndDriftDistance )* aZDVector,
   DBCandidate::Hypo aHypo,
   DABoolean aExtractHitsIfNull )
{
   FitDirection aFitDirection = aHypo >= DBCandidate::kExitElectron ?
      kOutwardFit : kInwardFit ;

   return fittedTrack( theSeed,
		       theFrame,
		       aDRVector,
		       aCathodeVector,
		       aRphiVector,
		       aZVector,
		       aZDVector,
		       DBCandidate::mass( aHypo ),
		       aFitDirection,
		       aExtractHitsIfNull ) ;
}

KTHelix*
ChisqTrackFitter::fittedTrack( const TRSeedTrack* theSeed,
			       Frame& theFrame,
			       GeV aMass,
			       FitDirection aFitDirection )
{
   return fittedTrack( theSeed, theFrame, 0, 0, 0, 0, 0, aMass, aFitDirection ) ;
}


KTHelix*
ChisqTrackFitter::fittedTrack(
   const TRSeedTrack* theSeed,
   Frame& theFrame,
   const STL_VECTOR(
      HIDRSurfaceFactory::DRHitAndDriftDistance )* aDRVector,
   const STL_VECTOR( const CalibratedCathodeHit* )* aCathodeVector,
   const STL_VECTOR( const CalibratedSVRphiHit* )* aRphiVector,
   const STL_VECTOR( const CalibratedSVZHit* )* aZVector,
   const STL_VECTOR(
      HIZDSurfaceFactory::ZDHitAndDriftDistance )* aZDVector,
   GeV aMass,
   FitDirection aFitDirection,
   DABoolean aExtractHitsIfNull )
{
   // Initialize helix to be fitted.  Third argument tells the
   // HIIntersectionSurfaces to calculate the derivatives w.r.t. the track
   // parameters at the initial reference point, not at the current ref pt.
   ChisqHelix* fittedHelix = new ChisqHelix( *( ( KTHelix* ) theSeed ),
					     aMass,
					     true ) ;
   ChisqHelix copyOfOriginal( *fittedHelix ) ;


   m_addHitOperation->setPrintDiagnostics( m_printDiagnostics ) ;

   // Vector of HIIntersectionSurfaces to be passed to HIHelixIntersector.
   STL_VECTOR( HIIntersectionSurface* )* pSurfaces =
      new STL_VECTOR( HIIntersectionSurface* ) ;

   // Generate surfaces for DR layers with hits.
   if( aDRVector != 0 || aExtractHitsIfNull )
   {
      HIDRSurfaceFactory drFactory( theFrame,
				    m_useDRAxialHits,
				    m_useDRStereoUHits,
				    m_useDRStereoVHits,
				    m_correctDRHits ) ;

      if( drFactory.geometryAvailable() )
      {
	 STL_VECTOR( HIDRSurfaceFactory::DRHitAndDriftDistance ) drHitInfos ;

	 if( aDRVector == 0 )
	 {
	    // Catch exceptions from extracting DR hits on track.
	    try
	    {
	       drFactory.getDRHits( theSeed,
				    drHitInfos,
				    m_drHitUsageTag,
				    m_drHitProdTag,
				    true // fittable hits only
		  ) ;
	    }
	    catch( NO_TABLE_EXCEPTION( CalibratedDRHit )& aException )
	    {
	       // Exception is caught if specified by user.  Otherwise,
	       // it is rethrown.
	       if( m_continueIfHitsMissing )
	       {
		  report( WARNING, kFacilityString )
		     << "Continuing without DR hits." << endl;
	       }
	       else
	       {
		  // This rethrows the exception currently being handled.
		  throw ;
	       }
	    }
	    catch( DAExceptionBase& aException )
	    {
	       // Exception is caught if specified by user.  Otherwise,
	       // it is rethrown.
	       if( m_continueIfAnyException )
	       {
		  report( WARNING, kFacilityString )
		     << "Continuing despite exception: \n "
		     << aException.what() << endl ;
	       }
	       else
	       {
		  throw ;
	       }
	    }
	 }
	 else
	 {
	    drHitInfos = *aDRVector ;
	 }

	 if( drHitInfos.size() > 0 )
	 {
	    drFactory.generateSingleWireSurfaces(
	       *pSurfaces,
	       drHitInfos,
	       m_addHitOperation,
	       false,  // check cross origin
	       m_forceDRFittingWeight,
	       m_drFittingWeight ) ;
	 }
      }
   }

   const Record& baseGeometryRecord = theFrame.record(Stream::kBaseGeometry);
   FAItem< DetectorConfiguration > detectorConfig;
   extract(baseGeometryRecord, detectorConfig);

   // Generate surfaces for ZD layers with hits.
   if ( detectorConfig->useZDnotSI() )
   {
      if( aZDVector != 0 || aExtractHitsIfNull )
      {
	 HIZDSurfaceFactory zdFactory( theFrame,
				       m_useZDStereoUHits,
				       m_useZDStereoVHits,
				       m_correctZDHits ) ;

	 if( zdFactory.geometryAvailable() )
	 {
	    STL_VECTOR( HIZDSurfaceFactory::ZDHitAndDriftDistance ) zdHitInfos ;

	    if( aZDVector == 0 )
	    {
	       // Catch exceptions from extracting ZD hits on track.
	       try
	       {
		  zdFactory.getZDHits( theSeed,
				       zdHitInfos,
				       m_zdHitUsageTag,
				       m_zdHitProdTag,
				       true // fittable hits only
		     ) ;
	       }
	       catch( NO_TABLE_EXCEPTION( CalibratedZDHit )& aException )
	       {
		  // Exception is caught if specified by user.  Otherwise,
		  // it is rethrown.
		  if( m_continueIfHitsMissing )
		  {
		     report( WARNING, kFacilityString )
			<< "Continuing without ZD hits." << endl;
		  }
		  else
		  {
		     // This rethrows the exception currently being handled.
		     throw ;
		  }
	       }
	       catch( DAExceptionBase& aException )
	       {
		  // Exception is caught if specified by user.  Otherwise,
		  // it is rethrown.
		  if( m_continueIfAnyException )
		  {
		     report( WARNING, kFacilityString )
			<< "Continuing despite exception: \n "
			<< aException.what() << endl ;
		  }
		  else
		  {
		     throw ;
		  }
	       }
	    }
	    else
	    {
	       zdHitInfos = *aZDVector ;
	    }

	    report(DEBUG, kFacilityString)
	       << "about to append " << zdHitInfos.size() << " ZD hits to the list of surfaces" << endl;

	    if( zdHitInfos.size() > 0 )
	    {
	       zdFactory.generateSingleWireSurfaces(
		  *pSurfaces,
		  zdHitInfos,
		  m_addHitOperation,
		  false,  // check cross origin
		  m_forceZDFittingWeight,
		  m_zdFittingWeight ) ;

	       report(DEBUG, kFacilityString) << "did it!" << endl;
	    }
	 }
      }
   }
   else
   { // NOT useZDnotSI
      // Generate surfaces for Si ladders with hits.
      if( aRphiVector != 0 || aZVector != 0 || aExtractHitsIfNull )
      {
	 HISiSurfaceFactory siFactory( theFrame, 0, 0, false ) ;

	 if( siFactory.geometryAvailable() )
	 {
	    STL_VECTOR( const CalibratedSVRphiHit* ) svrHits ;
	    STL_VECTOR( const CalibratedSVZHit* ) svzHits ;

	    if( aRphiVector == 0 )
	    {
	       if( aExtractHitsIfNull && m_useSVRPhiHits )
	       {
		  // Catch exceptions from extracting SVR hits on track.
		  try
		  {
		     siFactory.getSVRHits( theSeed,
					   svrHits,
					   m_svrHitUsageTag,
					   m_svrHitProdTag ) ;
		  }
		  catch( NO_TABLE_EXCEPTION( CalibratedSVRphiHit )& aException )
		  {
		     // Exception is caught if specified by user.  Otherwise,
		     // it is rethrown.
		     if( m_continueIfHitsMissing )
		     {
			report( WARNING, kFacilityString )
			   << "Continuing without SVRphi hits." << endl;
		     }
		     else
		     {
			// This rethrows the exception currently being handled.
			throw ;
		     }
		  }
		  catch( DAExceptionBase& aException )
		  {
		     // Exception is caught if specified by user.  Otherwise,
		     // it is rethrown.
		     if( m_continueIfAnyException )
		     {
			report( WARNING, kFacilityString )
			   << "Continuing despite exception: \n "
			   << aException.what() << endl ;
		     }
		     else
		     {
			throw ;
		     }
		  }
	       }
	    }
	    else
	    {
	       svrHits = *aRphiVector ;
	    }

	    if( aZVector == 0 )
	    {
	       if( aExtractHitsIfNull && m_useSVZHits )
	       {
		  // Catch exceptions from extracting SVZ hits on track.
		  try
		  {
		     siFactory.getSVZHits( theSeed,
					   svzHits,
					   m_svzHitUsageTag,
					   m_svzHitProdTag ) ;
		  }
		  catch( NO_TABLE_EXCEPTION( CalibratedSVZHit )& aException )
		  {
		     // Exception is caught if specified by user.  Otherwise,
		     // it is rethrown.
		     if( m_continueIfHitsMissing )
		     {
			report( WARNING, kFacilityString )
			   << "Continuing without SVZ hits." << endl;
		     }
		     else
		     {
			// This rethrows the exception currently being handled.
			throw ;
		     }
		  }
		  catch( DAExceptionBase& aException )
		  {
		     // Exception is caught if specified by user.  Otherwise,
		     // it is rethrown.
		     if( m_continueIfAnyException )
		     {
			report( WARNING, kFacilityString )
			   << "Continuing despite exception: \n "
			   << aException.what() << endl ;
		     }
		     else
		     {
			throw ;
		     }
		  }
	       }
	    }
	    else
	    {
	       svzHits = *aZVector ;
	    }

	    if( svrHits.size() > 0 || svzHits.size() > 0 )
	    {
	       siFactory.generateCenterSurfacesWithHits(
		  svrHits,
		  svzHits,
		  m_addHitOperation,
		  m_forceSVRFittingWeight,
		  m_svrFittingWeight,
		  m_forceSVZFittingWeight,
		  m_svzFittingWeight ) ;
	    }

	    siFactory.transferSurfacesTo( *pSurfaces ) ;
	 }
      }
   } // end if NOT useZDnotSI

   // Generate surface for cathode layer.
   if( ( aCathodeVector != 0 || aExtractHitsIfNull ) && m_useDRCathodeHits )
   {
      HICathodeSurfaceFactory cathodeFactory( theFrame ) ;

      if( cathodeFactory.geometryAvailable() )
      {
	 STL_VECTOR( const CalibratedCathodeHit* ) cathodeHits ;

	 if( aCathodeVector == 0 )
	 {
	    // Catch exceptions from extracting cathode hits on track.
	    try
	    {
               cathodeFactory.getCathodeHits( theSeed,
                                              cathodeHits,
                                              m_cathodeHitUsageTag,
                                              m_cathodeHitProdTag ) ;
	    }
	    catch( NO_TABLE_EXCEPTION( CalibratedCathodeHit )& aException )
	    {
	       // Exception is caught if specified by user.  Otherwise,
	       // it is rethrown.
	       if( m_continueIfHitsMissing )
	       {
		  report( WARNING, kFacilityString )
		     << "Continuing without cathode hits." << endl;
	       }
	       else
	       {
		  // This rethrows the exception currently being handled.
		  throw ;
	       }
	    }
	    catch( DAExceptionBase& aException )
	    {
	       // Exception is caught if specified by user.  Otherwise,
	       // it is rethrown.
	       if( m_continueIfAnyException )
	       {
		  report( WARNING, kFacilityString )
		     << "Continuing despite exception: \n "
		     << aException.what() << endl ;
	       }
	       else
	       {
		  throw ;
	       }
	    }
	 }
	 else
	 {
	    cathodeHits = *aCathodeVector ;
	 }

	 if( cathodeHits.size() > 0 )
	 {
	    cathodeFactory.generateCylinderWithHits(
	       *pSurfaces,
	       cathodeHits,
	       m_addHitOperation,
	       m_forceCathodeFittingWeight,
	       m_cathodeFittingWeight,
	       m_scaleCathodeFittingWeight,
	       m_cathodeFittingWeightScale ) ;
	 }
      }
   }

   // Finish setup.
   if( pSurfaces->size() == 0 )
   {
      delete pSurfaces ;
      fittedHelix->setFitFailed( true ) ;
      return fittedHelix ;
   }
   else
   {
      if( m_printDiagnostics )
      {
	 STL_VECTOR( HIIntersectionSurface* )::const_iterator surfaceItr =
	    pSurfaces->begin() ;
	 STL_VECTOR( HIIntersectionSurface* )::const_iterator lastSurface =
	    pSurfaces->end() ;

	 for( ; surfaceItr != lastSurface ; ++surfaceItr )
	 {
	    ( *surfaceItr )->setPrintDiagnostics( true ) ;
	 }
      }
   }


   //~~~~~~~~~~~~~~~~~~~~~ Apply corrections to hits ~~~~~~~~~~~~~~~~~~~~~

   // Form vector of HICalibratedHits.
   STL_VECTOR( HICalibratedHit* ) hiHitsOnSeed ;

   STL_VECTOR( HIIntersectionSurface* )::const_iterator itr =
      pSurfaces->begin() ;
   STL_VECTOR( HIIntersectionSurface* )::const_iterator end =
      pSurfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      ( *itr )->addHICalibratedHitsToVector( hiHitsOnSeed ) ;
   }

   // Clear the HICalibratedHits and apply track- and mass-dependent
   // corrections to the hits on surfaces.
   STL_VECTOR( HICalibratedHit* )::iterator hiitr = hiHitsOnSeed.begin() ;
   STL_VECTOR( HICalibratedHit* )::iterator hiend = hiHitsOnSeed.end() ;

   for( ; hiitr != hiend ; ++hiitr )
   {
      ( *hiitr )->setInfoOnTrack( false ) ;
      ( *hiitr )->setUseInFit( true ) ;
      ( *hiitr )->clear() ;
      ( *hiitr )->applyTrackCorrections( *fittedHelix ) ;
   }

   //~~~~~~~~~~~~~~~~~~~~~~~~ Start fitting ~~~~~~~~~~~~~~~~~~~~~~~~

   HepVector deltaParams ;
   HepSymMatrix errorMatrix ;

   double chisq = 1.e10 ;
   int ndof = 1 ;
   double oldChisq = 1.e10 ;
   int oldNdof = 1 ;

   DABoolean useCompositeConstraint = false ;

//    // TEMPORARY!!!
//    m_fitConstraint->setConstraintParametersOne( *fittedHelix ) ;
//    m_fixParameter[ HIHelix::kD0 ] = true ;
//    m_fixParameter[ HIHelix::kZ0 ] = true ;

   // If no constraints other than fixing parameters are to be applied,
   // then simply remove the free parameters from the helix.  Otherwise,
   // fixing the parameters is applied after the unconstrained fit as
   // another constraint.  This is to avoid inverting a singular matrix
   // in solving the constraint.
   if( m_fitConstraint == 0 )
   {
      for( int i = HIHelix::kCurvature ; i <= HIHelix::kZ0 ; ++i )
      {
	 if( m_fixParameter[ i ] )
	 {
	    fittedHelix->fixParameter( i ) ;
	 }
      }
   }
   else
   {
      m_fixParamsConstraint->clearFixedParameters() ;

      for( int i = HIHelix::kCurvature ; i <= HIHelix::kZ0 ; ++i )
      {
	 if( m_fixParameter[ i ] )
	 {
	    m_fixParamsConstraint->setParameterToFix( i ) ;
	    useCompositeConstraint = true ;
	 }
      }

      if( useCompositeConstraint )
      {
	 m_compositeConstraint->recalculateNumberOfConstraints() ;
	 m_fixParamsConstraint->setConstraintParametersOne( *fittedHelix ) ;
      }
   }


   // Save the initial error and information matrices.
   HepSymMatrix initErrorMatrix ;
   HepSymMatrix initInfoMatrix ;

   int ierr ;
   if( m_includeInitErrorMatrix )
   {
      initErrorMatrix = fittedHelix->errorMatrix() ;
      initInfoMatrix  = initErrorMatrix.inverse( ierr ) ;

      if( ierr )
      {
	 report(DEBUG, kFacilityString) << "ierr: canelling fit at init" << endl;

	 // Delete the z-conditions, the surfaces, and the vector itself.
	 STL_VECTOR( HIIntersectionSurface* )::iterator itr =
	    pSurfaces->begin() ;
	 STL_VECTOR( HIIntersectionSurface* )::iterator last =
	    pSurfaces->end() ;

	 for( ; itr != last ; ++itr )
	 {
	    ( *itr )->deleteCondition() ;
	    delete *itr ;
	 }
	 delete pSurfaces ;

	 fittedHelix->setFitFailed( true ) ;
	 return fittedHelix ;
      }
   }
   else
   {
      fittedHelix->clearErrorMatrix() ;
   }


   // Iterate fit until chisq changes by less than x or
   // more than 10 iterations have been performed.
   int nIter = 0 ;

   // Disable hit rejecton until after the first iteration.
   m_addHitOperation->setRejectHits( false ) ;

   // Instantiate the HIHelixIntersector.
   HIHelixIntersector intersector( *pSurfaces,
				   HIHelixIntersector::kOrderAsGiven,
				   fittedHelix,
				   0, 0,     // global cond. and oper.
				   false,    // check cross origin
				   false ) ; // attempt curling

   // The track parameters at the "origin" -- these change from iteration
   // to iteration.
   HepVector paramsAtOrigin = fittedHelix->freeTrackParameters() ;

   // The difference between the previous fit parameters ( = params at start )
   // and the new params after each iteration -- used to calculate chisq.
   HepVector diffPrevFit( paramsAtOrigin.num_row(), 0 ) ;

   HepSymMatrix savedErrorMatrix ;
   STL_VECTOR( ChisqHelix::HitAndLinkData ) savedHitsOnTrack ;

   DABoolean fitConverged = false ;
   DABoolean tooManyIterations = false ;
   DABoolean noDof = false ;

   if( m_printDiagnostics )
   {
      // This is false by default.
      intersector.setPrintDiagnostics( true ) ;

      report( DEBUG, kFacilityString )
	 << "Starting fit"
	 << endl << "x " << fittedHelix->position()
	 << endl << "p "
	 << fittedHelix->momentum()
	 << " mag "
	 << fittedHelix->momentum().mag()
	 << endl << "ref pt " << fittedHelix->referencePoint()
	 << " mass " << fittedHelix->mass()
	 << endl;
   }

   do
   {
      // Go back to the first surface.
      intersector.resetSurfacePointer() ;

      // Reset the surfaces.
      intersector.resetSurfaces() ;

      // Clear the HICalibratedHits and allow them all to be reused in
      // the fit -- hits to be discarded are reevaluated for each iteration
      // (except the first).
      STL_VECTOR( HICalibratedHit* )::iterator hiitr = hiHitsOnSeed.begin() ;
      STL_VECTOR( HICalibratedHit* )::iterator hiend = hiHitsOnSeed.end() ;

      for( ; hiitr != hiend ; ++hiitr )
      {
	 // infoOnTrack is already set to false by ChisqHelix::reset() in
	 // previous iteration.
	 //
	 // If this statement is removed in the future, then a cumulative
	 // count of the number of hits dropped needs to be kept.  Currently,
	 // the number of hits dropped is reset before each iteration.

	 ( *hiitr )->setUseInFit( true ) ;
      }

      // Start swimming.
      HIHelixIntersector::IntersectionStatus status =
	 intersector.swimToCurrentSurface( m_moveDirection ) ;

      do
      {
	 status = intersector.swimToNextIntersection( m_moveDirection ) ;
      }
      while( status == HIHelixIntersector::kIntersectionOK ) ;

      // The helix has accumulated the information matrix and residual vector.
      // Add on the original information matrix (if desired) and invert to
      // solve for the parameter corrections.

      HepSymMatrix infoMatrix = fittedHelix->informationMatrix() ;

      if( m_includeInitErrorMatrix )
      {
	 infoMatrix += initInfoMatrix ;
      }

      // Invert the matrix.
      errorMatrix = infoMatrix.inverse( ierr ) ;

      if( ierr )
      {
	 report(DEBUG, kFacilityString) << "ierr: can't invert " << infoMatrix << endl;

	 // Delete the z-conditions, the surfaces, and the vector itself.
	 STL_VECTOR( HIIntersectionSurface* )::iterator itr =
	    pSurfaces->begin() ;
	 STL_VECTOR( HIIntersectionSurface* )::iterator last =
	    pSurfaces->end() ;

	 for( ; itr != last ; ++itr )
	 {
	    ( *itr )->deleteCondition() ;
	    delete *itr ;
	 }
	 delete pSurfaces ;

	 *fittedHelix = copyOfOriginal ;
	 fittedHelix->setFitFailed( true ) ;
	 return fittedHelix ;
      }

      // Calculate change to track parameters.
      deltaParams = errorMatrix * fittedHelix->residualVector() ;

      // Save the previous chisq and ndof.
      oldChisq = chisq ;
      oldNdof = ndof ;

      // Calculate new chisq.
      chisq = fittedHelix->chisq() ;
      ndof  = fittedHelix->ndof() ;

      diffPrevFit +=  deltaParams ;
      if( m_includeInitErrorMatrix )
      {
	 chisq += initErrorMatrix.similarity( diffPrevFit ) ;
      }

      if( m_printDiagnostics )
      {
	 cout
	    << "\n iteration " << nIter+1
	    << "\n params  " ;
	    
	 for( int i = 1 ; i <= paramsAtOrigin.num_row() ; ++i )
	 {
	    cout << paramsAtOrigin( i ) << " " ;
	 }
	 cout << "\n dParams " ;
	 for( int j = 1 ; j <= deltaParams.num_row() ; ++j )
	 {
	    cout << deltaParams( j ) << " " ;
	 }
	 cout << "\n errors " ;
	 for( int k = 1 ; k <= errorMatrix.num_row() ; ++k )
	 {
	    cout << ( errorMatrix( k, k ) > 0. ?
		      sqrt( errorMatrix( k, k ) ) : -999 ) << " " ;
	 }
	 cout << endl ;
      }

      if( ndof > 0 )
      {
	 double deltaChisqNdof = fabs( oldChisq/oldNdof - chisq/ndof ) ;
	 fitConverged = deltaChisqNdof < m_maxDeltaChisqNdof ;
      }
      else
      {
	 noDof = true ;
      }

      if( m_printDiagnostics )
      {
	 cout << "niter " << nIter+1
	      << " hits dropped " << fittedHelix->numberHitsDropped()
	      << " flipped " << fittedHelix->numberHitsFlipped() << endl ;
      }

      tooManyIterations = ++nIter > m_maxIterations ;
      fittedHelix->setNumberIterations( nIter ) ;

      if( !fitConverged && !tooManyIterations && !noDof )
      {
	 // Add the previous track parameter corrections.
	 paramsAtOrigin += deltaParams ;

	 savedHitsOnTrack = fittedHelix->hitsOnTrack() ;
	 savedErrorMatrix = errorMatrix ;

// 	 fittedHelix->resetHitsOnTrack() ;
// 	 fittedHelix->setChisq( 0. ) ;
	 fittedHelix->reset() ;
	 fittedHelix->setTotalArcLength( 0. ) ;
	 fittedHelix->setLastArcLength( 0. ) ;
	 fittedHelix->clearInfoMatrix() ;
	 fittedHelix->clearResidualVector() ;

	 // Reset reference point to original, constrained parameters to their
	 // fixed values, unconstrained parameters to their given values,
	 // and error matrix to initial matrix if it is included in the fit.
	 fittedHelix->setTrackParamsAtInitRefPt( paramsAtOrigin ) ;
      }
      else if( nIter > 1 )
      {
	 fittedHelix->returnToStart() ;
	 fittedHelix->setErrorMatrix( savedErrorMatrix ) ;
	 fittedHelix->setChisq( chisq ) ;
	 fittedHelix->setHitsOnTrack( savedHitsOnTrack ) ;
      }

      if( m_printDiagnostics )
      {
	 cout
	    << "x " << fittedHelix->position()
	    << "\n p "
	    << fittedHelix->momentum()
	    << " mag "
	    << fittedHelix->momentum().mag()
	    << "\n ref pt " << fittedHelix->referencePoint()
	    << " mass " << fittedHelix->mass()
	    << " chisq/ndof " << chisq << "/" << ndof << "\n"
	    << endl ;
      }

      // Enable hit rejection. (Hits not deleted on first iteration.)
      m_addHitOperation->setRejectHits( true ) ;
   }
   while( !fitConverged && !tooManyIterations && !noDof ) ;

   if( m_printDiagnostics )
   {
      if( fitConverged ) cout << "Fit converged." << endl ;
      else if( tooManyIterations ) cout << "Too many iterations." << endl ;
      else if( noDof ) cout << "No degrees of freedom." << endl ;

      cout << "Number of hits dropped " << fittedHelix->numberHitsDropped()
	   << " flipped " << fittedHelix->numberHitsFlipped()
	   << " iterations " << fittedHelix->numberIterations() << endl ;
   }


   // Set the fit's link data residual and error to the smoothed quantities.
   // Also copy link data from HICalibratedHit to link data.
   STL_VECTOR( HIFitHelix::HitAndLinkData )::iterator hitItr =
      fittedHelix->hitsOnTrack().begin() ;
   STL_VECTOR( HIFitHelix::HitAndLinkData )::iterator hitEnd =
      fittedHelix->hitsOnTrack().end() ;

   for( ; hitItr != hitEnd ; ++hitItr )
   {
      hitItr->calculateSmoothedResidualAndError() ;
      hitItr->takeOwnershipOfLinkData() ;
   }


   // Delete the z-conditions, the surfaces, and the vector itself.
   itr = pSurfaces->begin() ;
   end = pSurfaces->end() ;

   for( ; itr != end ; ++itr )
   {
      ( *itr )->deleteCondition() ;
      delete *itr ;
   }
   delete pSurfaces ;

   if( tooManyIterations || noDof )
   {
      fittedHelix->setFitFailed( true ) ;
      return fittedHelix ;
   }

   //~~~~~~~~~~~~~~~~~~~~~~~~ Apply fit constraints ~~~~~~~~~~~~~~~~~~~~~~~~~

   if( m_fitConstraint != 0 )
   {
      if( useCompositeConstraint )
      {
	 m_compositeConstraint->applyConstraint( fittedHelix ) ;
      }
      else
      {
	 m_fitConstraint->applyConstraint( fittedHelix ) ;
      }
   }

   return fittedHelix ;
}


void
ChisqTrackFitter::clearCache()
{
   // Delete the helices in the vectors.
   STL_VECTOR( ChisqHelix* )::iterator helixItr =
      m_fittedHelices->begin() ;
   STL_VECTOR( ChisqHelix* )::iterator helixEnd =
      m_fittedHelices->end() ;

   for( ; helixItr != helixEnd ; ++helixItr )
   {
      delete *helixItr ;
   }
   m_fittedHelices->clear() ;

   TrackFitter::clearCache() ;
}

//
// const member functions
//

//
// static member functions
//
