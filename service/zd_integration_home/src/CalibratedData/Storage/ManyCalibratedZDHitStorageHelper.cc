// -*- C++ -*-
//
// Package:     <TestStorage>
// Module:      ManyCalibratedZDHitStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:12:52 EDT 1999
// $Id: ManyCalibratedZDHitStorageHelper.cc,v 1.1 2002/09/27 20:30:21 bkh Exp $
//
// Revision history
//
// $Log: ManyCalibratedZDHitStorageHelper.cc,v $
// Revision 1.1  2002/09/27 20:30:21  bkh
// Add ZD
//
// Revision 1.1  2000/03/03 15:12:56  lyon
// Storage helpers for CalibratedData
//
// Revision 1.3  1999/10/18 18:13:03  cdj
// first submission
//
// Revision 1.2  1999/09/30 22:38:56  cdj
// now actually works
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */


#include "CalibratedData/CalibratedZDHit.h"

typedef CalibratedZDHit _many_element_ ;
#include "StorageManagement/instantiate_faptrtablehelper.h"
