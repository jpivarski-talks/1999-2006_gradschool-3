// -*- C++ -*-
//
// Package:     TrackFitter
// Module:      T_LatticeProtonCathodeHit
// 
// Description: Instantiate Lattice
//
// Implementation:
//
// Author:      Werner Sun
// Created:     Mon Sep  6 14:45:44 EDT 1999
// $Id: T_LatticeProtonCathodeHit.cc,v 1.2 2000/06/27 23:35:23 wsun Exp $
//
// Revision history
//
// $Log: T_LatticeProtonCathodeHit.cc,v $
// Revision 1.2  2000/06/27 23:35:23  wsun
// Fixed bugs for OSF1 compilation.
//
// Revision 1.1  1999/09/10 20:24:19  wsun
// First submission.
//
//

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
#include "CalibratedData/CalibratedCathodeHit.h"
#include "TrackFitter/TrackFitCathodeHitLink.h"

typedef TRHelixCandidateFit< DBCandidate::kProton > _left_lattice_type_;
typedef CalibratedCathodeHit _right_lattice_type_;
typedef TrackFitCathodeHitLink _linkdata_type_;

#define INSTANTIATE_LATTICE_VECTOR_LINK
//#define INSTANTIATE_LATTICE_VECTOR_LEFTID
//#define INSTANTIATE_LATTICE_VECTOR_RIGHTID
#define INSTANTIATE_LATTICE_VECTOR_LEFTNODE
#define INSTANTIATE_LATTICE_VECTOR_RIGHTNODE
//#define INSTANTIATE_LATTICE_FIND

#include "Lattice/instantiate_Lattice.h"
