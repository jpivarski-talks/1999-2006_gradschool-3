// -*- C++ -*-
//
// Package:     CommandPattern
// Module:      STL_Command
// 
// Description: Instantiate STL classes for Command
//
//
// Author:      Martin Lohner
// Created:     Fri July 11 12:00:00 EDT 1997
// $Id: STL_vectorOfCalibratedDRHits.cc,v 1.1.1.1 1998/04/30 14:53:04 lyon Exp $
//
// Revision history
//
// $Log: STL_vectorOfCalibratedDRHits.cc,v $
// Revision 1.1.1.1  1998/04/30 14:53:04  lyon
// Imported TrackFinder source
//
// Revision 1.3  1998/03/15 00:07:21  mkl
// instantiation for all Parameter< vector< T > > templates
//
// Revision 1.2  1998/02/03 21:28:04  mkl
// switch over to STLUtility library
//
// Revision 1.1  1997/09/23 17:39:58  cdj
// Moved from JobControl library
//
// Revision 1.3  1997/08/29 04:22:04  mkl
// simplify JobControl: remove FileInput/Output files, bug flag changes, handling of suffix .gz etc.
//
// Revision 1.2  1997/08/13 07:10:57  mkl
// Chains of Sources, new activation of streams, and ProxyDict
//
// Revision 1.1  1997/07/11 20:40:23  mkl
// STL instantiation files added
//
//

#include "Experiment/Experiment.h"
// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <vector>

// user include files

class CalibratedDRHit;
typedef const CalibratedDRHit* _vector_contents_;

#include "STLUtility/instantiate_vector.h"


