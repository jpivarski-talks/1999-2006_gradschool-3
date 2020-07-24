// -*- C++ -*-
//
// Package:     McCannProd
// Module:      T_McCannMuonQualityProxy
// 
// Description: Instantiate Classes for McCannCandidateQuality
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_McCannMuonQualityProxy.cc,v 1.2 2000/08/16 20:18:28 wsun Exp $
//
// Revision history
//
// $Log: T_McCannMuonQualityProxy.cc,v $
// Revision 1.2  2000/08/16 20:18:28  wsun
// Moved instantiate_many to McCannFilter.
//
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from McCannFilter
//
// Revision 1.3  1999/08/03 22:13:19  wsun
// Include .h files instead of typedefs.
//
// Revision 1.2  1999/07/15 23:01:20  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:14:05  wsun
// First submission of producer.
//
//
//
#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include "FrameAccess/FAPtrTable.h"
#include "McCannFilter/McCannMuonQuality.h"
#include "McCannProd/McCannCandidateQualityProxy.h"
#include "McCannProd/Template/McCannCandidateQualityProxy.cc"

#include <vector>
#include <map>

//typedef McCannCandidateQuality< DBCandidate::kMuon > McCannMuonQuality;

template class McCannCandidateQualityProxy< DBCandidate::kMuon > ;

// #include "FrameAccess/instantiate_macros.h"
// INSTANTIATE_MANY( McCannMuonQuality )
// #include "FrameAccess/instantiate_many.h"
