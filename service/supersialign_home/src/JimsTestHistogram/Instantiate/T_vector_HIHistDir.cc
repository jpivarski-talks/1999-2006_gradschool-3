// -*- C++ -*-
//
// Package:     DummyNtuples
// Module:      T_vector_HIHistDir.cc
// 
// Description: Instantiate STL classes for DummyHistogram
//
//
// Author:      Martin Lohner
// Created:     Wed Sep 11 21:45:29 EDT 1999
// $Id: T_vector_HIHistDir.cc,v 1.1 1999/10/01 18:43:20 mkl Exp $
//
// Revision history
//
// $Log: T_vector_HIHistDir.cc,v $
// Revision 1.1  1999/10/01 18:43:20  mkl
// fix up memory leaks
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files

// STL include files

class HIHistDir;
typedef HIHistDir* _vector_contents_ ;

#include "STLUtility/instantiate_vector.h"
