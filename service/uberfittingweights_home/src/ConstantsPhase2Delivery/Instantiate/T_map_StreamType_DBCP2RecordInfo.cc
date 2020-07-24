// -*- C++ -*-
//
// Package:     <package>
// Module:      T_map_StreamType_DBCP2RecordInfo
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Valentine Kouznetsov
// Created:     Mon Jan 20 09:53:17 EST 2003
// $Id: T_map_StreamType_DBCP2RecordInfo.cc,v 1.1.1.1 2003/01/28 21:49:54 vk Exp $
//
// Revision history
//
// $Log: T_map_StreamType_DBCP2RecordInfo.cc,v $
// Revision 1.1.1.1  2003/01/28 21:49:54  vk
// imported ConstantsPhase2Delivery sources
//
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

class DBCP2RecordInfo;

typedef StreamType _map_key_;
typedef DBCP2RecordInfo* _map_contents_;
typedef less< StreamType > _map_compare_;

#include "STLUtility/instantiate_map.h"
