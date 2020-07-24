#if !defined(KALMAN_KALMANEXITPIONQUALITY_H)
#define KALMAN_KALMANEXITPIONQUALITY_H
// -*- C++ -*-
//
// Package:     McCann
// Module:      McCannExitPionQuality
// 
// Description: typedef for McCannCandidateQuality< DBCandidate::kExitPion >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: McCannExitPionQuality.h,v 1.2 1999/07/25 16:33:30 mkl Exp $
//
// Revision history
//
// $Log: McCannExitPionQuality.h,v $
// Revision 1.2  1999/07/25 16:33:30  mkl
// merged in new Quality typedef files from mdc1 branch
//
// Revision 1.1.2.1  1999/07/24 20:54:07  mkl
// added typedef files for all the different track qualities
//
//

// user include files
#include "CleoDB/DBCandidate.h"
#include "McCannFilter/McCannCandidateQuality.h"

typedef
McCannCandidateQuality< DBCandidate::kExitPion > McCannExitPionQuality;

#endif /* KALMAN_KALMANEXITPIONQUALITY_H */
