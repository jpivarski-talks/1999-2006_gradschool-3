#if !defined(HISTOGRAMINTERFACE_HISTOGRAMPACKAGE_H)
#define HISTOGRAMINTERFACE_HISTOGRAMPACKAGE_H
// -*- C++ -*-
//
// Package:     HistogramInterface
// Module:      HistogramPackage
// 
// Description: Include all Histogram headers for easy reference
//
// Usage:
//    <Usage>
//
// Author:      Martin Lohner
// Created:     Wed Aug 31 10:58:31 EDT 1999
// $Id: HistogramPackage.h,v 1.2 1999/12/23 23:25:28 cdj Exp $
//
// Revision history
//
// $Log: HistogramPackage.h,v $
// Revision 1.2  1999/12/23 23:25:28  cdj
// added arithmetic operations on histograms
//
// Revision 1.1  1999/09/07 18:53:33  mkl
// new histogram interface
//
//

#include "HistogramInterface/HIHist1D.h"
#include "HistogramInterface/HIHist2D.h"
#include "HistogramInterface/HIHistProf.h"
#include "HistogramInterface/HINtuple.h"
#include "HistogramInterface/HIHistoManager.h"
#include "HistogramInterface/HIDirSwitch.h"
#include "HistogramInterface/HIHistDir.h"

#include "HistogramInterface/HIArithmetic/HIArithmetic.h"

#endif /* HISTOGRAMINTERFACE_HISTOGRAMMANAGER_H */
