#if !defined(KALMAN_KALMANEXITMUONQUALITY_H)
#define KALMAN_KALMANEXITMUONQUALITY_H
// -*- C++ -*-
//
// Package:     Pivarski
// Module:      PivarskiExitMuonQuality
// 
// Description: typedef for PivarskiCandidateQuality< DBCandidate::kExitMuon >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: PivarskiExitMuonQuality.h,v 1.2 1999/07/25 16:33:30 mkl Exp $
//
// Revision history
//
// $Log: PivarskiExitMuonQuality.h,v $
// Revision 1.2  1999/07/25 16:33:30  mkl
// merged in new Quality typedef files from mdc1 branch
//
// Revision 1.1.2.1  1999/07/25 16:28:37  mkl
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
PivarskiCandidateQuality< DBCandidate::kExitMuon > PivarskiExitMuonQuality;

#endif /* KALMAN_KALMANEXITMUONQUALITY_H */
