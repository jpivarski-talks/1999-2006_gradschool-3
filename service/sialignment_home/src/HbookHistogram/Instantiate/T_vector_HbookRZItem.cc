// -*- C++ -*-
//
// Package:     <HbookHistogram>
// Module:      T_vector_HbookRZItem
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Mon Jan 31 21:21:53 EST 2000
// $Id: T_vector_HbookRZItem.cc,v 1.1 2000/02/01 02:47:31 cdj Exp $
//
// Revision history
//
// $Log: T_vector_HbookRZItem.cc,v $
// Revision 1.1  2000/02/01 02:47:31  cdj
// first submission
//

#include "Experiment/Experiment.h"

// system include files
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
//#include "Experiment/report.h"
#include "HbookHistogram/HbookRZItem.h"

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
// constants, enums and typedefs
//

typedef HbookRZItem* _vector_contents_;
#include "STLUtility/instantiate_vector.h"
