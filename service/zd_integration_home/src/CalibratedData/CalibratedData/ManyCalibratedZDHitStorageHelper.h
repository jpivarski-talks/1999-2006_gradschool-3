#if !defined(CALIBRATEDDATA_MANYCALIBRATEDZDHITSTORAGEHELPER_H)
#define CALIBRATEDDATA_MANYCALIBRATEDZDHITSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      ManyCalibratedZDHitStorageHelper
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:04:02 EDT 1999
// $Id: ManyCalibratedZDHitStorageHelper.h,v 1.1 2002/09/27 20:30:07 bkh Exp $
//
// Revision history
//
// $Log: ManyCalibratedZDHitStorageHelper.h,v $
// Revision 1.1  2002/09/27 20:30:07  bkh
// Add ZD
//
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
#include "CalibratedData/CalibratedZDHit.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<CalibratedZDHit> ManyCalibratedZDHitStorageHelper ;

#endif /* CALIBRATEDDATA_MANYCALIBRATEDZDHITSTORAGEHELPER_H */
