// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      infoXandStream
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Jan 26 15:49:48 EST 2000
// $Id: infoXandStream.cc,v 1.2 2000/05/08 19:52:08 cdj Exp $
//
// Revision history
//
// $Log: infoXandStream.cc,v $
// Revision 1.2  2000/05/08 19:52:08  cdj
// now compiles on OSF with MICO
//
// Revision 1.1.1.1  2000/01/29 20:47:37  cdj
// imported RunStatisticsSourceModule source
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
#include "RunStatisticsSourceModule/infoXandStream.h"

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

static const char* const kFacilityString = "RunStatisticsSourceModule.infoXandStream" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: infoXandStream.cc,v 1.2 2000/05/08 19:52:08 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
const Stream::Type& 
infoX2StreamType( const InfoX::Type& iType )
{
   if( iType == InfoX::BeginRun ) { return Stream::kBeginRun; }
   if( iType == InfoX::StartRun ) { return Stream::kStartRun; }
   if( iType == InfoX::EndRun ) { return Stream::kEndRun; }
   return Stream::kNone;
}

InfoX::Type
stream2InfoXType( const Stream::Type& iType ) {
   if( iType == Stream::kBeginRun ) { return InfoX::BeginRun; }
   if( iType == Stream::kStartRun ) { return InfoX::StartRun; }
   if( iType == Stream::kEndRun ) { return InfoX::EndRun; }
   return InfoX::NoRS;
}
