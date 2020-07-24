// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      T_Lattice_SeedTrack-SVRHit
// 
// Description: Instantiate Lattice<TRSeedTrack, CalibratedSVRphiHit>
//
// Author:      Merlin Meyer-Mitchell
// Created:     Thu Oct 1 1998
// $Id: T_Lattice_SeedTrack-SVRHit.cc,v 1.3 1999/05/23 21:10:20 meyerm Exp $
//
// Revision history
//
// $Log: T_Lattice_SeedTrack-SVRHit.cc,v $
// Revision 1.3  1999/05/23 21:10:20  meyerm
// simplify Instantiate directory-remove *_include.h files
//
// Revision 1.2  1999/05/11 00:18:48  wsun
// Added Lattice flags to control template instantiations.
//
// Revision 1.2  1999/02/22 21:51:30  meyerm
// changed SeedTrackSV*HitLattice to use DefaultLinkData
//
// Revision 1.1  1998/12/17 00:33:32  meyerm
// moving SeedTrackSV*Lattice stuff to TrackFinder
//
// Revision 1.1  1998/11/09 20:55:31  meyerm
// fixed to work with new Lattice and renamed some files
//
// Revision 1.2  1998/10/29 01:15:55  meyerm
// removed internal geometry code
//
// Revision 1.1  1998/10/08 21:18:35  meyerm
// added SeedTrackSVZHitLattice and renamed .SVRh. to .SVRH.
//
// Revision 1.4  1998/10/08 00:06:00  meyerm
// changed to work with Lattice update
//
// Revision 1.3  1998/10/02 18:22:36  meyerm
// updated all to work with SeedTrackSVRHitLattice.h
//
// Revision 1.2  1998/10/01 23:31:41  meyerm
// gave all Id and Log
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
#include "TrackFinder/SeedTrackSVRHitLattice.h"

//------------------------------------------------------------//
// Specify Lattice linkage here.                              //
// This Lattice links TRSeedTracks with CalibratedSVRphiHits. //
//------------------------------------------------------------//
typedef TRSeedTrack _left_lattice_type_;
typedef CalibratedSVRphiHit _right_lattice_type_;
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
