// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_LatticeHelixFitSVZHit
// 
// Description: Instantiate Lattice
//
// Implementation:
//
// Author:      Michael Marsh
// Created:     Tue Dec 15 10:06 EST 1998
//
// $Id: T_LatticeHelixFitSVZHit.cc,v 1.3 1999/06/25 12:08:47 marsh Exp $
//
// $Log: T_LatticeHelixFitSVZHit.cc,v $
// Revision 1.3  1999/06/25 12:08:47  marsh
// Fixed include path for calibrated silicon hits.
//
// Revision 1.2  1999/06/09 19:08:15  marsh
// Added #define's to instantiate Lattice nodes.
//
// Revision 1.1  1999/04/23 21:35:46  marsh
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
#include "SiHits/CalibratedSVZHit.h"
#include "TrackFitter/TrackFitSVZHitLink.h"

typedef TRHelixFit _left_lattice_type_;
typedef CalibratedSVZHit _right_lattice_type_;
typedef TrackFitSVZHitLink _linkdata_type_;

#define INSTANTIATE_LATTICE_VECTOR_LINK
//#define INSTANTIATE_LATTICE_VECTOR_LEFTID
//#define INSTANTIATE_LATTICE_VECTOR_RIGHTID
#define INSTANTIATE_LATTICE_VECTOR_LEFTNODE
#define INSTANTIATE_LATTICE_VECTOR_RIGHTNODE
//#define INSTANTIATE_LATTICE_FIND

#include "Lattice/instantiate_Lattice.h"
