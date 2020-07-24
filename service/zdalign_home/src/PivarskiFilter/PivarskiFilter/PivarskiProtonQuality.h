#if !defined(KALMAN_KALMANPROTONQUALITY_H)
#define KALMAN_KALMANPROTONQUALITY_H
// -*- C++ -*-
//
// Package:     Pivarski
// Module:      PivarskiProtonQuality
// 
// Description: typedef for PivarskiCandidateQuality< DBCandidate::kProton >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: PivarskiProtonQuality.h,v 1.2 1999/07/25 16:33:34 mkl Exp $
//
// Revision history
//
// $Log: PivarskiProtonQuality.h,v $
// Revision 1.2  1999/07/25 16:33:34  mkl
// merged in new Quality typedef files from mdc1 branch
//
// Revision 1.1.2.1  1999/07/24 20:54:19  mkl
// added typedef files for all the different track qualities
//
//

// user include files
#include "CleoDB/DBCandidate.h"
#include "PivarskiFilter/PivarskiCandidateQuality.h"

typedef
PivarskiCandidateQuality< DBCandidate::kProton > PivarskiProtonQuality;

#endif /* KALMAN_KALMANPROTONQUALITY_H */
