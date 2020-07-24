#if !defined(KALMAN_KALMANELECTRONQUALITY_H)
#define KALMAN_KALMANELECTRONQUALITY_H
// -*- C++ -*-
//
// Package:     Pivarski
// Module:      PivarskiElectronQuality
// 
// Description: typedef for PivarskiCandidateQuality< DBCandidate::kElectron >
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner
// Created:     Sat Jul 24 10:32:02 EST 1999
// $Id: PivarskiElectronQuality.h,v 1.2 1999/07/25 16:33:27 mkl Exp $
//
// Revision history
//
// $Log: PivarskiElectronQuality.h,v $
// Revision 1.2  1999/07/25 16:33:27  mkl
// merged in new Quality typedef files from mdc1 branch
//
// Revision 1.1.2.1  1999/07/24 20:54:05  mkl
// added typedef files for all the different track qualities
//
//

// user include files
#include "CleoDB/DBCandidate.h"
#include "PivarskiFilter/PivarskiCandidateQuality.h"

typedef
PivarskiCandidateQuality< DBCandidate::kElectron > PivarskiElectronQuality;

#endif /* KALMAN_KALMANELECTRONQUALITY_H */
