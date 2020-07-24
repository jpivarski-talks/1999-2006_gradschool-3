// -*- C++ -*-
//
// Package:     DataHandler
// Module:      STL_SyncValue
// 
// Description: Instantiate STL classes for SyncValue
//
//
// Author:      Simon Patton
// Created:     Wed Sep 11 21:45:29 EDT 1996
// $Id: STL_SyncValue_string.cc,v 1.4 1998/04/01 15:44:23 mkl Exp $
//
// Revision history
//
// $Log: STL_SyncValue_string.cc,v $
// Revision 1.4  1998/04/01 15:44:23  mkl
// switched to C++Std standard include area: fwd_string/istream/ostream
//
// Revision 1.3  1998/02/03 21:28:40  mkl
// switch over to STLUtility library
//
// Revision 1.2  1997/08/27 03:39:18  sjp
// Updated to new name for bug flags
//
// Revision 1.1  1997/08/06 15:39:49  sjp
// New instnatiation file
//


#include "Experiment/Experiment.h"
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataHandler/SyncValue.h"

#if defined(OLD_CXX_STRING_CLASS_BUG)
#include "C++Std/fwd_string.h"
#include <String.h>
#else
#include <string>
#endif /* OLD_CXX_STRING_CLASS_BUG */
#include <map>

typedef string _map_key_ ;
typedef SyncValue _map_contents_ ;
typedef less< _map_key_ > _map_compare_ ;

#include "STLUtility/instantiate_map.h"
