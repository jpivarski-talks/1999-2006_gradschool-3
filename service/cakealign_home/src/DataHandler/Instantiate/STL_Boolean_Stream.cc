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
// $Id
//
// Revision history
//
// $Log: STL_Boolean_Stream.cc,v $
// Revision 1.2  1998/02/03 21:28:36  mkl
// switch over to STLUtility library
//
// Revision 1.1  1997/11/19 15:25:33  sjp
// New  file to instantate necessary map
//


#include "Experiment/Experiment.h"
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataHandler/Stream.h"

typedef Stream::Type  _map_key_ ;
typedef DABoolean _map_contents_ ;
typedef less< _map_key_ > _map_compare_ ;

#include "STLUtility/instantiate_map.h"
