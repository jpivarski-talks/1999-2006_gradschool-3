// -*- C++ -*-
//
// Package:     <AlignmentHistogram>
// Module:      AlignmentHistogram
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Mon Jun  5 11:46:37 EDT 2000
// $Id: AlignmentHistogram.cc,v 1.2 2001/03/29 16:34:40 llh14 Exp $
//
// Revision history
//
// $Log: AlignmentHistogram.cc,v $
// Revision 1.2  2001/03/29 16:34:40  llh14
// Some Code cleanup
//
// Revision 1.1.1.1  2001/03/07 18:03:00  llh14
// imported AlignmentHistogram
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
#include "AlignmentHistogram/AlignmentHistogram.h"

#include "../src/ChisqForSiAlignment/ChisqForSiAlignment/HistogramResiduals.h"
#include "MinuitInterface/MinuitInterface.h"
#include "../src/OverlapHitResiduals/OverlapHitResiduals/HistogramOverlapResiduals.h"

#include "JobControl/JobControl.h"

#include <fstream.h>
#include <iostream.h>

#include "ToolBox/HistogramPackage.h"

//Debugging
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"
#include "FrameAccess/extract.h"
#include "DataHandler/Frame.h"
#include "ASiStorePro/ASiSensorInfo.h"
#include "ASiStorePro/ASiStore.h"

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
static const char* const kFacilityString = "AlignmentHistogram.AlignmentHistogram" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: AlignmentHistogram.cc,v 1.2 2001/03/29 16:34:40 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
AlignmentHistogram::AlignmentHistogram()
   : FrameIteratorModuleBase( "AlignmentHistogram", "This is my module" ),
     m_SVGlobalConstants(new DBSVAlignmentGlobal[1], 1),
     m_SVLayerConstants(new DBSVAlignmentLayer[4], 4),
     m_SVLadderConstants(new DBSVAlignmentLadder[61], 61),
     m_SVSensorConstants(new DBSVAlignmentSensor[447], 447),
     m_SVGlobalConstantsHolder(&m_SVGlobalConstants),
     m_SVLayerConstantsHolder(&m_SVLayerConstants),
     m_SVLadderConstantsHolder(&m_SVLadderConstants),
     m_SVSensorConstantsHolder(&m_SVSensorConstants),
     m_normRphi("RphiNorm", this, 0.000300),
     m_normZ("ZNorm", this, 0.00150),
     m_cutRphi("RphiCut", this, 0.000300),
     m_cutZ("ZCut", this, 0.02),
     m_TracksToHist("TracksToHist", this, 10000),
     m_GlobalInFile("GlobalInFile", this, "/a/solssb/nfs/solaris2/c3off/Offline/rel/current_interim/data/si3.svalignmentglobal"),
     m_LayerInFile("LayerInFile", this, "/a/solssb/nfs/solaris2/c3off/Offline/rel/current_interim/data/si3.svalignmentlayer"),
     m_LadderInFile("LadderInFile", this, "/a/solssb/nfs/solaris2/c3off/Offline/rel/current_interim/data/si3.svalignmentladder"),
     m_SensorInFile("SensorInFile", this, "/a/solssb/nfs/solaris2/c3off/Offline/rel/current_interim/data/si3.svalignmentsensor"),
     m_OverlapToggleOff("OverlapToggleOff", this, true)
   
{
   // reading global constants from the default location
   ifstream ifs1( m_GlobalInFile.value().c_str() );
   
   ifstream ifs2( m_LayerInFile.value().c_str() );
   
   ifstream ifs3( m_LadderInFile.value().c_str() );
   
   ifstream ifs4( m_SensorInFile.value().c_str() );

    if ( m_SVGlobalConstants.readFromStream(ifs1) && 
	 m_SVLayerConstants.readFromStream(ifs2)  &&
	 m_SVLadderConstants.readFromStream(ifs3) &&
	 m_SVSensorConstants.readFromStream(ifs4) )
       
    { 
       report(INFO, kFacilityString) 
	  << "\nDefault Input successful!" << endl; 
    }    

   //register the Proxy Factory created by the FIHolder
   registerProxyFactory( Stream::kSVAlignment,
			 m_SVGlobalConstantsHolder.makeFactory(),
			 UsageTag() );

   //register the Proxy Factory created by the FIHolder
   registerProxyFactory( Stream::kSVAlignment,
			 m_SVLayerConstantsHolder.makeFactory(),
			 UsageTag() );

   //register the Proxy Factory created by the FIHolder
   registerProxyFactory( Stream::kSVAlignment,
			 m_SVLadderConstantsHolder.makeFactory(),
			 UsageTag() );

   //register the Proxy Factory created by the FIHolder
   registerProxyFactory( Stream::kSVAlignment,
			 m_SVSensorConstantsHolder.makeFactory(),
			 UsageTag() );

   //You must set what streams you wish to iterate over
   //  that is, what events should the Frames be stopped on
   iterateOver( Stream::kEvent );

}

// AlignmentHistogram::AlignmentHistogram( const AlignmentHistogram& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

AlignmentHistogram::~AlignmentHistogram()
{
   report( DEBUG, kFacilityString)
      << "In destructor"
      << endl;
}

//
// assignment operators
//
// const AlignmentHistogram& AlignmentHistogram::operator=( const AlignmentHistogram& rhs )
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

//
// const member functions
//
void
AlignmentHistogram::iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd )
{
   cout 
      <<"\n" << m_GlobalInFile.value().c_str() 
      <<"\n" << m_LayerInFile.value().c_str() 
      <<"\n" << m_LadderInFile.value().c_str() 
      <<"\n" << m_SensorInFile.value().c_str() 
      << endl;

   // reading global constants from the designated location
   ifstream ifs1( m_GlobalInFile.value().c_str() );
   
   ifstream ifs2( m_LayerInFile.value().c_str() );
   
   ifstream ifs3( m_LadderInFile.value().c_str() );
   
   ifstream ifs4( m_SensorInFile.value().c_str() );

   if ( m_SVGlobalConstants.readFromStream(ifs1) && 
	m_SVLayerConstants.readFromStream(ifs2)  &&
	m_SVLadderConstants.readFromStream(ifs3) &&
	m_SVSensorConstants.readFromStream(ifs4) )
      
    { 
       report(INFO, kFacilityString) 
	  << "\nOverwrite Input successful!" << endl; 
    }    

   report( INFO, kFacilityString )
      << "starting iteration " << endl;

   // Let's get the histogram manager
   // Get an instance of JobControl
   JobControl* jc = JobControl::instance();

   // Get the histogram manager
   HIHistoManager* hm = jc->histogramManagerP();
   
   // Make a histogram directory
   HIHistDir* hdir = hm->mkdir("SiAlign");


   // Making some more Ntuples for debugging
   const CharList Label1[] = { "residual", "sensor", "curvature", "ladder",
			      "clamshell", "Z", "phi0", "cotTheta", 
			      "eventCtr"};

   const CharList Label2[] = { "resCur", "resNext",
			       "resDiff", "ZDiff", "ZLesser", "Phi0Diff",
			       "Phi0Lesser", "LadderLesser", "Layer", 
			       "EventCtr" };


// Choose the title of the histograms
    HINtuple* m_trackRphiHits = hdir->ntuple("Rphi Residuals",
   		  ChisqForSiAlignment::kNtupleSize, 50000, Label1);
    HINtuple* m_trackZHits = hdir->ntuple("Z Residuals", 
   		  ChisqForSiAlignment::kNtupleSize, 50000, Label1);

    HINtuple* m_overLapRphiHits = hdir->ntuple("Rphi Overlap Hits",
		  OverlapHitResiduals::kNtupleSize, 50000, Label2);
    HINtuple* m_overLapZHits = hdir->ntuple("Z Overlap Hits", 
		  OverlapHitResiduals::kNtupleSize, 50000, Label2);

   Count eventCtr =0;
   double chisq =0;

// Overlap Toggle
    if(m_OverlapToggleOff.value())

    {   
       // Make an instance of this class in order to make residual ntuple
       HistogramResiduals hist(iBegin, iEnd, 
			       m_SVGlobalConstants,
			       m_SVLayerConstants, m_SVLadderConstants,
			       HistogramResiduals::kBulk );
       
       hist.calcResiduals(iBegin, iEnd, eventCtr, chisq, 
			  m_TracksToHist.value(), 
			  m_normRphi.value(), m_normZ.value(), 
			  m_cutRphi.value(), m_cutZ.value(),
			  true, m_trackRphiHits, m_trackZHits);
    }

    else
    {
       // Make an instance of this class in order to make overlap residual
       // differences
       HistogramOverlapResiduals overLapHist(iBegin, iEnd, 
					     m_SVGlobalConstants,
					     m_SVLayerConstants, 
					     m_SVLadderConstants,
					     HistogramOverlapResiduals::kBulk );
       
       overLapHist.calcResiduals(iBegin, iEnd, eventCtr, chisq, 
				 m_TracksToHist.value(), 
				 m_normRphi.value(), m_normZ.value(), 
				 m_cutRphi.value(), m_cutZ.value(),
				 true, m_overLapRphiHits, m_overLapZHits);
    }

}

//
// static member functions
//
