// -*- C++ -*-
//
// Package:     Navigation
// Module:      T_FAItem_MCParticle
// 
// Description: Instantiate FAItem<MCParticle>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Tomasz Skwarnicki
// Created:     Sun Jul 23 22:21:56 EDT 2000
// $Id: T_FAItem_MCParticle.cc,v 1.1 2002/11/07 17:42:15 bkh Exp $
//
// Revision history
//
// $Log: T_FAItem_MCParticle.cc,v $
// Revision 1.1  2002/11/07 17:42:15  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.1  2000/07/27 01:32:00  ts
// NavRich added
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files

#include "MCInfo/MCDecayTree/MCParticle.h"

#include "FrameAccess/Template/FAItem.cc" 

template class FAItem<MCParticle> ;
