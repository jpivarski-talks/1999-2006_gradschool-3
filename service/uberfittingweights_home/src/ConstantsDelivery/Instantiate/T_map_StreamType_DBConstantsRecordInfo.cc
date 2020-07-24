// -*- C++ -*-
//
// Package:     <package>
// Module:      T_map_StreamType_DBConstantsRecordInfo
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Apr 19 13:58:50 EDT 2000
// $Id: T_map_StreamType_DBConstantsRecordInfo.cc,v 1.1 2000/04/27 21:14:09 cdj Exp $
//
// Revision history
//
// $Log: T_map_StreamType_DBConstantsRecordInfo.cc,v $
// Revision 1.1  2000/04/27 21:14:09  cdj
// now works on OSF using MICO ORB
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/StreamType.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

class DBConstantsRecordInfo;

typedef StreamType _map_key_;
typedef DBConstantsRecordInfo* _map_contents_;
typedef less< StreamType > _map_compare_;

#include "STLUtility/instantiate_map.h"
