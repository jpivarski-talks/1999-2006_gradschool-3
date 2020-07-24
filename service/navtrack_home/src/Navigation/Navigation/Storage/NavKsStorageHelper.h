#if !defined(NAVIGATION_NAVKSSTORAGEHELPER_H)
#define NAVIGATION_NAVKSSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavKsStorageHelper
// 
// Description: StorageHelper to store a NavKs
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: NavKsStorageHelper.h,v 1.1 2003/09/02 01:53:14 cdj Exp $
//
// Revision history
//
// $Log: NavKsStorageHelper.h,v $
// Revision 1.1  2003/09/02 01:53:14  cdj
// can now store NavKs and NavShowers using templated storage helper
//
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "Navigation/NavKs.h"
#include "FrameAccess/FATable.h"
#include "Navigation/Storage/NavTypeStorageHelper.h"

// forward declarations
typedef NavTypeStorageHelper<NavKs> NavKsStorageHelper;

#endif /* NAVIGATION_NAVKSSTORAGEHELPER_H */
