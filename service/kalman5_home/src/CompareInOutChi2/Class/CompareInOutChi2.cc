// -*- C++ -*-
//
// Package:     CompareInOutChi2
// Module:      CompareInOutChi2
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu May 22 16:28:03 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "CompareInOutChi2/CompareInOutChi2.h"
#include "Experiment/report.h"
#include "Experiment/units.h"  // for converting to/from standard CLEO units

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h"
#include "TrackRoot/TRHelixFit.h"
#include "TrackRoot/TRTrackFitQuality.h"
#include "TrackRoot/TRSeedTrackQuality.h"
#include "TrackDelivery/TDKinematicFit.h"



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
static const char* const kFacilityString = "Processor.CompareInOutChi2" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: processor.cc,v 1.33 2002/12/18 01:45:06 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//



//
// constructors and destructor
//
CompareInOutChi2::CompareInOutChi2( void )               // anal1
   : Processor( "CompareInOutChi2" )
{
   report( DEBUG, kFacilityString ) << "here in ctor()" << endl;

   // ---- bind a method to a stream -----
   // These lines ARE VERY IMPORTANT! If you don't bind the 
   // code you've just written (the "action") to a stream, 
   // your code won't get executed!

   bind( &CompareInOutChi2::event,    Stream::kEvent );
   //bind( &CompareInOutChi2::beginRun, Stream::kBeginRun );
   //bind( &CompareInOutChi2::endRun,   Stream::kEndRun );

   // do anything here that needs to be done at creation time
   // (e.g. allocate resources etc.)


}

CompareInOutChi2::~CompareInOutChi2()                    // anal5
{
   report( DEBUG, kFacilityString ) << "here in dtor()" << endl;
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

//
// member functions
//

// ------------ methods for beginning/end "Interactive" ------------
// --------------------------- init method -------------------------
void
CompareInOutChi2::init( void )          // anal1 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in init()" << endl;

   // do any initialization here based on Parameter Input by User
   // (e.g. run expensive algorithms that are based on parameters
   //  specified by user at run-time)

}

// -------------------- terminate method ----------------------------
void
CompareInOutChi2::terminate( void )     // anal5 "Interactive"
{
   report( DEBUG, kFacilityString ) << "here in terminate()" << endl;

   // do anything here BEFORE New Parameter Change
   // (e.g. write out result based on parameters from user-input)
 
}


// ---------------- standard place to book histograms ---------------
void
CompareInOutChi2::hist_book( HIHistoManager& iHistoManager )
{
   report( DEBUG, kFacilityString ) << "here in hist_book()" << endl;

   // book your histograms here

}

// --------------------- methods bound to streams -------------------
ActionBase::ActionResult
CompareInOutChi2::event( Frame& iFrame )          // anal3 equiv.
{
   report( DEBUG, kFacilityString ) << "here in event()" << endl;


   // Create a table of tracks and fill it.
   FATable< NavTrack > trackTable;
   extract( iFrame.record( Stream::kEvent ) , trackTable );

   if ( trackTable.size() < 5 ) return ActionBase::kFailed;

   FATable< NavTrack >::const_iterator trackTableBegin = trackTable.begin();
   FATable< NavTrack >::const_iterator trackTableEnd = trackTable.end();
   // Loop over tracks.
   for ( FATable< NavTrack >::const_iterator trackItr = trackTableBegin;
         trackItr != trackTableEnd ;
         ++trackItr )
   {
      
      // What I've learned: 1. kEntireDetector == kSVR + kSVZ + kDR3Axial + kDR3Stereo + kDRCathode, as expected
      //                    2. degreesOfFreedom + 5 == kEntireDetector, surprisingly
      //                    3. ratioNumberHitsToExpected * Expected is <= kEntireDetector
      //                    3.5   This is because numberHits is layers, it agrees with HitLayers(kEntireDetector)
      //                    4. disagreement between kEntireDetector and seedDOF+5-dropped is either symmetric Gaussian w/RMS 1.8
      //                          or extreme with kEntireDetector << seedDOF+5-dropped
      //                    5. The latter only happens when kEntireDetector < 40 (just under the peak of normalcy).
      //                    6. Occasionally pion and exit will disagree in kEntireDetector and/or ratio*Expected: usually exit has more
      //                    7. dropped never disagrees between pion and exit

      // Summary: official number of hits is seedDOF+5-dropped (assuming seedDOF is okay)
      //          kEntireDetector is which hits were actually and really used
      //          a better official: numberHitsInput() - numberHitsDropped()
      //               this number can be different for in and out
      //               numberHitsInput() must be the variable, because of seedDOF tests

      // Outstanding problems: bug1 fix got rid of the exithits << pionHits problems
      //                           but we now have a new pionHits <slightly< exitHits problem
      //                       sometimes, rarely, the fit will keep a hit it was supposed to drop
      // When you get a back-aged Kalman, run this on it.

      cout << trackItr->pionQuality()->numberSubdetectorHits(TRSubdetectorLists::kEntireDetector) << " "
	   << trackItr->pionQuality()->numberHitsInput() << " "
	   << trackItr->pionQuality()->numberHitsDropped() << " "
	   << (trackItr->pionQuality()->fit()  &&  !trackItr->pionQuality()->fitAbort()) << " "
	   << trackItr->exitPionQuality()->numberSubdetectorHits(TRSubdetectorLists::kEntireDetector) << " "
	   << trackItr->exitPionQuality()->numberHitsInput() << " "
	   << trackItr->exitPionQuality()->numberHitsDropped() << " "
	   << (trackItr->exitPionQuality()->fit()  &&  !trackItr->exitPionQuality()->fitAbort()) << endl;


//       cout << "pion "
//  	   << (trackItr->pionQuality()->degreesOfFreedom() + 5) << " "
// // 	   << (trackItr->pionQuality()->ratioNumberHitsToExpected() * trackItr->pionQuality()->numberHitsExpected()) << " "
// 	   << trackItr->pionQuality()->numberSubdetectorHits(TRSubdetectorLists::kEntireDetector) << " "
// // 	   << (trackItr->pionQuality()->numberSubdetectorHits(TRSubdetectorLists::kSVR) +
// // 	       trackItr->pionQuality()->numberSubdetectorHits(TRSubdetectorLists::kSVZ) +
// // 	       trackItr->pionQuality()->numberSubdetectorHits(TRSubdetectorLists::kDR3Axial) +
// // 	       trackItr->pionQuality()->numberSubdetectorHits(TRSubdetectorLists::kDR3Stereo) +
// // 	       trackItr->pionQuality()->numberSubdetectorHits(TRSubdetectorLists::kDRCathode)) << " "
// // 	   << (trackItr->seedQuality()->degreesOfFreedom() + 5 - trackItr->pionQuality()->numberHitsDropped()) << " "
// 	   << (int(trackItr->pionQuality()->numberHitsInput()) - int(trackItr->pionQuality()->numberHitsDropped())) << " "
// 	   << (trackItr->pionQuality()->fit()  &&  !trackItr->pionQuality()->fitAbort()) << endl;

//       cout << "exit "
//  	   << (trackItr->exitPionQuality()->degreesOfFreedom() + 5) << " "
// // 	   << (trackItr->exitPionQuality()->ratioNumberHitsToExpected() * trackItr->exitPionQuality()->numberHitsExpected()) << " "
// 	   << trackItr->exitPionQuality()->numberSubdetectorHits(TRSubdetectorLists::kEntireDetector) << " "
// // 	   << (trackItr->exitPionQuality()->numberSubdetectorHits(TRSubdetectorLists::kSVR) +
// // 	       trackItr->exitPionQuality()->numberSubdetectorHits(TRSubdetectorLists::kSVZ) +
// // 	       trackItr->exitPionQuality()->numberSubdetectorHits(TRSubdetectorLists::kDR3Axial) +
// // 	       trackItr->exitPionQuality()->numberSubdetectorHits(TRSubdetectorLists::kDR3Stereo) +
// // 	       trackItr->exitPionQuality()->numberSubdetectorHits(TRSubdetectorLists::kDRCathode)) << " "
// // 	   << (trackItr->seedQuality()->degreesOfFreedom() + 5 - trackItr->exitPionQuality()->numberHitsDropped()) << " "
// 	   << (int(trackItr->exitPionQuality()->numberHitsInput()) - int(trackItr->exitPionQuality()->numberHitsDropped())) << " "
// 	   << (trackItr->exitPionQuality()->fit()  &&  !trackItr->exitPionQuality()->fitAbort()) << endl;



   }


//       DABoolean buggy(false);

//       if ( fabs(trackItr->pionHelix()->d0()) < 0.02  &&  trackItr->pionQuality()->degreesOfFreedom() > 6 ) {
//          KTHelix helix(*trackItr->pionHelix());
//          double dummy;
//          helix.moveToRadius(0.02, dummy);
//          if ( helix.referencePoint().z() < 0. ) {
//             if (trackItr->exitPionQuality()->degreesOfFreedom() < 0 ) {
// 	       cout << "BUG1 ";
// 	       cout << trackItr->pionQuality()->chiSquare() << " "
// 		    << trackItr->exitPionQuality()->chiSquare() << endl;
// 	       buggy = true;
// 	    }

// 	 } } // end if suspicious

//       if ( fabs(trackItr->exitPionHelix()->cotTheta()) < 0.1 ) {
//          if ( trackItr->exitPionFit()->position().x() * trackItr->exitPionFit()->momentum().x() +
//               trackItr->exitPionFit()->position().y() * trackItr->exitPionFit()->momentum().y()   < 0. ) {
// 	    cout << "BUG2 ";
// 	    cout << trackItr->pionQuality()->chiSquare() << " "
// 		 << trackItr->exitPionQuality()->chiSquare() << endl;
// 	    buggy = true;
// 	 }

//       } // end if suspicious

//      if ( ! buggy ) {
// 	 cout << "OKAY ";
// 	 cout << trackItr->pionQuality()->chiSquare() << " "
// 	      << trackItr->exitPionQuality()->chiSquare() << endl;
//      }
//    }

   return ActionBase::kPassed;
}

/*
ActionBase::ActionResult
CompareInOutChi2::beginRun( Frame& iFrame )       // anal2 equiv.
{
   report( DEBUG, kFacilityString ) << "here in beginRun()" << endl;

   return ActionBase::kPassed;
}
*/

/*
ActionBase::ActionResult
CompareInOutChi2::endRun( Frame& iFrame )         // anal4 equiv.
{
   report( DEBUG, kFacilityString ) << "here in endRun()" << endl;

   return ActionBase::kPassed;
}
*/

//
// const member functions
//

//
// static member functions
//
