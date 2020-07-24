#if !defined(TRACKROOT_TRTRACKPIONFITQUALITY_H)
#define TRACKROOT_TRTRACKPIONFITQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackPionFitQuality
// 
// Description: typedef for TRTrackCandidateFitQuality< DBCandidate::kChargedPion >
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Nov 22 18:53:07 EST 1999
// $Id: TRTrackPionFitQuality.h,v 1.2 2000/04/25 17:12:42 marsh Exp $
//
// Revision history
//
// $Log: TRTrackPionFitQuality.h,v $
// Revision 1.2  2000/04/25 17:12:42  marsh
// Missing colon in template arg.
//
// Revision 1.1  2000/04/17 18:47:35  marsh
// Re-named candidate quality typedef's
//
// Revision 1.1  1999/11/23 00:01:31  wsun
// First submission.
//
//
//

// user include files
#include "CleoDB/DBCandidate.h"
#include "TrackRoot/TRTrackCandidateFitQuality.h"

typedef
TRTrackCandidateFitQuality< DBCandidate::kChargedPion > TRTrackPionFitQuality;

#endif /* TRACKROOT_TRTRACKPIONFITQUALITY_H */
