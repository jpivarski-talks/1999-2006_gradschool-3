// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      T_multimap_StreamType_string
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Apr 26 13:13:00 EDT 2000
// $Id: T_multimap_StreamType_string.cc,v 1.1 2000/04/27 21:14:10 cdj Exp $
//
// Revision history
//
// $Log: T_multimap_StreamType_string.cc,v $
// Revision 1.1  2000/04/27 21:14:10  cdj
// now works on OSF using MICO ORB
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/StreamType.h"

// STL classes
#include <string>
#include <map>

//
// constants, enums and typedefs
//

typedef StreamType _multimap_key_;
typedef string _multimap_contents_;
typedef less< StreamType > _multimap_compare_;

#include "STLUtility/instantiate_multimap.h"
