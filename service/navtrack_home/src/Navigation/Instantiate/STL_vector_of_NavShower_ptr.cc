// -*- C++ -*-
//
// Package:     Navigation
// Module:      STL_vector_of_NavShower_ptr
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Brian Heltsley
// Created:     Tue Dec 29 14:10:30 EST 1998
// $Id: STL_vector_of_NavShower_ptr.cc,v 1.1 2001/04/03 16:58:03 bkh Exp $
//
// Revision history
//
// $Log: STL_vector_of_NavShower_ptr.cc,v $
// Revision 1.1  2001/04/03 16:58:03  bkh
// Implement lists of nearby showers/tracks based on distance
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Navigation/NavShower.h"

// STL classes
#include <vector>

typedef const NavShower* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"

