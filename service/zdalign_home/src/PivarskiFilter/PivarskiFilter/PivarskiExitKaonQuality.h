#if !defined(KALMAN_KALMANEXITKAONQUALITY_H)
#define KALMAN_KALMANEXITKAONQUALITY_H
// -*- C++ -*-
//
// Package:     Pivarski
// Module:      PivarskiExitKaonQuality
// 
// Description: typedef for PivarskiCandidateQuality< DBCandidate::kExitKaon >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: PivarskiExitKaonQuality.h,v 1.2 1999/07/25 16:33:29 mkl Exp $
//
// Revision history
//
// $Log: PivarskiExitKaonQuality.h,v $
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
#include "PivarskiFilter/PivarskiCandidateQuality.h"

typedef
PivarskiCandidateQuality< DBCandidate::kExitKaon > PivarskiExitKaonQuality;

#endif /* KALMAN_KALMANEXITKAONQUALITY_H */
