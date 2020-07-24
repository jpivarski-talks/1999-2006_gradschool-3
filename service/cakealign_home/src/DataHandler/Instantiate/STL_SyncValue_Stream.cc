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
// $Id: STL_SyncValue_Stream.cc,v 1.3 1998/02/03 21:28:40 mkl Exp $
//
// Revision history
//
// $Log: STL_SyncValue_Stream.cc,v $
// Revision 1.3  1998/02/03 21:28:40  mkl
// switch over to STLUtility library
//
// Revision 1.2  1997/08/27 03:39:17  sjp
// Updated to new name for bug flags
//
// Revision 1.1  1997/08/06 15:39:46  sjp
// New instnatiation file
//


#include "Experiment/Experiment.h"
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataHandler/Stream.h"
#include "DataHandler/SyncValue.h"

// STL include files
#include <map>

typedef Stream::Type _map_key_ ;
typedef SyncValue _map_contents_ ;
typedef less< _map_key_ > _map_compare_ ;

#include "STLUtility/instantiate_map.h"
