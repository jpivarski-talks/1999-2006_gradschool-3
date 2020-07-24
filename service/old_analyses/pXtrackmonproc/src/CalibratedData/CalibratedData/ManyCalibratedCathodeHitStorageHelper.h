#if !defined(CALIBRATEDDATA_MANYCALIBRATEDCATHODEHITSTORAGEHELPER_H)
#define CALIBRATEDDATA_MANYCALIBRATEDCATHODEHITSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      ManyCalibratedCathodeHitStorageHelper
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:04:02 EDT 1999
// $Id: ManyCalibratedCathodeHitStorageHelper.h,v 1.1 2000/03/03 15:14:04 lyon Exp $
//
// Revision history
//
// $Log: ManyCalibratedCathodeHitStorageHelper.h,v $
// Revision 1.1  2000/03/03 15:14:04  lyon
// Storage helpers
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
#include "CalibratedData/CalibratedCathodeHit.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<CalibratedCathodeHit> ManyCalibratedCathodeHitStorageHelper ;

#endif /* CALIBRATEDDATA_MANYCALIBRATEDCATHODEHITSTORAGEHELPER_H */
