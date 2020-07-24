// -*- C++ -*-
//
// Package:     KalmanProd
// Module:      T_KalmanProtonQualityProxy
// 
// Description: Instantiate Classes for KalmanCandidateQuality
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue May 11 15:26:39 EDT 1999
// $Id: T_KalmanProtonQualityProxy.cc,v 1.2 2000/08/16 20:18:29 wsun Exp $
//
// Revision history
//
// $Log: T_KalmanProtonQualityProxy.cc,v $
// Revision 1.2  2000/08/16 20:18:29  wsun
// Moved instantiate_many to KalmanFilter.
//
// Revision 1.1.1.1  1999/09/10 21:58:59  wsun
// Transfered from KalmanFilter
//
// Revision 1.3  1999/08/03 22:13:20  wsun
// Include .h files instead of typedefs.
//
// Revision 1.2  1999/07/15 23:01:21  cdj
// now compiles with CC optimization on
//
// Revision 1.1  1999/05/14 01:14:06  wsun
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
#include "KalmanFilter/KalmanProtonQuality.h"
#include "KalmanProd/KalmanCandidateQualityProxy.h"
#include "KalmanProd/Template/KalmanCandidateQualityProxy.cc"

#include <vector>
#include <map>

//typedef KalmanCandidateQuality< DBCandidate::kProton > KalmanProtonQuality;
template class KalmanCandidateQualityProxy< DBCandidate::kProton > ;

// #include "FrameAccess/instantiate_macros.h"
// INSTANTIATE_MANY( KalmanProtonQuality )
// #include "FrameAccess/instantiate_many.h"
