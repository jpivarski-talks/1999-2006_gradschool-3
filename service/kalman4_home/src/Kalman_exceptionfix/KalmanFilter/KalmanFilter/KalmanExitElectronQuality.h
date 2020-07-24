#if !defined(KALMAN_KALMANEXITELECTRONQUALITY_H)
#define KALMAN_KALMANEXITELECTRONQUALITY_H
// -*- C++ -*-
//
// Package:     Kalman
// Module:      KalmanExitElectronQuality
// 
// Description: typedef for KalmanCandidateQuality< DBCandidate::kExitElectron >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: KalmanExitElectronQuality.h,v 1.2 1999/07/25 16:33:28 mkl Exp $
//
// Revision history
//
// $Log: KalmanExitElectronQuality.h,v $
// Revision 1.2  1999/07/25 16:33:28  mkl
// merged in new Quality typedef files from mdc1 branch
//
// Revision 1.1.2.1  1999/07/25 16:28:35  mkl
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
KalmanCandidateQuality< DBCandidate::kExitElectron > KalmanExitElectronQuality;

#endif /* KALMAN_KALMANEXITELECTRONQUALITY_H */
