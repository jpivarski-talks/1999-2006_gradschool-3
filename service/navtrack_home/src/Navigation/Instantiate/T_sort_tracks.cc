// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_sort_tracks
// 
// Description: instantiate algorithm sort
//
// Implementation:
//
// Author:      Brian K. Heltsley
// Created:     Thu Apr 29 13:54:24 EDT 1999
// $Id: T_sort_tracks.cc,v 1.1 2001/04/03 16:58:05 bkh Exp $
//
// Revision history
//
// $Log: T_sort_tracks.cc,v $
// Revision 1.1  2001/04/03 16:58:05  bkh
// Implement lists of nearby showers/tracks based on distance
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <algorithm>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files

#include "Navigation/NavTrack.h"
#include "Navigation/NavShower.h"

// STL classes
#include <vector>
#include <algorithm>

//
// constants, enums and typedefs
//

typedef STL_VECTOR( const NavTrack * ) Vec ;


typedef Vec::iterator                    _sort_iterator_      ;
typedef Vec::value_type                  _sort_value_type_    ;
typedef NavShower::CompareShowerToTrack  _sort_compare_class_ ;

#include "STLUtility/instantiate_sort_compare.h"

//template Vec* STD_PREFIX copy_backward( Vec* ,  
//					Vec* ,
//					Vec* );


