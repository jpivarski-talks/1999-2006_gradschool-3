// -*- C++ -*-
//
// Package:     DataHandler
// Module:      STL_Record
// 
// Description: Instantiate STL classes for Record
//
//
// Author:      Simon Patton
// Created:     Wed Sep 11 21:45:29 EDT 1996
// $Id: STL_Record.cc,v 1.8 1998/02/03 21:28:38 mkl Exp $
//
// Revision history
//
// $Log: STL_Record.cc,v $
// Revision 1.8  1998/02/03 21:28:38  mkl
// switch over to STLUtility library
//
// Revision 1.7  1997/08/27 03:39:14  sjp
// Updated to new name for bug flags
//
// Revision 1.6  1997/08/06 15:39:04  sjp
// Converted to use instantiation include files
//
// Revision 1.5  1997/07/28 16:33:12  sjp
// Fixed to work under cxx 5.6
//
// Revision 1.4  1997/07/09 18:00:12  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.3  1997/05/07 20:09:52  sjp
// Removed "wrong" code
//
// Revision 1.2  1997/05/07 14:04:32  sjp
// Added g++ code for EXPLICIT_STL flag
//
// Revision 1.1  1997/03/29 16:34:53  sjp
// New field to instantiated templated class
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataHandler/Stream.h"

// STL include files
#include <map>

class Record ;

typedef Stream::Type _map_key_ ;
typedef Record* _map_contents_ ;
typedef less< _map_key_ > _map_compare_ ;

#include "STLUtility/instantiate_map.h"
