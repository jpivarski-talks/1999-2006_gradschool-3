// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      T_vector_voidPtr
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Thu Oct 14 17:59:32 EDT 1999
// $Id: T_vector_voidPtr.cc,v 1.1 2001/07/23 20:02:44 cdj Exp $
//
// Revision history
//
// $Log: T_vector_voidPtr.cc,v $
// Revision 1.1  2001/07/23 20:02:44  cdj
// added instantiation of vector<void*> so stand alone code will work
//
// Revision 1.1  1999/10/18 18:29:14  cdj
// first submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

typedef void* _vector_contents_;
#include "STLUtility/instantiate_vector.h"

