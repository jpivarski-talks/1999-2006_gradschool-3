#if !defined(TRACKROOT_MANYTRTRACKSTORAGEHELPER_H)
#define TRACKROOT_MANYTRTRACKSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      ManyTRTrackStorageHelper
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Wed Nov 3 17:04:02 EDT 1999
// $Id: ManyTRTrackStorageHelper.h,v 1.1 2000/01/05 18:51:06 cdj Exp $
//
// Revision history
//
// $Log: ManyTRTrackStorageHelper.h,v $
// Revision 1.1  2000/01/05 18:51:06  cdj
// moved to new directory
//
// Revision 1.1  1999/11/04 14:44:58  mkl
// added storage helper for TRTrack
//
//

// system include files

// user include files
#include "StorageManagement/SMFAPtrTableStorageHelper.h"
#include "TrackRoot/TRTrack.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<TRTrack> ManyTRTrackStorageHelper ;

#endif /* TRACKROOT_MANYTRTRACKSTORAGEHELPER_H */
