#if !defined(TRACKROOT_MANYTRSEEDTRACKQUALITYSTORAGEHELPER_H)
#define TRACKROOT_MANYTRSEEDTRACKQUALITYSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <TrackRoot>
// Module:      ManyTRSeedTrackQualityStorageHelper
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:04:02 EDT 1999
// $Id: ManyTRSeedTrackQualityStorageHelper.h,v 1.2 2000/10/28 20:34:51 ajm36 Exp $
//
// Revision history
//
// $Log: ManyTRSeedTrackQualityStorageHelper.h,v $
// Revision 1.2  2000/10/28 20:34:51  ajm36
// added numberLayers, numberMultipleHits, useForExtrapolation to seed qualities
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
#include "TrackRoot/TRSeedTrackQuality.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<TRSeedTrackQuality> ManyTRSeedTrackQualityStorageHelper ;

#endif /* TRACKROOT_MANYTRSEEDTRACKQUALITYSTORAGEHELPER_H */
