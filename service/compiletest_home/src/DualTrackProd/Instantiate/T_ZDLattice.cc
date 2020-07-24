// -*- C++ -*-
//
// Package:     Lattice
// Module:      T_Lattice_template
// 
// Description: Template for Lattice instantiation file.
//
// Implementation:
//   Process with mklattice to generate a useable file.
//   Put the resulting files, T_Lattice.cc and T_find_for_Lattice.cc,
//   in the Instantiate directory.
//
// Author:      Jon Thaler
// Created:     June 12, 2001
//

//---------------+
// CLEO includes |
//---------------+
#include "Experiment/Experiment.h"
#include "Experiment/types.h"

//-------------------------------------------------------------+
// Turn off auto-instantiation of "" name string (for Solaris) |
//-------------------------------------------------------------+
#define HETEROCONTAINER_HCTYPETAGTEMPLATE_CC

//--------------------------+
// Deal with compiler bugs. |
//--------------------------+
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif

//-------------------------+
// User include files      |
//  * The left  data type. |
//  * The right data type. |
//  * the LinkData   type. |
//-------------------------+
#include "DualTrackProd/DualTrackFitHelix.h"
#include "CalibratedData/CalibratedZDHit.h"
#include "DualTrackProd/DualTrackZDHitLink.h"

//-------------------------------+
// Specify Lattice linkage here. |
//-------------------------------+
typedef DualTrackFitHelix   _left_lattice_type_;
typedef CalibratedZDHit  _right_lattice_type_;
typedef DualTrackZDHitLink   _linkdata_type_;

//--------------------------------------------------------------+
// Control which instantiations are actually done.              |
// This is needed due to difficulty automating template         |
// instantiation in all cases.  Most of the time, only RIGHTID  |
// is left undone (it's usually the same type as LEFTID).       |
//--------------------------------------------------------------+
#define INSTANTIATE_LATTICE_VECTOR_LINK
#define INSTANTIATE_LATTICE_VECTOR_LEFTID
//#define INSTANTIATE_LATTICE_VECTOR_RIGHTID
#define INSTANTIATE_LATTICE_VECTOR_LEFTNODE
#define INSTANTIATE_LATTICE_VECTOR_RIGHTNODE

#include "Lattice/instantiate_Lattice.h"
