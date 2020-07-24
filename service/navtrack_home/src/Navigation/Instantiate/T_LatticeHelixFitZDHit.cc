// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_LatticeHelixFitZDHit
// 
// Description: Instantiate Lattice
//
// Implementation:
//
// Author:      Jim Pivarski
// Created:     Wed Oct  8 13:21:28 EDT 2003
// $Id: T_LatticeHelixFitZDHit.cc,v 1.1 2003/10/16 18:43:15 mccann Exp $
//
// Revision history
//
// $Log: T_LatticeHelixFitZDHit.cc,v $
// Revision 1.1  2003/10/16 18:43:15  mccann
// adding ZD files
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"

#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <list>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackRoot/TRHelixFit.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "TrackFitter/TrackFitZDHitLink.h"

typedef TRHelixFit _left_lattice_type_;
typedef CalibratedZDHit _right_lattice_type_;
typedef TrackFitZDHitLink _linkdata_type_;

#define INSTANTIATE_LATTICE_VECTOR_LINK
//#define INSTANTIATE_LATTICE_VECTOR_LEFTID
//#define INSTANTIATE_LATTICE_VECTOR_RIGHTID
#define INSTANTIATE_LATTICE_VECTOR_LEFTNODE
#define INSTANTIATE_LATTICE_VECTOR_RIGHTNODE
//#define INSTANTIATE_LATTICE_FIND

#include "Lattice/instantiate_Lattice.h"
