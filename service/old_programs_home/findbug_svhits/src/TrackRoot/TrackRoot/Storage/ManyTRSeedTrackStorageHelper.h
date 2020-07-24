#if !defined(TRACKROOT_MANYTRSEEDTRACKSTORAGEHELPER_H)
#define TRACKROOT_MANYTRSEEDTRACKSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      ManyTRSeedTrackStorageHelper
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:04:02 EDT 1999
// $Id: ManyTRSeedTrackStorageHelper.h,v 1.1 2000/01/05 18:51:23 cdj Exp $
//
// Revision history
//
// $Log: ManyTRSeedTrackStorageHelper.h,v $
// Revision 1.1  2000/01/05 18:51:23  cdj
// first submission
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
#include "TrackRoot/TRSeedTrack.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<TRSeedTrack> ManyTRSeedTrackStorageHelper ;

#endif /* TRACKROOT_MANYTRSEEDTRACKSTORAGEHELPER_H */
