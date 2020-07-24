#if !defined(KALMAN_KALMANEXITPROTONQUALITY_H)
#define KALMAN_KALMANEXITPROTONQUALITY_H
// -*- C++ -*-
//
// Package:     Kalman
// Module:      KalmanExitProtonQuality
// 
// Description: typedef for KalmanCandidateQuality< DBCandidate::kExitProton >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: KalmanExitProtonQuality.h,v 1.2 1999/07/25 16:33:31 mkl Exp $
//
// Revision history
//
// $Log: KalmanExitProtonQuality.h,v $
// Revision 1.2  1999/07/25 16:33:31  mkl
// merged in new Quality typedef files from mdc1 branch
//
// Revision 1.1.2.1  1999/07/25 16:28:38  mkl
// added remaining exit hypotheses files
//
// Revision 1.1.2.1  1999/07/24 20:54:07  mkl
// added typedef files for all the different track qualities
//
//

// user include files
#include "CleoDB/DBCandidate.h"
#include "KalmanFilter/KalmanCandidateQuality.h"

typedef
KalmanCandidateQuality< DBCandidate::kExitProton > KalmanExitProtonQuality;

#endif /* KALMAN_KALMANEXITPROTONQUALITY_H */
