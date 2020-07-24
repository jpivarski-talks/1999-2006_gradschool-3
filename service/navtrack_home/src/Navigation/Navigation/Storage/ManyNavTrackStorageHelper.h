#if !defined(NAVIGATION_MANYNAVTRACKSTORAGEHELPER_H)
#define NAVIGATION_MANYNAVTRACKSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      ManyNavTrackStorageHelper
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:04:02 EDT 1999
// $Id: ManyNavTrackStorageHelper.h,v 1.1 2003/08/31 23:25:35 cdj Exp $
//
// Revision history
//
// $Log: ManyNavTrackStorageHelper.h,v $
// Revision 1.1  2003/08/31 23:25:35  cdj
// added the ability to store NavTracks
//
// Revision 1.2  1999/09/30 22:38:57  cdj
// now actually works
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
//

// system include files

// user include files
#include "StorageManagement/SMFAPtrTableStorageHelper.h"
#include "Navigation/NavTrack.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<NavTrack> ManyNavTrackStorageHelper ;

#endif /* NAVIGATION_MANYNAVTRACKSTORAGEHELPER_H */
