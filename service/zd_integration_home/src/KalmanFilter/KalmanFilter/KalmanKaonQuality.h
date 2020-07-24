#if !defined(KALMAN_KALMANKAONQUALITY_H)
#define KALMAN_KALMANKAONQUALITY_H
// -*- C++ -*-
//
// Package:     Kalman
// Module:      KalmanKaonQuality
// 
// Description: typedef for KalmanCandidateQuality< DBCandidate::kKaon >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: KalmanKaonQuality.h,v 1.2 1999/07/25 16:33:32 mkl Exp $
//
// Revision history
//
// $Log: KalmanKaonQuality.h,v $
// Revision 1.2  1999/07/25 16:33:32  mkl
// merged in new Quality typedef files from mdc1 branch
//
// Revision 1.1.2.1  1999/07/24 20:54:09  mkl
// added typedef files for all the different track qualities
//
//

// user include files
#include "CleoDB/DBCandidate.h"
#include "KalmanFilter/KalmanCandidateQuality.h"

typedef
KalmanCandidateQuality< DBCandidate::kChargedKaon > KalmanKaonQuality;

#endif /* KALMAN_KALMANKAONQUALITY_H */
