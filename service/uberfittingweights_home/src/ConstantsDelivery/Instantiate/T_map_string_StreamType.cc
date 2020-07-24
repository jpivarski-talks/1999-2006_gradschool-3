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
// Author:      Chris D Jones
// Created:     Wed Apr 19 15:07:24 EDT 2000
// $Id: T_map_string_StreamType.cc,v 1.1 2000/04/27 21:14:10 cdj Exp $
//
// Revision history
//
// $Log: T_map_string_StreamType.cc,v $
// Revision 1.1  2000/04/27 21:14:10  cdj
// now works on OSF using MICO ORB
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
