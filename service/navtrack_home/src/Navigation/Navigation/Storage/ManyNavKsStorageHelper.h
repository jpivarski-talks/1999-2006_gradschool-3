#if !defined(NAVIGATION_MANYNAVKSSTORAGEHELPER_H)
#define NAVIGATION_MANYNAVKSSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      ManyNavKsStorageHelper
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:04:02 EDT 1999
// $Id: ManyNavKsStorageHelper.h,v 1.1 2003/09/02 01:53:13 cdj Exp $
//
// Revision history
//
// $Log: ManyNavKsStorageHelper.h,v $
// Revision 1.1  2003/09/02 01:53:13  cdj
// can now store NavKs and NavShowers using templated storage helper
//
//

// system include files

// user include files
#include "StorageManagement/SMFAPtrTableStorageHelper.h"
#include "Navigation/NavKs.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<NavKs> ManyNavKsStorageHelper ;

#endif /* NAVIGATION_MANYNAVKSSTORAGEHELPER_H */
