#if !defined(NAVIGATION_NAVTRACKSTORAGEHELPER_H)
#define NAVIGATION_NAVTRACKSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackStorageHelper
// 
// Description: StorageHelper to store a NavTrack
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri May 14 16:55:38 EDT 1999
// $Id: NavTrackStorageHelper.h,v 1.2 2003/09/02 01:50:20 cdj Exp $
//
// Revision history
//
// $Log: NavTrackStorageHelper.h,v $
// Revision 1.2  2003/09/02 01:50:20  cdj
// now used templated version
//
// Revision 1.1  2003/08/31 23:25:36  cdj
// added the ability to store NavTracks
//
// Revision 1.1  1999/10/18 18:13:05  cdj
// first submission
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
//

// system include files

// user include files
#include "StorageManagement/SMStorageHelper.h"
#include "Navigation/NavTrack.h"
#include "FrameAccess/FATable.h"
#include "Navigation/Storage/NavTypeStorageHelper.h"

// forward declarations
typedef NavTypeStorageHelper<NavTrack> NavTrackStorageHelper;

#endif /* NAVIGATION_NAVTRACKSTORAGEHELPER_H */
