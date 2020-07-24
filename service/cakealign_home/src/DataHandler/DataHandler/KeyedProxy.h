#if !defined(DATAHANDLER_KEYEDPROXY_H)
#define DATAHANDLER_KEYEDPROXY_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      KeyedProxy
// 
// Description: Explicit implementation of STL pair
//
// Usage:
//    <usage>
//
// Author:      Simon Patton
// Created:     Mon Jul 28 11:29:29 EDT 1997
// $Id: KeyedProxy.h,v 1.3 1999/11/12 18:46:57 cdj Exp $
//
// Revision history
//
// $Log: KeyedProxy.h,v $
// Revision 1.3  1999/11/12 18:46:57  cdj
// now uses DurableDataKey
//
// Revision 1.2  1998/09/25 19:31:45  cdj
// no longer use ProxyDict
//
// Revision 1.1  1997/08/06 15:02:11  sjp
// New file to associate a Key with a Proxy
//

// system include files

// user include files
#include "DataHandler/DurableDataKey.h"

// forward declarations
class ProxyBase ;

struct KeyedProxy
{
      //use a DurableDataKey so that it owns the memory for the 
      // usage and production strings
      DurableDataKey first;
      ProxyBase* second;
      KeyedProxy () :
	 second( 0 ) {} ;
      KeyedProxy(const KeyedProxy& p) :
	 first(p.first) ,
	 second(p.second) {}
      KeyedProxy (const DataKey& a, ProxyBase* b) :
	 first(a) ,
	 second(b) {}

};

// inline function definitions

#endif /* DATAHANDLER_KEYEDPROXY_H */
