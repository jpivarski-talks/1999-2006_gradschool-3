// -*- C++ -*-
//
// Package:     <DualTrackDeltas>
// Module:      STL_save
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Lauren Hsu
// Created:     Wed May 17 12:21:35 EDT 2000
// $Id: T_STLsave.cc,v 1.1.1.1 2001/01/30 23:57:19 llh14 Exp $
//
// Revision history
//
// $Log: T_STLsave.cc,v $
// Revision 1.1.1.1  2001/01/30 23:57:19  llh14
// imported DualTrackHelices sources
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
//#include "package/STL_save.h"

#include "STLUtility/fwd_vector.h"
#include <vector>
typedef STL_VECTOR( double ) _container_type_ ;
#include "StorageManagement/instantiate_contents_builtin.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "package.STL_save" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: T_STLsave.cc,v 1.1.1.1 2001/01/30 23:57:19 llh14 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//

