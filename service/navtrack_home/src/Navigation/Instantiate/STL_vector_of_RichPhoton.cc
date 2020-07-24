// -*- C++ -*-
//
// Package:     Navigation
// Module:      STL_vector_of_RichPhoton
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Tomasz Skwarnicki
// Created:     Sun Jul 23 05:54:55 EDT 2000
// $Id: STL_vector_of_RichPhoton.cc,v 1.1 2000/07/27 01:31:59 ts Exp $
//
// Revision history
//
// $Log: STL_vector_of_RichPhoton.cc,v $
// Revision 1.1  2000/07/27 01:31:59  ts
// NavRich added
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "RichPhotonsProd/RichPhoton.h"

// STL classes
#include <vector>

typedef const RichPhoton* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"
