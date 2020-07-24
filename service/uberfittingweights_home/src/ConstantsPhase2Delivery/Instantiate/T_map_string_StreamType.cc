// -*- C++ -*-
//
// Package:     <package>
// Module:      T_map_string_StreamType
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Valentine Kouznetsov
// Created:     Mon Jan 20 09:53:17 EST 2003
// $Id: T_map_string_StreamType.cc,v 1.1.1.1 2003/01/28 21:49:54 vk Exp $
//
// Revision history
//
// $Log: T_map_string_StreamType.cc,v $
// Revision 1.1.1.1  2003/01/28 21:49:54  vk
// imported ConstantsPhase2Delivery sources
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
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
#include <string>
//#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

typedef string _map_key_;
typedef StreamType _map_contents_;
typedef less< string > _map_compare_;

#include "STLUtility/instantiate_map.h"
