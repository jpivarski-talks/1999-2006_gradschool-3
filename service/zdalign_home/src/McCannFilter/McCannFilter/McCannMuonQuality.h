#if !defined(KALMAN_KALMANMUONQUALITY_H)
#define KALMAN_KALMANMUONQUALITY_H
// -*- C++ -*-
//
// Package:     McCann
// Module:      McCannMuonQuality
// 
// Description: typedef for McCannCandidateQuality< DBCandidate::kMuon >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: McCannMuonQuality.h,v 1.2 1999/07/25 16:33:33 mkl Exp $
//
// Revision history
//
// $Log: McCannMuonQuality.h,v $
// Revision 1.2  1999/07/25 16:33:33  mkl
// merged in new Quality typedef files from mdc1 branch
//
// Revision 1.1.2.1  1999/07/24 20:54:13  mkl
// added typedef files for all the different track qualities
//
//

// user include files
#include "CleoDB/DBCandidate.h"
#include "McCannFilter/McCannCandidateQuality.h"

typedef
McCannCandidateQuality< DBCandidate::kMuon > McCannMuonQuality;

#endif /* KALMAN_KALMANMUONQUALITY_H */
