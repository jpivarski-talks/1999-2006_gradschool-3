// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      T_vector_TRHelixFit_pointer
// 
// Description: Instantiate vector< TRHelixFit* >
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Sat Jun 27 17:04:52 EDT 1998
// $Id: T_vector_TRHelixFit_pointer.cc,v 1.1.1.1 1998/07/20 21:16:10 marsh Exp $
//
// Revision history
//
// $Log: T_vector_TRHelixFit_pointer.cc,v $
// Revision 1.1.1.1  1998/07/20 21:16:10  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:28:01  marsh
// New library for track objects
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "TrackRoot/TRHelixFit.h"

// STL classes
#include <vector>

typedef TRHelixFit* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"
