#if !defined(DUALTRACKHELICES_MANYDUALTRACKDELTASSTORAGEHELPER_H)
#define DUALTRACKHELICES_MANYDUALTRACKHELICESSTORAGEHELPER_H
// -*- C++ -*-
//
// Package:     <DualTrackHelices>
// Module:      ManyDualTrackHelicesStorageHelper
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:04:02 EDT 1999
// $Id: ManyDualTrackHelicesStorageHelper.h,v 1.1.1.1 2001/01/30 23:57:19 llh14 Exp $
//
// Revision history
//
// $Log: ManyDualTrackHelicesStorageHelper.h,v $
// Revision 1.1.1.1  2001/01/30 23:57:19  llh14
// imported DualTrackHelices sources
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
#include "DualTrackHelices/DualTrackHelices.h"

// forward declarations

typedef SMFAPtrTableStorageHelper<DualTrackHelices> ManyDualTrackHelicesStorageHelper ;

#endif /* DUALTRACKHELICES_MANYDUALTRACKHELICESSTORAGEHELPER_H */
