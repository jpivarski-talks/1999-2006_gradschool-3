// -*- C++ -*-
//
// Package:     <ZDAlignMod>
// Module:      ZDAlignMod
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 09:37:30 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "Experiment/report.h"
#include "ZDAlignMod/ZDAlignMod.h"
//#include "DataHandler/Stream.h"
//#include "FrameAccess/FAItem.h"
//#include "FrameAccess/FATable.h"
//#include "FrameAccess/extract.h"
//#include "DataHandler/Frame.h"

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
static const char* const kFacilityString = "ZDAlignMod.ZDAlignMod" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: fimodule.cc,v 1.2 2000/12/04 19:11:05 cdj Exp $";
static const char* const kTagString = "$Name: v06_08_00 $";

//
// static data member definitions
//

//
// constructors and destructor
//
ZDAlignMod::ZDAlignMod()
   : FrameIteratorModuleBase( "ZDAlignMod", "This is my module" )
   //,m_header(0), m_headerHolder(&m_header)
{
   //register the Proxy Factory created by the FIHolder
#if 0
   registerProxyFactory( Stream::kBeginRun,
			 m_headerHolder.makeFactory(),
			 UsageTag() );
#endif
   //You must set what streams you which to iterate over
   //  that is, what events should the Frames be stopped on
   iterateOver( Stream::kEvent );

}

// ZDAlignMod::ZDAlignMod( const ZDAlignMod& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ZDAlignMod::~ZDAlignMod()
{
}

//
// assignment operators
//
// const ZDAlignMod& ZDAlignMod::operator=( const ZDAlignMod& rhs )
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
ZDAlignMod::iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd )
{
   report( INFO, kFacilityString )
      << "starting iteration " << endl;
   
//Below is an example of an iteration loop.  We loop over the data 4
//  times and for each iteration we set the run number to be the same as
//  the number of times we have iterated.  The for each event we extract
//  the run number and print it out.
#if 0
   //you can loop over the set of Frames as long as you want
   for( unsigned int timesIterated = 1;
	timesIterated < 5;
	++timesIterated ) {

      report( INFO, kFacilityString )
	 << "start iteration" << timesIterated << endl;

      //Now reset the value we want to use in this iteration
      DBRunHeader header( timesIterated );
      m_headerHolder.hold( &header );

      
      //loop over all the Frames
      for( FIFrameIterator itFrame = iBegin;
	   itFrame != iEnd;
	   ++itFrame ) {

	 FAItem<DBRunHeader> spRunHeader;
	 extract( itFrame->record( Stream::kBeginRun ), spRunHeader );
	 
	 report( INFO, kFacilityString ) 
	    <<" RunHeader " << spRunHeader->number() << endl;
      }
   }
#endif
}

//
// static member functions
//
