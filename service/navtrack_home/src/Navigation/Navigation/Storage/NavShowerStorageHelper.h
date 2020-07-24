#if !defined(NAVIGATION_NAVSHOWERSTORAGEHELPER_H)
#define NAVIGATION_NAVSHOWERSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavShowerStorageHelper
// 
// Description: StorageHelper to store a NavShower
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: NavShowerStorageHelper.h,v 1.1 2003/09/02 01:53:14 cdj Exp $
//
// Revision history
//
// $Log: NavShowerStorageHelper.h,v $
// Revision 1.1  2003/09/02 01:53:14  cdj
// can now store NavKs and NavShowers using templated storage helper
//
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "Navigation/NavShower.h"
#include "FrameAccess/FATable.h"
#include "Navigation/Storage/NavTypeStorageHelper.h"

// forward declarations
typedef NavTypeStorageHelper<NavShower> NavShowerStorageHelper;


#endif /* NAVIGATION_NAVSHOWERSTORAGEHELPER_H */
