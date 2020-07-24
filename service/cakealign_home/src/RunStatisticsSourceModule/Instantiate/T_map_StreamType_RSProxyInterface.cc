// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      T_map_StreamType_RSProxyInterface
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Mon May  8 15:23:57 EDT 2000
// $Id: T_map_StreamType_RSProxyInterface.cc,v 1.1 2000/05/08 19:52:11 cdj Exp $
//
// Revision history
//
// $Log: T_map_StreamType_RSProxyInterface.cc,v $
// Revision 1.1  2000/05/08 19:52:11  cdj
// now compiles on OSF with MICO
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
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
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

class RSProxyInterface;
typedef StreamType _map_key_;
typedef RSProxyInterface* _map_contents_;
typedef less<StreamType> _map_compare_;

#include "STLUtility/instantiate_map.h"
