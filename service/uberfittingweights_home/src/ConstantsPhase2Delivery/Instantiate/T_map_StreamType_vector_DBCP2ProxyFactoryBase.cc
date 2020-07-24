// -*- C++ -*-
//
// Package:     <package>
// Module:      T_map_StreamType_vector_DBCP2ProxyFactoryBase
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Valentine Kouznetsov
// Created:     Mon Jan 20 09:53:17 EST 2003
// $Id: T_map_StreamType_vector_DBCP2ProxyFactoryBase.cc,v 1.1.1.1 2003/01/28 21:49:54 vk Exp $
//
// Revision history
//
// $Log: T_map_StreamType_vector_DBCP2ProxyFactoryBase.cc,v $
// Revision 1.1.1.1  2003/01/28 21:49:54  vk
// imported ConstantsPhase2Delivery sources
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/StreamType.h"

// STL classes
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
class DBCP2ProxyFactoryBase;

typedef DBCP2ProxyFactoryBase* _vector_contents_;
#include "STLUtility/instantiate_vector.h"

#include "STLUtility/fwd_vector.h"

typedef StreamType _map_key_;
typedef STL_VECTOR( DBCP2ProxyFactoryBase* ) _map_contents_;
typedef less< StreamType > _map_compare_;

#include "STLUtility/instantiate_map.h"
