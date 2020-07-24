// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRichServer
// 
// Description:
//
// Implementation:
//
// Author:      Tomasz Skwarnicki
// Created:     Fri Jul 22 17:29:00 EDT 2000
// $Id: NavRichServer_CRegionChamberCoord.cc,v 1.1 2000/07/27 01:31:35 ts Exp $
//
// Revision history
//
// $Log: NavRichServer_CRegionChamberCoord.cc,v $
// Revision 1.1  2000/07/27 01:31:35  ts
// NavRich added
//
//

#include "Experiment/Experiment.h"

// system include files
//#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavRichServer.h"

#include "DataHandler/Record.h"
#include "FrameAccess/extract.h"           

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>



//
// const member functions
//

#include "Navigation/NavRichServer_macro.h"
NAVRICHSER( RichCRegionChamberCoord , richCRegionChamberCoord , m_RichCRegionChamberCoord );



