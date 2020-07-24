#if !defined(TRACKROOT_TRTRACKEXITELECTRONFITQUALITY_H)
#define TRACKROOT_TRTRACKEXITELECTRONFITQUALITY_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRTrackExitElectronFitQuality
// 
// Description: typedef for TRTrackCandidateFitQuality< DBCandidate::kExitElectron >
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Nov 22 18:53:07 EST 1999
// $Id: TRTrackExitElectronFitQuality.h,v 1.2 2000/04/25 17:12:40 marsh Exp $
//
// Revision history
//
// $Log: TRTrackExitElectronFitQuality.h,v $
// Revision 1.2  2000/04/25 17:12:40  marsh
// Missing colon in template arg.
//
// Revision 1.1  2000/04/17 18:47:33  marsh
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
TRTrackCandidateFitQuality< DBCandidate::kExitElectron >
   TRTrackExitElectronFitQuality;

#endif /* TRACKROOT_TRTRACKEXITELECTRONFITQUALITY_H */
