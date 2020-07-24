// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      Inserter_TRHelixFit
// 
// Description: operator<<() for TRHelixFit
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 11:53:14 EDT 1998
// $Id: Inserter_TRHelixFit.cc,v 1.2 2002/08/16 19:36:55 cleo3 Exp $
//
// Revision history
//
// $Log: Inserter_TRHelixFit.cc,v $
// Revision 1.2  2002/08/16 19:36:55  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.1.1.1  1998/07/20 21:16:10  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:27:56  marsh
// New library for track objects
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "C++Std/iostream.h"

// user include files
//#include "Experiment/report.h"
#include "TrackRoot/TRHelixFit.h"

// STL classes

ostream&
operator<<( ostream& s , const TRHelixFit& helix )
{
   s << "TRHelixFit " << helix.identifier() << '\n';
   s << "( " << helix.curvature()
     << " , " << helix.phi0()
     << " , " << helix.d0()
     << " , " << helix.cotTheta()
     << " , " << helix.z0()
     << " )\n";
   s << helix.referencePoint() << '\n';
   s << helix.errorMatrix() << '\n';
   
   return s;
}
