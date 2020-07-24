// -*- C++ -*-
//
// Package:     <TestStorage>
// Module:      ManyNavKsStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed May 19 17:12:52 EDT 1999
// $Id: ManyNavKsStorageHelper.cc,v 1.1 2003/09/02 01:53:06 cdj Exp $
//
// Revision history
//
// $Log: ManyNavKsStorageHelper.cc,v $
// Revision 1.1  2003/09/02 01:53:06  cdj
// can now store NavKs and NavShowers using templated storage helper
//
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


#include "Navigation/NavKs.h"
#include "Navigation/Storage/NavFAConstPtrTableProxy.h"

template<>
SMProxyBase*
SMFAPtrTableStorageHelper<NavKs>::makeProxy() const {
   return new NavFAConstPtrTableProxy<NavKs>(
      const_cast<SMFAPtrTableStorageHelper<NavKs>*>(this) );
}

typedef NavKs _many_element_ ;
#include "StorageManagement/instantiate_faptrtablehelper.h"
