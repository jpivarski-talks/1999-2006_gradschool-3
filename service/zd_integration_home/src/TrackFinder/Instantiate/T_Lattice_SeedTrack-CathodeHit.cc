// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      T_Lattice_SeedTrack-CathodeHit
// 
// Description: Instantiate Lattice<TRSeedTrack, CalibratedCathodeHit>
//
// Author:      Werner Sun
// Created:     Sun Sep  5 22:21:04 EDT 1999
// $Id: T_Lattice_SeedTrack-CathodeHit.cc,v 1.2 2000/02/08 18:30:35 lyon Exp $
//
// Revision history
//
// $Log: T_Lattice_SeedTrack-CathodeHit.cc,v $
// Revision 1.2  2000/02/08 18:30:35  lyon
// Add Storage helper instantiations
//
// Revision 1.1  1999/09/10 20:21:34  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"

//----------------------//
// system include files //
//----------------------//
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif /* AMBIGUOUS_STRING_FUNCTIONS_BUG */

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <list>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

//--------------------//
// user include files //
//--------------------//
#include "TrackFinder/SeedTrackCathodeHitLattice.h"

//------------------------------------------------------------//
// Specify Lattice linkage here.                              //
// This Lattice links TRSeedTracks with CalibratedCathodeHits.//
//------------------------------------------------------------//
typedef TRSeedTrack _left_lattice_type_;
typedef CalibratedCathodeHit _right_lattice_type_;
typedef DefaultLinkData _linkdata_type_;

//-----------------------//
// Set INSTANTIATE flags //
//-----------------------//
#define INSTANTIATE_LATTICE_VECTOR_LINK
//#define INSTANTIATE_LATTICE_VECTOR_LEFTID
//#define INSTANTIATE_LATTICE_VECTOR_RIGHTID
#define INSTANTIATE_LATTICE_VECTOR_LEFTNODE
#define INSTANTIATE_LATTICE_VECTOR_RIGHTNODE

#include "Lattice/instantiate_Lattice.h"

