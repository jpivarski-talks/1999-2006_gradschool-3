// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_LatticeHelixFitDRHit
// 
// Description: Instantiate Lattice
//
// Implementation:
//
// Author:      Michael Marsh
// Created:     Tue Dec 15 10:06 EST 1998
//
// $Id: T_LatticeHelixFitDRHit.cc,v 1.2 1999/06/09 19:08:13 marsh Exp $
//
// $Log: T_LatticeHelixFitDRHit.cc,v $
// Revision 1.2  1999/06/09 19:08:13  marsh
// Added #define's to instantiate Lattice nodes.
//
// Revision 1.1  1999/04/23 21:35:44  marsh
// Changed NavTrack to deliver links, not link data.
//
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
#include "CalibratedData/CalibratedDRHit.h"
#include "TrackFitter/TrackFitDRHitLink.h"

typedef TRHelixFit _left_lattice_type_;
typedef CalibratedDRHit _right_lattice_type_;
typedef TrackFitDRHitLink _linkdata_type_;

#define INSTANTIATE_LATTICE_VECTOR_LINK
//#define INSTANTIATE_LATTICE_VECTOR_LEFTID
//#define INSTANTIATE_LATTICE_VECTOR_RIGHTID
#define INSTANTIATE_LATTICE_VECTOR_LEFTNODE
#define INSTANTIATE_LATTICE_VECTOR_RIGHTNODE
//#define INSTANTIATE_LATTICE_FIND

#include "Lattice/instantiate_Lattice.h"
