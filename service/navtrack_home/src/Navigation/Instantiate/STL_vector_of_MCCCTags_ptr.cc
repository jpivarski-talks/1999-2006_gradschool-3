// -*- C++ -*-
//
// Package:     Navigation
// Module:      STL_vector_of_MCCCTags_ptr
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Brian Heltsley
// Created:     Tue Dec 29 14:10:30 EST 1998
// $Id: STL_vector_of_MCCCTags_ptr.cc,v 1.1 2002/11/07 17:42:14 bkh Exp $
//
// Revision history
//
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "MCCCTagger/MCCCTags.h"

// STL classes
#include <vector>

typedef MCCCTags* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"

