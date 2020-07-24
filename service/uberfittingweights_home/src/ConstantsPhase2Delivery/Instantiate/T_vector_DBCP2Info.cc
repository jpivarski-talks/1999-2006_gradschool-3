// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      T_vector_DBCP2Info
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Valentine Kouznetsov
// Created:     Mon Jan 20 09:53:17 EST 2003
// $Id: T_vector_DBCP2Info.cc,v 1.1.1.1 2003/01/28 21:49:55 vk Exp $
//
// Revision history
//
// $Log: T_vector_DBCP2Info.cc,v $
// Revision 1.1.1.1  2003/01/28 21:49:55  vk
// imported ConstantsPhase2Delivery sources
//
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

class DBCP2Info;

typedef DBCP2Info* _vector_contents_;
#include "STLUtility/instantiate_vector.h"
