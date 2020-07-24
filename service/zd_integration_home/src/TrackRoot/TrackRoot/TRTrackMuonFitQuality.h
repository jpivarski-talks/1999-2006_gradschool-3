#if !defined(TRACKROOT_TRTRACKMUONFITQUALITY_H)
#define TRACKROOT_TRTRACKMUONFITQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackMuonFitQuality
// 
// Description: typedef for TRTrackCandidateFitQuality< DBCandidate::kMuon >
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Nov 22 18:53:07 EST 1999
// $Id: TRTrackMuonFitQuality.h,v 1.1 2000/04/17 18:47:35 marsh Exp $
//
// Revision history
//
// $Log: TRTrackMuonFitQuality.h,v $
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
TRTrackCandidateFitQuality< DBCandidate::kMuon > TRTrackMuonFitQuality;

#endif /* TRACKROOT_TRTRACKMUONFITQUALITY_H */
