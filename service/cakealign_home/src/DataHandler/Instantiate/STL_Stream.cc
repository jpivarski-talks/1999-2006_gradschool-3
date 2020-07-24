// -*- C++ -*-
//
// Package:     DataHandler
// Module:      STL_Stream
// 
// Description: Instantiate STL classes for Stream
//
//
// Author:      Simon Patton
// Created:     Wed Sep 11 21:45:29 EDT 1996
// $Id: STL_Stream.cc,v 1.6 1998/02/03 21:28:39 mkl Exp $
//
// Revision history
//
// $Log: STL_Stream.cc,v $
// Revision 1.6  1998/02/03 21:28:39  mkl
// switch over to STLUtility library
//
// Revision 1.5  1997/08/27 03:39:15  sjp
// Updated to new name for bug flags
//
// Revision 1.4  1997/08/06 15:39:06  sjp
// Converted to use instantiation include files
//
// Revision 1.3  1997/07/28 16:33:14  sjp
// Fixed to work under cxx 5.6
//
// Revision 1.2  1997/07/09 18:00:13  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.1  1997/05/07 20:02:12  sjp
// New file to instantiate STL for Streams
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataHandler/Stream.h"

// STL include files
#include <set>

typedef Stream::Type _set_contents_ ;
typedef less< _set_contents_ > _set_compare_ ;

#include "STLUtility/instantiate_set.h"
