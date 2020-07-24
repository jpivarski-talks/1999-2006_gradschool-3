// -*- C++ -*-
//
// Package:     <TestStorage>
// Module:      ManyTRTrackStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Wed Nov 3 17:12:52 EDT 1999
// $Id: ManyTRTrackStorageHelper.cc,v 1.1 2000/01/05 18:46:13 cdj Exp $
//
// Revision history
//
// $Log: ManyTRTrackStorageHelper.cc,v $
// Revision 1.1  2000/01/05 18:46:13  cdj
// first submission
//
// Revision 1.1  1999/11/04 14:44:53  mkl
// added storage helper for TRTrack
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
#include <map>
//#include <set>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/TRTrack.h"

// STL includes
#include <vector>
#include <map>

typedef TRTrack _many_element_ ;
#include "StorageManagement/instantiate_faptrtablehelper.h"
