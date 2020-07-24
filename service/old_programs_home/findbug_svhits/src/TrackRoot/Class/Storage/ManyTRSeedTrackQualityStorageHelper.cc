// -*- C++ -*-
//
// Package:     <TestStorage>
// Module:      ManyTRSeedTrackQualityStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:12:52 EDT 1999
// $Id: ManyTRSeedTrackQualityStorageHelper.cc,v 1.2 2000/10/28 20:34:45 ajm36 Exp $
//
// Revision history
//
// $Log: ManyTRSeedTrackQualityStorageHelper.cc,v $
// Revision 1.2  2000/10/28 20:34:45  ajm36
// added numberLayers, numberMultipleHits, useForExtrapolation to seed qualities
//
// Revision 1.3  1999/10/18 18:13:03  cdj
// first submission
//
// Revision 1.2  1999/09/30 22:38:56  cdj
// now actually works
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */


#include "TrackRoot/TRSeedTrackQuality.h"

typedef TRSeedTrackQuality _many_element_ ;
#include "StorageManagement/instantiate_faptrtablehelper.h"
