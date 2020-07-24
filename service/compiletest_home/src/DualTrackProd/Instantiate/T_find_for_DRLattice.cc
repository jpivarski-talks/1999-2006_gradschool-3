// -*- C++ -*-
//
// Package:     Lattice
// Module:      T_find_for_Lattice
// 
// Description: Instantiate find<>() functions for Lattice
//
//
// Author:      Jon Thaler
// Created:     June 12, 2001
//

// #include "Experiment/Experiment.h"

// // system include files
// #if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// #include <algorithm>
// #endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// // STL classes
// #include <algorithm>

// //-------------------------------------------+
// // find is declared as:                      |
// // template<T1,T2> T1 find(T1,T1,const T2&); |
// //-------------------------------------------+
// typedef int _find_type_ ;
// #define _find_iterator_ int
// #include "STLUtility/instantiate_find.h"
// #undef STLUTILITY_INSTANTIATE_FIND_H
// #undef _find_iterator_
// #define _find_iterator_ const int
// #include "STLUtility/instantiate_find.h"
