// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      T_vector_DBConstantsInfo
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Sun Apr 23 12:46:35 EDT 2000
// $Id: T_vector_DBConstantsInfo.cc,v 1.1 2000/04/27 21:14:11 cdj Exp $
//
// Revision history
//
// $Log: T_vector_DBConstantsInfo.cc,v $
// Revision 1.1  2000/04/27 21:14:11  cdj
// now works on OSF using MICO ORB
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

class DBConstantsInfo;

typedef DBConstantsInfo* _vector_contents_;
#include "STLUtility/instantiate_vector.h"
