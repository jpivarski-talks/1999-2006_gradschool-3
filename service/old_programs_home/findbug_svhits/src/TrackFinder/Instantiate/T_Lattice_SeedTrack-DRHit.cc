// -*- C++ -*-
//
// Package:     TrackFinder
// Module:      T_Lattice_SeedTrack-DRHit.cc
// 
// Description: Instantiate SeedTrack -> CalibratedDRHit Lattice
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Jon Thaler
// Created:     Fri May 8 17:00:32 EDT 1998
// $Id: T_Lattice_SeedTrack-DRHit.cc,v 1.6 2000/02/08 18:30:35 lyon Exp $
//
// Revision history
//
// $Log: T_Lattice_SeedTrack-DRHit.cc,v $
// Revision 1.6  2000/02/08 18:30:35  lyon
// Add Storage helper instantiations
//
// Revision 1.5  1999/05/23 21:10:20  meyerm
// simplify Instantiate directory-remove *_include.h files
//
//  Revision 1.2  1999/05/13 22:06:30  wsun
//  Removed find() instantiation -- done in ToolBox now.
//
//  Revision 1.1  1998/12/17 00:33:34  meyerm
//  moving SeedTrackSV*Lattice stuff to TrackFinder
//
//  Revision 1.1  1998/11/09 16:58:01  lyon
//  Updated for new Lattice specification
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


//--------------------+
// user include files |
//--------------------+
#include "TrackFinder/SeedDRHitLink.h"

//-----------------------------------------------------------+
// Specify Lattice linkage here.                             |
// This Lattice links CollatedRawDRHit with CalibratedDRHit. |
//-----------------------------------------------------------+
typedef TRSeedTrack      _left_lattice_type_;
typedef CalibratedDRHit  _right_lattice_type_;
typedef SeedDRHitLink    _linkdata_type_;

//-----------------------//
// Set INSTANTIATE flags //
//-----------------------//
#define INSTANTIATE_LATTICE_VECTOR_LINK
//#define INSTANTIATE_LATTICE_VECTOR_LEFTID
//#define INSTANTIATE_LATTICE_VECTOR_RIGHTID
#define INSTANTIATE_LATTICE_VECTOR_LEFTNODE
#define INSTANTIATE_LATTICE_VECTOR_RIGHTNODE

#include "Lattice/instantiate_Lattice.h"


