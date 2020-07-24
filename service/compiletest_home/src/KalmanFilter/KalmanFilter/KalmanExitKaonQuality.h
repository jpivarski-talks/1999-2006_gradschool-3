#if !defined(KALMAN_KALMANEXITKAONQUALITY_H)
#define KALMAN_KALMANEXITKAONQUALITY_H
// -*- C++ -*-
//
// Package:     Kalman
// Module:      KalmanExitKaonQuality
// 
// Description: typedef for KalmanCandidateQuality< DBCandidate::kExitKaon >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: KalmanExitKaonQuality.h,v 1.2 1999/07/25 16:33:29 mkl Exp $
//
// Revision history
//
// $Log: KalmanExitKaonQuality.h,v $
// Revision 1.2  1999/07/25 16:33:29  mkl
// merged in new Quality typedef files from mdc1 branch
//
// Revision 1.1.2.1  1999/07/25 16:28:36  mkl
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
KalmanCandidateQuality< DBCandidate::kExitKaon > KalmanExitKaonQuality;

#endif /* KALMAN_KALMANEXITKAONQUALITY_H */
