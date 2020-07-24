// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      T_LatticeExitElectronSVZHit
// 
// Description: Instantiate Lattice
//
// Implementation:
//
// Author:      Michael Marsh
// Created:     Tue Dec 15 10:06 EST 1998

#include "Experiment/Experiment.h"
#include "Experiment/types.h"

#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
//#include <list>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "TrackRoot/TRHelixCandidateFit.h"
//#include "CalibratedData/CalibratedSVZHit.h"
#include "TrackFitter/TrackFitSVZHitLink.h"

typedef TRHelixCandidateFit< DBCandidate::kExitElectron > _left_lattice_type_;
typedef CalibratedSVZHit _right_lattice_type_;
typedef TrackFitSVZHitLink _linkdata_type_;

#define INSTANTIATE_LATTICE_VECTOR_LINK
//#define INSTANTIATE_LATTICE_VECTOR_LEFTID
//#define INSTANTIATE_LATTICE_VECTOR_RIGHTID
#define INSTANTIATE_LATTICE_VECTOR_LEFTNODE
#define INSTANTIATE_LATTICE_VECTOR_RIGHTNODE
//#define INSTANTIATE_LATTICE_FIND

#include "Lattice/instantiate_Lattice.h"
