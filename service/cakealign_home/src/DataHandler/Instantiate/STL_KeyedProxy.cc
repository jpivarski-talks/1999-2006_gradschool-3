// -*- C++ -*-
//
// Package:     DataHanlder
// Module:      STL_KeyedProxy
// 
// Description: Instantiate STL classes for KeyedProxy
//
//
// Author:      Simon Patton
// Created:     Wed Sep 11 21:45:29 EDT 1996
// $Id: STL_KeyedProxy.cc,v 1.3 1998/02/03 21:28:36 mkl Exp $
//
// Revision history
//
// $Log: STL_KeyedProxy.cc,v $
// Revision 1.3  1998/02/03 21:28:36  mkl
// switch over to STLUtility library
//
// Revision 1.2  1997/08/27 03:39:11  sjp
// Updated to new name for bug flags
//
// Revision 1.1  1997/08/06 15:39:41  sjp
// New instnatiation file
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataHandler/KeyedProxy.h"

// STL include files
#include <vector>

typedef KeyedProxy _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"

//

#include "DataHandler/Stream.h"

typedef Stream::Type _map_key_ ;
typedef vector< KeyedProxy > _map_contents_ ;
typedef less< _map_key_ > _map_compare_ ;

#include "STLUtility/instantiate_map.h"
