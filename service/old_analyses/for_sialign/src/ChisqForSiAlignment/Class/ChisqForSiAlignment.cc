// -*- C++ -*-
//
// Package:     <ChisqForSiAlignment>
// Module:      ChisqForSiAlignment
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Sat Jun  3 09:58:31 EDT 2000
// $Id: ChisqForSiAlignment.cc,v 1.6 2001/06/25 22:20:20 llh14 Exp $
//
// Revision history
//
// $Log: ChisqForSiAlignment.cc,v $
// Revision 1.6  2001/06/25 22:20:20  llh14
// Changed type returned from Lattice code from pointer to ref
//
// Revision 1.5  2001/03/29 17:00:31  llh14
// Saving Curvature instead of Hybrid Number
//
// Revision 1.4  2001/02/21 16:51:40  llh14
// Added dof as Parameters
//
// Revision 1.3  2001/02/06 20:31:35  llh14
// Added dof, and MaxEvent parameters
//
// Revision 1.2  2001/02/02 20:43:41  llh14
// Fixed Chisq Norm Bug
//
// Revision 1.1.1.1  2001/02/02 18:03:29  llh14
// imported ChisqForSiAlignment
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ChisqForSiAlignment/ChisqForSiAlignment.h"

#include "DualTrackHelices/DualTrackHelices.h"
#include "STLUtility/fwd_vector.h"
#include "Navigation/NavTrack.h"

#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

#include "HelixIntersection/HIHelixIntersector.h"
#include "HelixIntersection/HIIntersectionSurface.h"
#include "HelixIntersection/HISiSurfaceFactory.h"
#include "HelixIntersection/HICompositeSurface.h"
#include "HelixIntersection/HIMagField.h"

#include "DataHandler/Stream.h"
#include "DataHandler/Frame.h"

#include "ASiStorePro/ASiStore.h"

#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAConstants.h"

#include "KinematicTrajectory/KTHelix.h"
#include "KinematicTrajectory/KTKinematicData.h"

#include "MinuitInterface/MinuitInterface.h"

//Debugging libraries
#include "TrackFitter/TrackFitSVRHitLink.h"

#include "TrackRoot/TRSeedTrack.h"
#include "TrackFinder/SeedTrackSVRHitLattice.h"
#include "TrackFitter/TrackFitSVRHitLink.h"

#include "Doit/DoitSeedQuality.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ChisqForSiAlignment.ChisqForSiAlignment" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ChisqForSiAlignment.cc,v 1.6 2001/06/25 22:20:20 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ChisqForSiAlignment::ChisqForSiAlignment() : 
   m_checkEvent(0), 
   MIFcn(), 
   m_iteration(1)

{
      m_removeHitsForRphi.push_back(m_badEventsForRphi);
      m_removeHitsForRphi.push_back(m_badTracksForRphi);
      m_removeHitsForRphi.push_back(m_badHitsForRphi);

      m_removeHitsForZ.push_back(m_badEventsForZ);
      m_removeHitsForZ.push_back(m_badTracksForZ);
      m_removeHitsForZ.push_back(m_badHitsForZ);

}

// ChisqForSiAlignment::ChisqForSiAlignment( const ChisqForSiAlignment& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ChisqForSiAlignment::~ChisqForSiAlignment()
{
   report( DEBUG, kFacilityString)
      << "In destructor"
      << endl;

}

//
// assignment operators
//
// const ChisqForSiAlignment& ChisqForSiAlignment::operator=( const ChisqForSiAlignment& rhs )
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

void
ChisqForSiAlignment::calcResiduals(const FIFrameIterator& iBegin, 
				   const FIFrameIterator& iEnd, 
				   Count& eventCtr, 
				   double& chisq, 
				   Count maxEvents, 
				   float normRphi,
				   float normZ,
				   float cutRphi,
				   float cutZ,
				   DABoolean histogramsOn, 
				   HINtuple* rPhi_TrackHits,
				   HINtuple* z_TrackHits
				   )

{     
   m_normRphi = normRphi;
   m_normZ = normZ;
   m_cutRphi = cutRphi;
   m_cutZ = cutZ;

   FIFrameIterator iFrame=iBegin;
   m_maxEvents = maxEvents;

   if (iBegin == iEnd)
   { cout << "Start equals End!!!" << endl; }

   Count trackCtr;

   //For DEBUGGING!!//
   Count residualRphiCtr=0;
   Count residualZCtr=0;

   Count missedHits = 0;
   Count totalHits = 0;

   DABoolean debugOn = false;

// extract silicon geometry using the first frame
   FAItem<ASiStore> iStore;
   extract(iFrame->record(Stream::kSVAlignment), iStore);

// Loop over the events
   for( ; iFrame != iEnd; ++iFrame, ++eventCtr)     
   {
      //making sure we have stopped in event
      if ( (*iFrame).stopStream() != Stream::kEvent )
      {  
	 report(ALERT, kFacilityString)
	    << "Not stopped in proper place!!!" << endl;
         continue;
      }

      //No more than maxEvents
      if ( eventCtr >= maxEvents ) { break; }
     
//    extract tracks	 
      FATable< NavTrack > tracks;
      extract( iFrame->record( Stream::kEvent ), tracks);
     
//    check to make sure we are using the proper tracks
      if (tracks.size() != 2)
      { report( ALERT, kFacilityString)
	 << "There are the wrong number of tracks!! Please check input file"
	 << endl;     }

//    extract the two objects we inserted into the frame
      FATable< DualTrackHelices > dualSetOfHelices;
      extract(iFrame->record(Stream::kEvent ), dualSetOfHelices);
      
      FATable< DualTrackHelices >::const_iterator dualHelix;
      FATable< NavTrack >::const_iterator trackIter;
         
//    Loop over the tracks and the Helices 
      for (trackIter = tracks.begin(), dualHelix = dualSetOfHelices.begin(), 
	   trackCtr = 1; trackIter != tracks.end();
	   ++trackIter, ++dualHelix, trackCtr++) {

///////////////////////////////////////////////////////////////////////////////
// How do I know if these hits were used in the fit to make the HIHelix? //////
      STL_VECTOR( const CalibratedSVRphiHit* ) siRphiHitsOnTrack =
	 *(trackIter->seedSVRHits());  //This comes from Doit
//	 *(trackIter->muonSVRHits());  //This comes from Kalman

      STL_VECTOR( const CalibratedSVZHit* ) siZHitsOnTrack =
	 *(trackIter->seedSVZHits());  //This comes from Doit
//         *(trackIter->muonSVZHits());  //This comes from Kalman

//////////////////////////////////////////////////////////////////////////////
//We need to find out if the hits are on track or not////////////////////////
//      if ( debugOn && eventCtr == m_checkEvent && false) 
//      {	    
//	 FATable< CalibratedSVRphiHit > siRphiHits;
//	 extract( iFrame->record(Stream::kEvent), siRphiHits);
	 
//	 FAItem< MuonFitSVRHitLattice > shrLattice;
//	 extract( iFrame->record(Stream::kEvent), shrLattice );
	 
//	 FATable< CalibratedSVRphiHit>::const_iterator testHitItr= 
//	    siRphiHits.begin();
	 
//	 for( ; testHitItr!=siRphiHits.end(); testHitItr++)
//	 {
	    
//	    const MuonFitSVRHitLattice::Links& links =
//	       shrLattice->linksGivenRight( testHitItr->identifier() );
	    
// Is there more than one link per hit?
//	    if ( &links != 0)
//	    {
	       
//	       report(ALERT, kFacilityString) 
//		  << "\nThe hit is: " << *testHitItr << endl;
	    
//	       for(MuonFitSVRHitLattice::Links::const_iterator linkItr 
//		   = links.begin(); linkItr != links.end(); ++linkItr)
		  
//	       {	 
//		  const TrackFitSVRHitLink& linkData = (*linkItr)->linkData(); 

//		  DABoolean isHitUsed = linkData.used();
//		  report(DEBUG, kFacilityString)
//		     << "The hit identifier is: " << testHitItr->identifier()
//		     << "\nIs the Hit used? "  << isHitUsed << endl;
//	       }
//	    }
	    
//	 }
      //     }
//////////////////////////////////////////////////////////////////////////////

//    Setup the Helix intersector
      STL_VECTOR( HIIntersectionSurface* ) surfaces;
      STL_VECTOR( HIIntersectionSurface* ) totalCompSurface;
 
//    Make a silicon surface factory
      HISiSurfaceFactory siSurfaceFactory( *iFrame, 0, 0, false );

//    check if factory is valid
      if ( ! siSurfaceFactory.geometryAvailable() )
      {
	 report(ALERT, kFacilityString) 
	    << "Geometry failed for surface!"
	    << "Check Code!,"
	    << endl;
      }

//    generate the surfaces - delete this at the end of track loop
      if ( siRphiHitsOnTrack.size() > 0 || siZHitsOnTrack.size() > 0)
      {       
	 siSurfaceFactory.generateCenterSurfacesWithHits( 
	    siRphiHitsOnTrack, siZHitsOnTrack, 0);
	 siSurfaceFactory.transferSurfacesTo( surfaces, false );

//    Make one composite surface from all the layers
	 totalCompSurface.push_back( *(surfaces.begin()) );

	 for(Count surfaceCtr=1; surfaceCtr < surfaces.size(); surfaceCtr++)
	 {
	    ( (HICompositeSurface*)(*totalCompSurface.begin()))
	       ->addChildren( *((HICompositeSurface*)surfaces[surfaceCtr]) );
	 }
      }

/////Fill vector with sensors for Rphi Hits and find the residuals////////////
      
      STL_VECTOR(SmallCount) siSensorsForRphiHits; 

// Get the list of sensors for the hits by looping over the Rphi hits
      for(STL_VECTOR(const CalibratedSVRphiHit* )::const_iterator 
	  siRphiHitIter = siRphiHitsOnTrack.begin(); 
	  siRphiHitIter != siRphiHitsOnTrack.end(); siRphiHitIter++) 
      {
	 if ( debugOn && eventCtr == m_checkEvent )
	 { report(DEBUG, kFacilityString) 
	    <<"\nWe are in event:  " << eventCtr
	    <<"\nThe hit:  " << *(*siRphiHitIter) 
	    << "\nThe sensor is located at:  " 
	    << iStore->coorSensorInSi( (*siRphiHitIter)->sensor() )
	    << endl;
	 }

	 siSensorsForRphiHits.push_back( (*siRphiHitIter)->sensor() );	    
      }

      matchSVHits( siSensorsForRphiHits, dualHelix, totalCompSurface, 
		   CalibratedHit::SVR, eventCtr, trackCtr, residualRphiCtr,
		   missedHits, totalHits, iStore, chisq, histogramsOn,
		   rPhi_TrackHits, debugOn);

/////////////////////////Now do the same for Z/////////////////////////////////
      STL_VECTOR(SmallCount) siSensorsForZHits; 

// Get the list of sensors for the hits by looping over the R-Phi hits
      for(STL_VECTOR(const CalibratedSVZHit* )::const_iterator 
	  siZHitIter = siZHitsOnTrack.begin(); 
	  siZHitIter != siZHitsOnTrack.end(); siZHitIter++) 
      {
	 siSensorsForZHits.push_back( (*siZHitIter)->sensor() );	    
      }
      
      matchSVHits( siSensorsForZHits, dualHelix, totalCompSurface, 
		   CalibratedHit::SVZ, eventCtr, trackCtr, residualZCtr,
		   missedHits, totalHits, iStore, chisq, histogramsOn,
		   z_TrackHits, debugOn);

/////////////////////////////////////////////////////////////////////////////

      //delete contents of STL vector for surfaces, and hits
      if ( siRphiHitsOnTrack.size() > 0 || siZHitsOnTrack.size() > 0)
      {  for ( Count surfCtr =0; surfCtr < surfaces.size(); surfCtr++ )      
         { 
	    surfaces[surfCtr]->deleteCondition();
	    delete surfaces[surfCtr]; 
	    
	 }

      }
      
      }//end loop over tracks

   }  //end loop over events

   cout
      << "Number of R-phi residuals calculated was:  " 
      << residualRphiCtr
      << "\nNumber of Z residuals calculated was:  " 
      << residualZCtr 
      << "\nNumber of R-phi+Z hits was: "
      << totalHits
      << "\nThe number of missed hits was:  "
      << missedHits
      << endl; 

   m_iteration = 0;

   return;      
}


/////////////////////////////////////////////////////////////////////////////

void
ChisqForSiAlignment::matchSVHits(STL_VECTOR(SmallCount)& siSensorsForHits,  
				 FATable< DualTrackHelices >::const_iterator& 
				 dualHelix,       
				 STL_VECTOR( HIIntersectionSurface* )& 
				 totalCompSurface, 
				 CalibratedHit::DeviceType hitType,
				 Count eventCtr, Count trackCtr,
				 Count& residualCtr, 
				 Count& missedHits, Count& totalHits,
				 FAItem<ASiStore>& iStore, double& chisq,
				 DABoolean histogramsOn, 
				 HINtuple* hist_TrackHits,
				 DABoolean debugOn
				 )

{
// Temporary normalization until we get some real fitting weights
// For now we're using the sigma's of the residual distributions

// Careful!  We are assuming norm will get overwritten
   float norm = 1;
   float cut = .015;

// For Debugging
   double sumD0 = 0.0;
   double sumZ0 = 0.0;
   double sumPx = 0.0;
   double sumPy = 0.0;
   double sumPz = 0.0;

// Check that hitType is the correct kind
   if ( hitType != CalibratedHit::SVR && hitType != CalibratedHit::SVZ )
   {
      report(ALERT, kFacilityString)
	 << "Incorrect hit type passed to ChisqForSiAlignment::matchSVHits"
	 << endl;      
      return;
   }

// Initialize residual values, and ntuple variables
   Meters residual = 0.0;
   static float ntuple[kNtupleSize];
//   ntuple[0] = 0.0;
   Count hitCtr = 1;

// loop over the hits
   for(STL_VECTOR(SmallCount )::const_iterator hitIter = 
	  siSensorsForHits.begin(); 
       hitIter != siSensorsForHits.end(); 
       hitIter++, hitCtr++) {

//    Check to see if the hit is on the correct component
//    !!Eventually we may have to move this inside the while loop
      DABoolean onComponent = selectComponent(*hitIter, iStore);

//    If its not, then skip this hit and check the next one
      if ( !onComponent )
      { continue; }

// Temporary code until we can remove bad hits from the Track Selector
      if( (m_iteration == 0) && isHitBad(eventCtr, trackCtr, hitCtr, hitType) )
      { continue; }

//    Extract the stored helix	 
      HIHelix intHelix = dualHelix->helix();

//    Make an intersector object
      HIHelixIntersector intersector( totalCompSurface, 
	    HIHelixIntersector::kIncreasingRadius, &intHelix );
	    
///////////DEBUGGING CODE!!//////////////////////////////////////////////////
   HIMagField::instance()->setBfield(15.0);

   if( eventCtr == m_checkEvent && debugOn && hitIter == siSensorsForHits.begin() && hitType == 1)
   {
      report(DEBUG, kFacilityString) 
	 <<"\nD0 " << intHelix.d0()
	 <<"\nZ0 " << intHelix.z0()
	 <<"\npX " << (intHelix.momentum()).x()
	 <<"\npY " << (intHelix.momentum()).y()
	 <<"\npZ " << (intHelix.momentum()).z()
	 << endl;
   }

   if( eventCtr == m_checkEvent && debugOn && trackCtr == 2 && hitIter == siSensorsForHits.begin() && hitType == 1)
   { sumD0 = (dualHelix->helix()).d0() 
	+ ( (dualHelix-1)->helix() ).d0();
      
     sumZ0 = ( dualHelix->helix() ).z0() 
	 - ( (dualHelix-1)->helix() ).z0();
     
     sumPx = ((dualHelix->helix()).momentum()).x() 
	 + ( ((dualHelix-1)->helix()).momentum() ).x();
     
     sumPy = ((dualHelix->helix()).momentum()).y() 
	 + ( ((dualHelix-1)->helix()).momentum() ).y();
     
     sumPz = ((dualHelix->helix()).momentum()).z() 
	 + ( ((dualHelix-1)->helix()).momentum() ).z();
	 
   }

//    In genereral don't print Diagnostics
      intersector.setPrintDiagnostics(false);
      ( intersector.currentSurface() )->setPrintDiagnostics(false);

//       print Diagnostics if hit is missed	    
      if( debugOn && eventCtr == m_checkEvent)
      {
	 intersector.setPrintDiagnostics(true);

	 //Loop over the surfaces
	 for(Count ctr =0; ctr < totalCompSurface.size(); ctr++) 
	 {
	    totalCompSurface[ctr]->setPrintDiagnostics(true);

	 }

      }
      
//////////////////////////////////////////////////////////////////////////

//    initialize "status" at the first intersection surface
      HIHelixIntersector::IntersectionStatus status =
	    intersector.swimToCurrentSurface(KTMoveControl::kDirectionForward);

/////////////////More Code for DEBUGGING!!//////////////////////////////////
      Count residual_check = 0;
      totalHits += 1;

      if( debugOn && eventCtr == m_checkEvent  ) 
      { report(DEBUG, kFacilityString) 
	 <<"\nHere is some Helix info - Just initialized status"
	 <<"\nposition:  " << intHelix.position()
	 <<"\nmomentum:  " << intHelix.momentum()
	 <<"\nmomentum mag:  " << (intHelix.momentum()).mag()
	 << endl;
      }

      if( status != HIHelixIntersector::kIntersectionOK )
      {
	 missedHits++;	    
	 residual_check = 1;

	 if (debugOn)
	 {  report(DEBUG, kFacilityString) 
	    << "\nWe are in event:  " << eventCtr 
	    << "\nMissed a hit due to bad status!" 
	    << "\nThe status is: " << status
	    << endl;
	 }

      }

///////////////////////////////////////////////////////////////////////////////

      while (status == HIHelixIntersector::kIntersectionOK )
      {	 
	    if( debugOn && eventCtr == m_checkEvent  ) 
	    { 
	       report(DEBUG, kFacilityString) 
	       <<"moved to next intersection" << endl; 
	    }

	 HIIntersectionSurface* surface = intersector.currentSurface();
	       
	 // Loop over all hits in this surface
	    do
	    {  
	       if( debugOn && eventCtr == m_checkEvent  ) 
	       { 
		  report(DEBUG, kFacilityString) 
		     <<"Advancing to next calibrated hit in the surface"
		     << endl; 
	       }

	    //Determine what kind of hit this is
	       const CalibratedHit* aHit = surface->calibratedHit();

	       if( debugOn && eventCtr == m_checkEvent  ) 
	       { 
		  report(DEBUG, kFacilityString)
		  <<"It is of hit type:  " << aHit->deviceType() << endl; 
	       }

	    //Check to see if its the kind of hit we're interested in
	       if ( aHit->deviceType() == hitType )
	       {
		  DABoolean isHitGood = false;
	       
	       // Match sensors if its an SVR hit
		  if ( hitType == CalibratedHit::SVR )
		  {
		  const CalibratedSVRphiHit* anSiHit =
		     (const CalibratedSVRphiHit* ) aHit;
		  norm = m_normRphi;
		  cut = m_cutRphi;

		  if( debugOn && eventCtr == m_checkEvent  ) 
		  {  
		     report(DEBUG, kFacilityString) 
		     <<"\nThe calibrated hit is on sensor:  "
		     << anSiHit->sensor()
		     << endl;		     
		  }
		  
		  if( anSiHit->sensor() == *hitIter)
		  { 
		     isHitGood = true; 

		     if ( histogramsOn )
		     {  SmallCount hybrid = anSiHit->hybrid(); 
		     double x = (intHelix.position()).x();
		     double y = (intHelix.position()).y();
		     
		     ntuple[kCurvature] = intHelix.curvature();
		     ntuple[kClamshell] = iStore-> clamshellForHybrid(hybrid);
		     ntuple[kZ] = (intHelix.position()).z(); 
		     ntuple[kPhi0] = (intHelix).phi0();
		     ntuple[kCotTheta] = (intHelix).cotTheta();
		     ntuple[kEventCtr] = eventCtr;

		    }
		    
		  }		  
	       }

	       // Or if its an SVZ hit.. 
	       // Caution! hitType must be SVR or SVZ - inserted a check above
	       else
	       {
		  const CalibratedSVZHit* anSiHit =
		     (const CalibratedSVZHit* ) aHit;
		  norm = m_normZ;
		  cut = m_cutZ;
		  
		  if( anSiHit->sensor() == *hitIter)
		  { 
		     isHitGood = true; 

		     if ( histogramsOn )
		     {  SmallCount hybrid = anSiHit->hybrid(); 

			double x = (intHelix.position()).x();
			double y = (intHelix.position()).y();

		        ntuple[kCurvature] = intHelix.curvature();
		        ntuple[kClamshell] = iStore-> clamshellForHybrid(hybrid);
		        ntuple[kZ] = (intHelix.position()).z(); 
			ntuple[kPhi0] = (intHelix).phi0();
			ntuple[kEventCtr] = eventCtr;

		     }
		  }
	       }

	       // Does the sensor number match the one we want?
	       if ( isHitGood )		     
	       {  
		  if( debugOn && eventCtr == m_checkEvent  ) 
		  { 
		     report(DEBUG, kFacilityString)
		     <<"We matched a hit!! - Calculating residual.." << endl; 
		  }
                  //Get the residual !!
		  residual = ( surface->dca( intHelix ) );
		  	
		  //Check that residual is not identically zero
		  if (residual == 0.0 )
		  { report(ALERT, kFacilityString)
		     <<"!! residual is identically zero !!" << endl; }
	
		  // fill histograms if that option is requested
		  if ( histogramsOn )
		  {  
		     ntuple[kResidual] = residual;
		     ntuple[kSensor] = *hitIter; //storing sensor number
		     ntuple[kLadder] = iStore-> ladderForSensor(*hitIter);
		     hist_TrackHits->fill(ntuple); 

		     //For checking residuals sums!
//		     ntuple[kSumResidual] += residual;
//		     ntuple[kZ] = (intHelix.position()).z();
//		     ntuple[kPhi0] = intHelix.phi0();

		  }			    

		  //Counts for DEBUGGING
		  residual_check = 1;
		  residualCtr++;	       
		  
		  //Store event numbers of bad residuals to be filtered
		  if ( m_iteration == 1 && (abs(residual) > cut) )
		  {
		     if( hitType == CalibratedHit::SVR)
		     { 
			(m_removeHitsForRphi[kEvent]).push_back(eventCtr); 
			(m_removeHitsForRphi[kTrack]).push_back(trackCtr); 
			(m_removeHitsForRphi[kHit]).push_back(hitCtr); 

		     }
		     else 
		     { 
			(m_removeHitsForZ[kEvent]).push_back(eventCtr); 
			(m_removeHitsForZ[kTrack]).push_back(trackCtr); 
			(m_removeHitsForZ[kHit]).push_back(hitCtr);

		     }
		     
		     //set residual value to zero if its a bad event
		     residual = 0;
		  }
		       
		  //Calculate the chi square
		  chisq += (residual/norm)*(residual/norm);
		  
	       }

	   }
	    
	 } while (surface->
		  advanceToNextCalibratedHit(intHelix)); 
	 
	 // swim to the next surface
	 status = intersector.swimToNextIntersection(
	    KTMoveControl::kDirectionForward );

	 if( debugOn  && eventCtr == m_checkEvent ) 
	 { 
	    report(DEBUG, kFacilityString)
	    <<"\nHere is some Helix info - Bottom of outer while loop"
	    <<"\nposition:  " << intHelix.position()
	    <<"\nmomentum:  " << intHelix.momentum()
	    <<"\nmomentum mag:  " << (intHelix.momentum()).mag()
	    << endl;
	      }

      } //close outer do-while loop
      
      //Try resetting surfaces here instead
      intersector.resetSurfaces();

      //Temporary fix for hits with failed residuals
      if(residual_check == 0)
      { 
	 missedHits++;	    

	 if( debugOn )
	 {
	    report(DEBUG, kFacilityString) 
	       << "\nMissed a hit due to something else!" 
	       << "\nWe are in event:  " << eventCtr
	       << "\nNumber of hits is:  " << siSensorsForHits.size()
	       << "\nThe failed hit is:  " << hitCtr
	       << "\nThe hit type is:  " << hitType
	       << "\nThe ladder is:  "  << iStore->ladderForSensor(*hitIter) 
	       << "\nThe sensor is located at:  " 
	       << iStore->coorSensorInSi(*hitIter)
	       << endl;
	 }
      }

      if( debugOn  && eventCtr == m_checkEvent ) 
      { 
	 report(DEBUG, kFacilityString)
	 <<"\nMoving on to the next track hit!" << endl;
      }

   }//end loop over hits

// For checking residual sums!   
//   ntuple[kLayersHit] = layersHit[0] + layersHit[1];
//   hist_TrackHits->fill(ntuple); 
   
return;
}

//////////////////////////////////////////////////////////////////////////////
DABoolean
ChisqForSiAlignment::isHitBad(Count eventCtr, Count trackCtr, Count hitCtr,
				CalibratedHit::DeviceType hitType)
{
   if (hitType != CalibratedHit::SVR && hitType != CalibratedHit::SVZ )
   {
      report( ALERT, kFacilityString )
	 <<"Wrong hit type in ChisqForSiAlignment::isHitBad!  Check Code!"
	 << endl;
   }
         
   if ( hitType == CalibratedHit::SVR )
   {
      for(Count i=0; i < (m_removeHitsForRphi[kEvent]).size(); i++)
      {	 
	 if ( eventCtr == m_removeHitsForRphi[kEvent][i] && 
	      trackCtr == m_removeHitsForRphi[kTrack][i] &&
	      hitCtr == m_removeHitsForRphi[kHit][i] )
	 { return true; }

      }

   }

   else 
   {
      for(Count j=0; j < (m_removeHitsForZ[kEvent]).size(); j++)
      {
	 if ( eventCtr == m_removeHitsForZ[kEvent][j] && 
	      trackCtr == m_removeHitsForZ[kTrack][j] &&
	      hitCtr == m_removeHitsForZ[kHit][j] )
	 { return true; }
      }
   }

return false;
}
//////////////////////////////////////////////////////////////////////////////
//double 
//ChisqForSiAlignment::getSigmaRes(HIIntersectionSurface* surface, 
//				 HIHelix& intHelix,
//				 FATable< DualTrackHelices >::const_iterator& 
//				 dualHelix)

//{

//get covariance matrix
//   HepSymMatrix covMatrix = dualHelix->errMatrix();
		      
/*   if (eventCtr == 2)
   {

      cout 
	 << "\nElement (2,3) is:  " << covMatrix(2,3) 
	 << "\nElement (3,4) is:  " << covMatrix(3,4) 
	 << "\nElement (5,5) is:  " << covMatrix(5,5) 
	 << "\n"
	 << endl;
   } */

   //get DCA derivative vectors
//   HepVector dcaDerivatives(HIHelix::kNTrackParameters);
//  surface->derivatives(intHelix, dcaDerivatives); 

//   Meters fitWeight = surface->fittingWeight();

/*   if (eventCtr == 2)
   {     cout << "The size of dcaDerivatives:  " 
	      << dcaDerivatives.num_row()
	      << dcaDerivatives[0] << " "
	      << dcaDerivatives[1] << " "
	      << dcaDerivatives[2] << " "
	      << dcaDerivatives[3] << " "
	      << dcaDerivatives[4] << " "
	      << endl; } */
		     
      //multiply to get sigmaProjSqrd
//   double sigmaProjSqrd
      //     = covMatrix.similarity(dcaDerivatives);
		      
/*		      cout 
		      << "the value of sigmaProjSquared: " 
		      << sigmaProjSqrd
		      <<endl;  */
		      
//   return sigmaProjSqrd;

//}


//
// const member functions
//

//
// static member functions
//


