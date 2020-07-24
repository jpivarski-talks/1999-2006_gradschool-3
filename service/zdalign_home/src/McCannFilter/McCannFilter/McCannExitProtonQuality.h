#if !defined(KALMAN_KALMANEXITPROTONQUALITY_H)
#define KALMAN_KALMANEXITPROTONQUALITY_H
// -*- C++ -*-
//
// Package:     McCann
// Module:      McCannExitProtonQuality
// 
// Description: typedef for McCannCandidateQuality< DBCandidate::kExitProton >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: McCannExitProtonQuality.h,v 1.2 1999/07/25 16:33:31 mkl Exp $
//
// Revision history
//
// $Log: McCannExitProtonQuality.h,v $
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
#include "McCannFilter/McCannCandidateQuality.h"

typedef
McCannCandidateQuality< DBCandidate::kExitProton > McCannExitProtonQuality;

#endif /* KALMAN_KALMANEXITPROTONQUALITY_H */
