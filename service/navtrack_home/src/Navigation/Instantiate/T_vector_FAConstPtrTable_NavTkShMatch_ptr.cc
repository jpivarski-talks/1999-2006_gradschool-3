// -*- C++ -*-
//
// Package:     <package>
// Module:      T_vector_FAConstPtrTable_NavTkShMatch_ptr
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Sun Aug 11 13:17:51 EDT 2002
// $Id: T_vector_FAConstPtrTable_NavTkShMatch_ptr.cc,v 1.1 2002/08/11 19:00:16 cdj Exp $
//
// Revision history
//
// $Log: T_vector_FAConstPtrTable_NavTkShMatch_ptr.cc,v $
// Revision 1.1  2002/08/11 19:00:16  cdj
// added instantiations needed for OSF
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
//#include "Experiment/report.h"
#include "FrameAccess/FAConstPtrTable.h"
#include "Navigation/NavTkShMatch.h"

//
// constants, enums and typedefs
//
typedef FAConstPtrTable<NavTkShMatch>* _vector_contents_;
#include "STLUtility/instantiate_vector.h"
