// -*- C++ -*-
//
// Package:     DummyHistogram
// Module:      T_vector_HINtuple.cc
// 
// Description: Instantiate STL classes for DummyHistograms
//
//
// Author:      Martin Lohner
// Created:     Wed Sep 11 21:45:29 EDT 1999
// $Id: T_vector_HINtuple.cc,v 1.1 1999/10/01 18:43:23 mkl Exp $
//
// Revision history
//
// $Log: T_vector_HINtuple.cc,v $
// Revision 1.1  1999/10/01 18:43:23  mkl
// fix up memory leaks
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files

// STL include files

class HINtuple;
typedef HINtuple* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"
