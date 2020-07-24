// -*- C++ -*-
//
// Package:     <DualTrackHelices>
// Module:      T_DualTrackHelices
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Wed Mar  1 16:02:58 EST 2000
// $Id: T_DualTrackHelicesStorage.cc,v 1.1.1.1 2001/01/30 23:57:19 llh14 Exp $
//
// Revision history
//
// $Log: T_DualTrackHelicesStorage.cc,v $
// Revision 1.1.1.1  2001/01/30 23:57:19  llh14
// imported DualTrackHelices sources
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
//#include "Experiment/report.h"

#include "DualTrackHelices/DualTrackHelices.h"
#include "DualTrackHelices/DualTrackHelicesStorageHelper.h"
SM_REGISTER_STORAGE(DualTrackHelicesStorageHelper)
