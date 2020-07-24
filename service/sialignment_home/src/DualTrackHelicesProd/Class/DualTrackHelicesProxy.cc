// -*- C++ -*-
//
// Package:     <DualTrackHelicesProd>
// Module:      DualTrackHelicesProxy
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Fri Jul  7 17:21:23 EDT 2000
// $Id: DualTrackHelicesProxy.cc,v 1.3 2001/08/23 17:05:13 mccann Exp $
//
// Revision history
//
// $Log: DualTrackHelicesProxy.cc,v $
// Revision 1.3  2001/08/23 17:05:13  mccann
// HIDualTrackConstraint constructor changed. DualTrackHelicesProxy ought to have the same behaviour now.
//
// Revision 1.2  2001/03/26 16:45:50  bkh
// Get MagneticField from StartRun not BeginRun
//
// Revision 1.1.1.1  2001/02/01 17:40:14  llh14
// imported DualTrackHelicesProd
//
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

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

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"           
#include "FrameAccess/FATable.h"
#include "FrameAccess/FAPtrTable.h"

#include "ChisqFitter/ChisqTrackFitter.h"
#include "KalmanFilter/KalmanTrackFitter.h"

#include "ASiStorePro/ASiStore.h"

#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"

#include "TrackRoot/TRSeedTrack.h"

#include "Navigation/NavTrack.h"

#include "STLUtility/fwd_vector.h"

#include "HelixIntersection/HIFitHelix.h"
#include "HelixIntersection/HIMagField.h"

#include "DualTrackHelicesProd/DualTrackHelicesProxy.h"
#include "DualTrackHelicesProd/DualTrackHelicesProd.h"
#include "ProxyBind/ProxyBoundMethodAction.h" //need for PROXY_BIND_METHOD

#include "BeamEnergy/BeamEnergy.h"

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
static const char* const kFacilityString = "DualTrackHelices.DualTrackHelicesProxy" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DualTrackHelicesProxy.cc,v 1.3 2001/08/23 17:05:13 mccann Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DualTrackHelicesProxy::DualTrackHelicesProxy(DualTrackHelicesProd* aDualTrackHelicesProd)
   : m_ptrTable( new value_type ),
     m_DualTrackHelicesProd( aDualTrackHelicesProd )
{
   if ( 0 == m_ptrTable ) {
      report( EMERGENCY, kFacilityString )
         << "Unable to allocate memory"
         << endl ;
      ::exit( 1 ) ;
   }

   //bind 'auxilliary' functions that you want called when a
   // new Record on the particular Stream appears
   //bind( &DualTrackHelicesProxy::boundMethod, Stream::kBeginRun );
}

// DualTrackHelicesProxy::DualTrackHelicesProxy( const DualTrackHelicesProxy& iproxy )
// {
//   *this = iproxy;
// }

DualTrackHelicesProxy::~DualTrackHelicesProxy()
{
   // calls "eraseAll" internally!
   delete m_ptrTable ;
   report( DEBUG, kFacilityString)
      << "In destructor"
      << endl;

}

//
// assignment operators
//
// const DualTrackHelicesProxy& DualTrackHelicesProxy::operator=( const DualTrackHelicesProxy& iproxy )
// {
//   if( this != &iproxy ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( iproxy );
//   }
//
//   return *this;
// }

//
// member functions
//
void
DualTrackHelicesProxy::invalidateCache()
{
   if ( 0 != m_ptrTable ) {
      (*m_ptrTable).eraseAll() ;
   }
}

const DualTrackHelicesProxy::value_type*
DualTrackHelicesProxy::faultHandler( const Record& iRecord,
			      const DataKey& iKey )
{
   DABoolean debugOn = false;
   DABoolean useAllHits = m_DualTrackHelicesProd->useAllHits();

   Frame& iFrame = iRecord.frame();
   FAItem<BeamEnergy> beam_energy;
   extract(iFrame.record(Stream::kStartRun), beam_energy);

   HIDualTrackConstraint constraint(
      0.0025, beam_energy->value(), HepVector3D( -1., 0., 0. ), false, 0.105658 );

   // be assertive: table should be empty 
   // else it's a programming error
   assert( 0 == (*m_ptrTable).size() );

// Extract tracks - using seed tracks for now
   FATable< NavTrack > eventTrackTable;
   extract( iRecord, eventTrackTable );

   if (eventTrackTable.size() != 2)
   {
      report( ALERT, kFacilityString )
	 << "The wrong number of Tracks!"
	 << endl;
      return 0;
   }

   HIMagField::instance()->setBfield( iFrame.record(Stream::kStartRun) ); 

// Make a single vector holding both tracks. 
   STL_VECTOR( HIFitHelix* ) helixVector; 

// Get two helices from NavTrack   
   HIFitHelix helix1( *( eventTrackTable.begin()->muonHelix() ), 
		       DBCandidate::mass( DBCandidate::kMuon ) );
   HIFitHelix helix2( *( (eventTrackTable.begin()+1)->muonHelix() ), 
		       DBCandidate::mass( DBCandidate::kMuon ) );

// Only use these two lines when addSiliconHits is turned off
   if(useAllHits)
   {
      helixVector.push_back( &helix1 );
      helixVector.push_back( &helix2 );
   }
// Fills the helixVector 
   else
   {
      addSiliconHits(iFrame, helixVector, eventTrackTable );
   }

   if (debugOn)
   {
      cout << "Testing the helix after deleting improvedHelix" 
	   << "\ncurvature: " << helixVector[0]->curvature()
	   << "\nphi0: " << helixVector[0]->phi0()
	   << "\nd0: " << helixVector[0]->d0()
	   << "\ncurvature: " << helixVector[1]->curvature()
	   << "\nphi0: " << helixVector[1]->phi0()
	   << "\nd0: " << helixVector[1]->d0()
	   << endl;
   }

// Apply fit constraint - helix tracks are changed now!!
   HIFitConstraint::Results results = 
      constraint.applyConstraint( helixVector );

   if (debugOn)
   {
      cout
	 << "\nConstraint chisq " 
	 << results.chisq()
	 << "\nndof " << results.ndof()
	 << endl;
   }      

// Loop over the NavTracks   
   for( Count dualTrackIter=1; dualTrackIter<=2; ++dualTrackIter)
   {
      HepSymMatrix covMatrix = 
	 helixVector[dualTrackIter-1]->errorMatrix();

      if (debugOn)
      {
	 cout 
	    << "\nElement (2,3) is:  " << covMatrix(2,3) 
	    << "\nElement (3,4) is:  " << covMatrix(3,4) 
	    << "\nElement (5,5) is:  " << covMatrix(5,5) 
	    << "\n"
	    << endl;  
      }

// Since we only work with two track events, the following line is true
      Count identifier = dualTrackIter;
      
// load our objects into the m_ptrTable
      m_ptrTable->
	 insert( new DualTrackHelices(identifier, results,
				      *helixVector[dualTrackIter-1]) );
   }
      
   if (debugOn)
   {
      cout
	 << "Track 1 constrained parameters:  "
	 << "\nCurvature: " << helixVector[0]->curvature()
	 << "\nPhi0: " << helixVector[0]->phi0()
	 << "\nD0: " << helixVector[0]->d0()
	 << "\nCotTheta: " << helixVector[0]->cotTheta()
	 << "\nZ0: " << helixVector[0]->z0()
	 
	 << "\n\nTrack 2 constrained parameters:  "
	 << "\nCurvature: " << helixVector[1]->curvature()
	 << "\nPhi0: " << helixVector[1]->phi0()
	 << "\nD0: " << helixVector[1]->d0()
	 << "\nCotTheta: " << helixVector[1]->cotTheta()
	 << "\nZ0: " << helixVector[1]->z0()
	 << endl;
   }

   return ( m_ptrTable ) ;
}

///////////////////////////////////////////////////////////////////////////////
void
DualTrackHelicesProxy::addSiliconHits(Frame& iFrame,
				      STL_VECTOR(HIFitHelix*)&
				      helixVector, 
				      const FATable<NavTrack>& 
				      eventTrackTable)

{
   DABoolean useLayer[kNumLayers];
   SmallCount excludeLayers[kNumLayers];
   double svrFittingWeight = m_DualTrackHelicesProd->svrWeight();
   double svzFittingWeight = m_DualTrackHelicesProd->svzWeight();

   useLayer[kL1] = m_DualTrackHelicesProd->useLay1();
   useLayer[kL2] = m_DualTrackHelicesProd->useLay2();
   useLayer[kL3] = m_DualTrackHelicesProd->useLay3();
   useLayer[kL4] = m_DualTrackHelicesProd->useLay4();

   for( Count layCtr = 0; layCtr!= kNumLayers; layCtr++)
   {
      //save the layer number if the layer is to be excluded
      if( !useLayer[layCtr] )
      {
	 excludeLayers[layCtr] = (layCtr+1);
      }
      //otherwise save a zero placeholder
      else
      {
	 excludeLayers[layCtr] = 0;
      }
      
   }

   FAItem<ASiStore> iStore;
   extract(iFrame.record(Stream::kSVAlignment), iStore);

   FATable< NavTrack >::const_iterator trackIter;
// Loop over the two tracks
   for (trackIter = eventTrackTable.begin();
	   trackIter != eventTrackTable.end(); ++trackIter) 
   {

// Instantiate ChisqTrackFitter
      ChisqTrackFitter fitter;  

// fitter.setPrintDiagnostics(true);

      fitter.setIncludeInitErrorMatrix(true);

// set fitting weights
      fitter.setSVRFittingWeight (svrFittingWeight); 
      fitter.setSVZFittingWeight (svzFittingWeight);
      
// Is this really neccessary?
//      fitter.setMaxHitsToDelete(0);

   //Force the Nav Track to be a seed track and feed it to Chisq
      const TRSeedTrack* aSeed = (TRSeedTrack*)(&*trackIter->muonHelix() );
   
// Get the list of silicon hits - remove those with bad hits
      STL_VECTOR( const CalibratedSVRphiHit* ) siRphiSeedHits =
	 *(trackIter->seedSVRHits());
      STL_VECTOR( const CalibratedSVZHit* ) siZSeedHits =
	 *(trackIter->seedSVZHits());
      
// Filter the list of RphiSeed Hits to remove unwanted layers //////////////
      STL_VECTOR( const CalibratedSVRphiHit* ) siRphiFilteredHits;
      
      STL_VECTOR( const CalibratedSVRphiHit* )::const_iterator rPhiHitIter;  
      for(rPhiHitIter = siRphiSeedHits.begin(); 
	  rPhiHitIter != siRphiSeedHits.end(); rPhiHitIter++)
      {
	 SmallCount sensor = (*rPhiHitIter)->sensor();
	 SmallCount layer =iStore->layerForSensor(sensor);
	 
	 // Save this hit in a new list if it is not in the unwanted layers
	 if( layer != excludeLayers[kL1] || layer != excludeLayers[kL2]
	    || layer != excludeLayers[kL3] || layer != excludeLayers[kL4])
	 {
	    siRphiFilteredHits.push_back( *rPhiHitIter );
	 }
      }
      
      STL_VECTOR( const CalibratedSVZHit* ) siZFilteredHits;
// Filter the list of ZSeed Hits to remove unwanted layers
      STL_VECTOR( const CalibratedSVZHit* )::const_iterator ZHitIter;  
      for(ZHitIter = siZSeedHits.begin(); 
	  ZHitIter != siZSeedHits.end(); ZHitIter++)
      {
	 SmallCount sensor = (*ZHitIter)->sensor();
	 SmallCount layer =iStore->layerForSensor(sensor);
	 
	 // Save this hit in a new list if it is not in the unwanted layers
	 if( layer != excludeLayers[kL1] || layer != excludeLayers[kL2]
	     || layer != excludeLayers[kL3] || layer != excludeLayers[kL4])
	 {
	    siZFilteredHits.push_back( *ZHitIter );
	 }
      }
      
// We must remember to delete the "improvedHelix"   
      KTHelix* improvedHelix = fitter.fittedTrack( aSeed, iFrame, 0, 0, 
						   &siRphiSeedHits, 
						   &siZSeedHits, 
						   DBCandidate::kMuon, false);
      
      helixVector.push_back(new HIFitHelix(*improvedHelix) );
      
      delete improvedHelix;

   }
   
   return;

}

//
// const member functions
//

//
// static member functions
//

//
// 'bound' functions
//

//void
//DualTrackHelicesProxy::boundMethod( const Record& iRecord )
//{
//   report( INFO, kFacilityString) <<"BoundMethod: " <<iRecord.stream().value() << endl;
//}

//-----------------------------------------------
// DO NOT DELETE
#include "ProxyBind/Template/ProxyBindableTemplate.cc"
template class ProxyBindableTemplate< FAPtrTable< DualTrackHelices > >;

PROXY_BIND_METHOD( DualTrackHelicesProxy )
//-----------------------------------------------


