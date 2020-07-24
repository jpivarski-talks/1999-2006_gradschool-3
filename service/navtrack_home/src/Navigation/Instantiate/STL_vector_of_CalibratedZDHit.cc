// -*- C++ -*-
//
// Package:     Navigation
// Module:      STL_vector_of_CalibratedZDHit
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jim Pivarski
// Created:     Wed Oct  8 13:20:02 EDT 2003
// $Id: STL_vector_of_CalibratedZDHit.cc,v 1.1 2003/10/16 18:43:07 mccann Exp $
//
// Revision history
//
// $Log: STL_vector_of_CalibratedZDHit.cc,v $
// Revision 1.1  2003/10/16 18:43:07  mccann
// adding ZD files
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "CalibratedData/CalibratedZDHit.h"

// STL classes
#include <vector>

typedef const CalibratedZDHit* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"