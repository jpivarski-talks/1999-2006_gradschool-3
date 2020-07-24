#if !defined(NAVIGATION_MANYNAVSHOWERSTORAGEHELPER_H)
#define NAVIGATION_MANYNAVSHOWERSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      ManyNavShowerStorageHelper
// 
// Description: Storage of FATable<NavShower>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:04:02 EDT 1999
// $Id: ManyNavShowerStorageHelper.h,v 1.1 2003/09/02 01:53:13 cdj Exp $
//
// Revision history
//
// $Log: ManyNavShowerStorageHelper.h,v $
// Revision 1.1  2003/09/02 01:53:13  cdj
// can now store NavKs and NavShowers using templated storage helper
//
//

// system include files

// user include files
#include "StorageManagement/SMFAPtrTableStorageHelper.h"
#include "Navigation/NavShower.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<NavShower> ManyNavShowerStorageHelper ;

#endif /* NAVIGATION_MANYNAVSHOWERSTORAGEHELPER_H */
