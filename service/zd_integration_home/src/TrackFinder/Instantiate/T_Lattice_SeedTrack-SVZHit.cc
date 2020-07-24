// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      T_Lattice_SeedTrack-SVZHit
// 
// Description: Instantiate Lattice<TRSeedTrack, CalibratedSVZHit>
//
// Author:      Merlin Meyer-Mitchell
// Created:     Thu Oct 8 1998
// $Id: T_Lattice_SeedTrack-SVZHit.cc,v 1.3 1999/05/23 21:10:21 meyerm Exp $
//
// Revision history
//
// $Log: T_Lattice_SeedTrack-SVZHit.cc,v $
// Revision 1.3  1999/05/23 21:10:21  meyerm
// simplify Instantiate directory-remove *_include.h files
//
// Revision 1.2  1999/05/11 00:18:49  wsun
// Added Lattice flags to control template instantiations.
//
// Revision 1.2  1999/02/22 21:51:31  meyerm
// changed SeedTrackSV*HitLattice to use DefaultLinkData
//
// Revision 1.1  1998/12/17 00:33:33  meyerm
// moving SeedTrackSV*Lattice stuff to TrackFinder
//
// Revision 1.1  1998/11/09 20:55:33  meyerm
// fixed to work with new Lattice and renamed some files
//
// Revision 1.2  1998/10/29 01:15:56  meyerm
// removed internal geometry code
//
// Revision 1.1  1998/10/08 21:18:38  meyerm
// added SeedTrackSVZHitLattice and renamed .SVRh. to .SVRH.
//

//--------------------------//
// Experiment include files //
//--------------------------//
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
#include "TrackFinder/SeedTrackSVZHitLattice.h"

//------------------------------------------------------------//
// Specify Lattice linkage here.                              //
// This Lattice links TRSeedTracks with CalibratedSVZHits.    //
//------------------------------------------------------------//
typedef TRSeedTrack _left_lattice_type_;
typedef CalibratedSVZHit _right_lattice_type_;
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
