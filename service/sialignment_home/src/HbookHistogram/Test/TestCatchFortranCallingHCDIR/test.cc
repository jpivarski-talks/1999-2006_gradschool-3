// -*- C++ -*-
//
// Package:     <HbookHistogram>
// Module:      test
// 
// Description: Tests to see if we can catch the case where hcdir_ is called
//              directly
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Dec 22 13:15:47 EST 1999
// $Id: test.cc,v 1.1 2000/03/11 18:05:48 cdj Exp $
//
// Revision history
//
// $Log: test.cc,v $
// Revision 1.1  2000/03/11 18:05:48  cdj
// first submission
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
#include <iostream.h>

#include <functional>
#include <utility>

// user include files
#include "Experiment/report.h"
#include "HistogramInterface/HistogramPackage.h"
#include "HbookHistogram/HbookManager.h"
#include "HbookHistogram/hbook_functions.h"

//#include "CLHEP/Geometry/Vector3D.h"


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

static const char* const kFacilityString = "HbookHistogram.test" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: test.cc,v 1.1 2000/03/11 18:05:48 cdj Exp $";
static const char* const kTagString = "$Name:  $";



int main()
{
   MessageLog::LogStream logger( DEBUG, cout );
   MessageLog::Tie(".", logger );

   HbookManager manager("test.hbook", 17, 1024);

   const unsigned int kNChannels = 10;
   const float kMin = 0.;
   const float kMax = 10.;

   HIHistDir* firstDir = manager.mkdir("first");
   HIHistDir* secondDir = manager.mkdir("second");

   HIHist1D* increase1D = firstDir->histogram( 1,
					       "increase1D",
					       kNChannels,
					       kMin,
					       kMax );

   HIHist1D* decrease1D = secondDir->histogram( 2,
						"decrease1D",
						kNChannels,
						kMin,
						kMax );
   

   for( unsigned int repetitions = 0;
	repetitions < 3001;
	++repetitions) {
  
      //fill histograms
      float x = kMin + (kMax-kMin)/kNChannels/2.0;
      
      
      for( unsigned int bin = 1;
	   bin < kNChannels;
	   ++bin ) {
	 
	 
	 for( unsigned int nEntries = 1;
	      nEntries < bin;
	      ++nEntries ) {
	    increase1D->fill( x );
	    //decrease1D->fill( kMax - x );
	 }
	 
	 x += (kMax-kMin)/kNChannels;
      }
   }

   //

   //now another 'Processor'
   {
   for( unsigned int repetitions = 0;
	repetitions < 3001;
	++repetitions) {
  
      //fill histograms
      float x = kMin + (kMax-kMin)/kNChannels/2.0;
      
      
      for( unsigned int bin = 1;
	   bin < kNChannels;
	   ++bin ) {
	 
	 
	 for( unsigned int nEntries = 1;
	      nEntries < bin;
	      ++nEntries ) {
	    //increase1D->fill( x );
	    decrease1D->fill( kMax - x );
	    //this line should cause the program to abort
	    hcdir_( "//PAWC", " ", 6 , 1 );

	 }
	 
	 x += (kMax-kMin)/kNChannels;
      }
   }
   }

   return 0;
}

