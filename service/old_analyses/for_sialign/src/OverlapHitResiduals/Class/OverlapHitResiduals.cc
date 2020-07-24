// -*- C++ -*-
//
// Package:     <OverlapHitResiduals>
// Module:      OverlapHitResiduals
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Sat Jun  3 09:58:31 EDT 2000
// $Id: OverlapHitResiduals.cc,v 1.1.1.1 2001/03/06 22:55:18 llh14 Exp $
//
// Revision history
//
// $Log: OverlapHitResiduals.cc,v $
// Revision 1.1.1.1  2001/03/06 22:55:18  llh14
// imported OverlapHitResiduals
//
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
#include "Experiment/report.h"
#include "OverlapHitResiduals/OverlapHitResiduals.h"
#include "OverlapHitInfo/OverlapHitInfo.h"

#include "DualTrackHelices/DualTrackHelices.h"
#include "STLUtility/fwd_vector.h"
#include "STLUtility/fwd_multimap.h"
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
static const char* const kFacilityString = "OverlapHitResiduals.OverlapHitResiduals" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: OverlapHitResiduals.cc,v 1.1.1.1 2001/03/06 22:55:18 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
OverlapHitResiduals::OverlapHitResiduals() : 
   m_checkEvent(96), MIFcn(), m_iteration(1)   

{

}

// OverlapHitResiduals::OverlapHitResiduals( const OverlapHitResiduals& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

OverlapHitResiduals::~OverlapHitResiduals()
{
   report( DEBUG, kFacilityString)
      << "In destructor"
      << endl;

}

//
// assignment operators
//
// const OverlapHitResiduals& OverlapHitResiduals::operator=( const OverlapHitResiduals& rhs )
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
OverlapHitResiduals::calcResiduals(const FIFrameIterator& iBegin, 
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
				   HINtuple* z_TrackHits)

{     
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
      STL_VECTOR( const CalibratedSVRphiHit* ) siRphiHitsOnTrack =
	 *(trackIter->seedSVRHits());  //This comes from Doit
//	 *(trackIter->muonSVRHits());  //This comes from Kalman

      STL_VECTOR( const CalibratedSVZHit* ) siZHitsOnTrack =
	 *(trackIter->seedSVZHits());  //This comes from Doit
//         *(trackIter->muonSVZHits());  //This comes from Kalman

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
      SmallCount rSensor;
      SmallCount rLayer;
      SmallCount zSensor;
      SmallCount zLayer;

// A clumsy way to do this, but I don't know a better way.
      STL_VECTOR(STL_VECTOR(SmallCount)) siSensorsForRphiHits; 
      STL_VECTOR(SmallCount) lay1_RSensors;
      siSensorsForRphiHits.push_back(lay1_RSensors);
      STL_VECTOR(SmallCount) lay2_RSensors;
      siSensorsForRphiHits.push_back(lay2_RSensors);
      STL_VECTOR(SmallCount) lay3_RSensors;
      siSensorsForRphiHits.push_back(lay3_RSensors);
      STL_VECTOR(SmallCount) lay4_RSensors;
      siSensorsForRphiHits.push_back(lay4_RSensors);

// Get the list of sensors for the hits by looping over the Rphi hits
// Store them by layer
      for(STL_VECTOR(const CalibratedSVRphiHit* )::const_iterator 
	  siRphiHitIter = siRphiHitsOnTrack.begin(); 
	  siRphiHitIter != siRphiHitsOnTrack.end(); siRphiHitIter++) 
      {
	 rSensor = (*siRphiHitIter)->sensor();
	 rLayer = iStore->layerForSensor(rSensor);

	 (siSensorsForRphiHits[rLayer-1]).push_back( rSensor );	    
      }

//  Loop over the layers for rphi hits
      for(Count rlayerCtr = 0; rlayerCtr < siSensorsForRphiHits.size(); 
	  rlayerCtr++)
      {

	 // Check to see if there are multiple hits in the layer 
	 if( (siSensorsForRphiHits[rlayerCtr]).size() > 1 )
	    
	 { 

	    const SmallCount numRphiHits = 
	       siSensorsForRphiHits[rlayerCtr].size();
	    
	    matchSVHits( siSensorsForRphiHits[rlayerCtr], numRphiHits, 
			 dualHelix, 
			 totalCompSurface, CalibratedHit::SVR, eventCtr, 
			 trackCtr, residualRphiCtr, missedHits, totalHits, 
			 iStore, chisq, histogramsOn, rPhi_TrackHits, debugOn);
	 }

      } //end loop over layers for rphi hits

/////////////////////////Now do the same for Z/////////////////////////////////
      STL_VECTOR(STL_VECTOR(SmallCount)) siSensorsForZHits; 
      STL_VECTOR(SmallCount) lay1_ZSensors;
      siSensorsForZHits.push_back(lay1_ZSensors);
      STL_VECTOR(SmallCount) lay2_ZSensors;
      siSensorsForZHits.push_back(lay2_ZSensors);
      STL_VECTOR(SmallCount) lay3_ZSensors;
      siSensorsForZHits.push_back(lay3_ZSensors);
      STL_VECTOR(SmallCount) lay4_ZSensors;
      siSensorsForZHits.push_back(lay4_ZSensors);

// Get the list of sensors for the hits by looping over the Z hits
      for(STL_VECTOR(const CalibratedSVZHit* )::const_iterator 
	  siZHitIter = siZHitsOnTrack.begin(); 
	  siZHitIter != siZHitsOnTrack.end(); siZHitIter++) 
      {
	 zSensor = (*siZHitIter)->sensor();
	 zLayer = iStore->layerForSensor(zSensor);
	       
	 siSensorsForZHits[zLayer-1].push_back( zSensor );	    
      }
      

//  Loop over the layers for z hits
      for(Count zlayerCtr = 0; zlayerCtr < siSensorsForZHits.size(); 
	  zlayerCtr++)
      {

	 // Check to see if there are multiple hits in the layer 
	 if( (siSensorsForZHits[zlayerCtr]).size() > 1 )
	    
	 { 

	    const SmallCount numZHits = siSensorsForZHits[zlayerCtr].size();

	    matchSVHits( siSensorsForZHits[zlayerCtr], numZHits, dualHelix, 
			 totalCompSurface, CalibratedHit::SVZ, eventCtr, 
			 trackCtr, residualZCtr, missedHits, totalHits, 
			 iStore, chisq, histogramsOn, z_TrackHits, debugOn);
	    
	 }
      } //end loop over layers for z hits

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
OverlapHitResiduals::matchSVHits(STL_VECTOR(SmallCount)& siSensorsForHits,  
				 const SmallCount numHits2,
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

// Careful!  We are assuming cut will get overwritten
   float cut = .015;

   Count startLadder[kNumLayers] = { 1, 8, 18, 36 };
   Count endLadder[kNumLayers] = { 7, 17, 35, 61 };

//   STL_VECTOR(float) res_Lad(numHits, 0.0);
   STL_MULTIMAP(int, OverlapHitInfo*) res_Lad;

// Check that hitType is the correct kind
   if ( hitType != CalibratedHit::SVR && hitType != CalibratedHit::SVZ )
   {
      report(ALERT, kFacilityString)
	 << "Incorrect hit type passed to OverlapHitResiduals::matchSVHits"
	 << endl;      
      return;
   }

// Initialize residual values, and ntuple variables
   Meters residual = 0.0;
   static float ntuple[kNtupleSize];
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

//    Extract the stored helix	 
      HIHelix intHelix = dualHelix->helix();

//    Make an intersector object
      HIHelixIntersector intersector( totalCompSurface, 
	    HIHelixIntersector::kIncreasingRadius, &intHelix );

//    initialize "status" at the first intersection surface
      HIHelixIntersector::IntersectionStatus status =
	    intersector.swimToCurrentSurface(KTMoveControl::kDirectionForward);

      OverlapHitInfo overlapHitInfo();

      while (status == HIHelixIntersector::kIntersectionOK )
      {	 

	    HIIntersectionSurface* surface = intersector.currentSurface();
	       
	 // Loop over all hits in this surface
	    do
	    {  

	    //Determine what kind of hit this is
	       const CalibratedHit* aHit = surface->calibratedHit();

	    //Check to see if its the kind of hit we're interested in
	       if ( aHit->deviceType() == hitType )
	       {
		  DABoolean isHitGood = false;
	       
	       // Match sensors if its an SVR hit
		  if ( hitType == CalibratedHit::SVR )
		  {
		  const CalibratedSVRphiHit* anSiHit =
		     (const CalibratedSVRphiHit* ) aHit;
		  
		  if( anSiHit->sensor() == *hitIter)
		  { 
		     isHitGood = true; 

		     if ( histogramsOn )
		     {  SmallCount hybrid = anSiHit->hybrid(); 
		     double x = (intHelix.position()).x();
		     double y = (intHelix.position()).y();
		     
		    }
		    
		  }		  
	       }

	       // Or if its an SVZ hit.. 
	       // Caution! hitType must be SVR or SVZ - inserted a check above
	       else
	       {
		  const CalibratedSVZHit* anSiHit =
		     (const CalibratedSVZHit* ) aHit;
		  
		  if( anSiHit->sensor() == *hitIter)
		  { 
		     isHitGood = true; 

		     if ( histogramsOn )
		     {  SmallCount hybrid = anSiHit->hybrid(); 

			double x = (intHelix.position()).x();
			double y = (intHelix.position()).y();

		     }
		  }
	       }

	       // Does the sensor number match the one we want?
	       if ( isHitGood )		     
	       {  
                  //Get the residual !!
		  residual = ( surface->dca( intHelix ) );
		  	
		  //Check that residual is not identically zero
		  if (residual == 0.0 )
		  { report(ALERT, kFacilityString)
		     <<"!! residual is identically zero !!" << endl; }
	
		  // fill histograms if that option is requested
		  if ( histogramsOn )
		  {  
		     int ladder = iStore->ladderForSensor(*hitIter);
		     int layer = iStore->layerForSensor(*hitIter);
		     double z = (intHelix.position()).z();
		     double phi0 = intHelix.phi0();

		     res_Lad.insert( make_pair(ladder, 
			     new OverlapHitInfo(residual, z, phi0, layer)) );

		  }			    

		  residualCtr++;	       
		  		       
	       }

	   }
	    
	 } while (surface->
		  advanceToNextCalibratedHit(intHelix)); 
	 
	 // swim to the next surface
	 status = intersector.swimToNextIntersection(
	    KTMoveControl::kDirectionForward );

      } //close outer do-while loop
      
      //Try resetting surfaces here instead
      intersector.resetSurfaces();

   }//end loop over hits


/////////   Calculating the residual differences ////////////////////////////
   int currentLadder = 0;
   SmallCount currentLayer = 0;
   double resDiff = 0;
  
// Loop over the pairs    
   for(STL_MULTIMAP(int, OverlapHitInfo*)::const_iterator currentLadItr = 
	  res_Lad.begin(); currentLadItr != res_Lad.end(); currentLadItr++)
      
   {
      currentLadder = (*currentLadItr).first;
      currentLayer = ((*currentLadItr).second)->getLayer();
      int numHits;

// Get the number of hits on the adjacent ladder
      if ( currentLadder == endLadder[currentLayer-1] )
      {  
         numHits = res_Lad.count( startLadder[currentLayer-1] );
      }

      else
      {
	 numHits = res_Lad.count(currentLadder+1); 
      }

// Check to see if there are any hits on the adjacent ladder
      if( numHits > 0) 
      {
	 STL_MULTIMAP(int, OverlapHitInfo*)::const_iterator nextLadItr;

	 if  (currentLadder!= endLadder[currentLayer -1])
	 {   
            //  Get the first iterator hits on the next ladder
	    nextLadItr = res_Lad.upper_bound(currentLadder);
	 }

	 else
	 {
	    //  Set the nextLadItr to the first ladder
	    nextLadItr =  res_Lad.begin();
	 }

	    for(Count resHitCtr = 0 ; resHitCtr < numHits ; 
		nextLadItr++, resHitCtr++)
	    {

	       if(currentLadder == endLadder[currentLayer - 1] && debugOn)
		  {  cout<<"\nThe currentLadder is: " << currentLadder
			 <<"\nThe adjacent Ladder is: " << (*nextLadItr).first
			 <<"\nThe residuals is: " << (*nextLadItr).second
			 << endl;
		  }

	       ntuple[kResCurrent] = ((*currentLadItr).second)->getResidual();
	       ntuple[kResNext] = ((*nextLadItr).second)->getResidual();

	       ntuple[kResDiff] = ((*nextLadItr).second)->getResidual() 
		  - ((*currentLadItr).second)->getResidual();
	       ntuple[kZDiff] = ((*nextLadItr).second)->getZ() 
		  - ((*currentLadItr).second)->getZ();
	       ntuple[kZ] = ((*currentLadItr).second)->getZ();
	       ntuple[kPhi0Diff] = ((*nextLadItr).second)->getPhi0() 
		  - ((*currentLadItr).second)->getPhi0();
	       ntuple[kPhi0] = ((*currentLadItr).second)->getPhi0();
	       ntuple[kEventCtr] = eventCtr;
	       ntuple[kLadder] = (*currentLadItr).first;
	       ntuple[kLayer] = currentLayer;
	       
	       hist_TrackHits->fill(ntuple); 
		
	    }//end loop over adjacent ladder hits
      }//end if       
   }//end loop over ladders with hits
 
   STL_MULTIMAP(int, OverlapHitInfo*)::const_iterator deleteItr 
      = res_Lad.begin();

   for( ; deleteItr != res_Lad.end(); deleteItr++)
   {
      delete (*deleteItr).second;
   }
   
return;
}

//////////////////////////////////////////////////////////////////////////////
//double 
//OverlapHitResiduals::getSigmaRes(HIIntersectionSurface* surface, 
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


