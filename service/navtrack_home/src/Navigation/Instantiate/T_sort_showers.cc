// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_sort_showers
// 
// Description: instantiate algorithm sort
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Thu Apr 29 13:54:24 EDT 1999
// $Id: T_sort_showers.cc,v 1.1 2001/04/03 16:58:04 bkh Exp $
//
// Revision history
//
// $Log: T_sort_showers.cc,v $
// Revision 1.1  2001/04/03 16:58:04  bkh
// Implement lists of nearby showers/tracks based on distance
//
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <algorithm>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files

#include "Navigation/NavShower.h"

// STL classes
#include <vector>
#include <algorithm>

//
// constants, enums and typedefs
//

typedef STL_VECTOR( const NavShower * ) Vec ;


typedef Vec::iterator                        _sort_iterator_      ;
typedef Vec::value_type                      _sort_value_type_    ;
typedef NavShower::CompareShowersByDistance  _sort_compare_class_ ;

#include "STLUtility/instantiate_sort_compare.h"

//template Vec* STD_PREFIX copy_backward( Vec* ,  
//					Vec* ,
//					Vec* );




