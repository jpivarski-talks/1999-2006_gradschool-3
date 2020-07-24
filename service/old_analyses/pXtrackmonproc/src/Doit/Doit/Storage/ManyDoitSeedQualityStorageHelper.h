#if !defined(DOIT_MANYDOITSEEDQUALITYSTORAGEHELPER_H)
#define DOIT_MANYDOITSEEDQUALITYSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <Doit>
// Module:      ManyDoitSeedQualityStorageHelper
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Tue Aug 15 23:47:22 EDT 2000
// $Id: ManyDoitSeedQualityStorageHelper.h,v 1.1 2000/08/16 19:42:16 wsun Exp $
//
// Revision history
//
// $Log: ManyDoitSeedQualityStorageHelper.h,v $
// Revision 1.1  2000/08/16 19:42:16  wsun
// First submission.
//
//

// system include files

// user include files
#include "StorageManagement/SMFAPtrTableStorageHelper.h"
#include "Doit/DoitSeedQuality.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<DoitSeedQuality> ManyDoitSeedQualityStorageHelper ;

#endif /* DOIT_MANYDOITSEEDQUALITYSTORAGEHELPER_H */
