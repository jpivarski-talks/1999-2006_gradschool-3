// -*- C++ -*-
//
// Package:     ChisqFitter
// Module:      T_ChisqTrackFitter
// 
// Description: Instantiate Classes for ChisqTrackFitter
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 14:59:33 EDT 1999
// $Id: T_ChisqTrackFitter.cc,v 1.1.1.1 1999/06/16 02:46:48 wsun Exp $
//
// Revision history
//
// $Log: T_ChisqTrackFitter.cc,v $
// Revision 1.1.1.1  1999/06/16 02:46:48  wsun
// First submission.
//
//

#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ChisqFitter/ChisqTrackFitter.h"

#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE( ChisqTrackFitter )
//Use the following if 'ChisqTrackFitter' contains a ','
// also make sure there are no blank spaces within 'ChisqTrackFitter'
/* INSTANTIATE_ONE_1_COMMA( ChisqTrackFitter ) */
#include "FrameAccess/instantiate_one.h"

