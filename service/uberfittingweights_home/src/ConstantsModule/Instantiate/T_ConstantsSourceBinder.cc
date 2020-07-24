// -*- C++ -*-
//
// Package:     ConstantsModule
// Module:      T_ConstantsSourceBinder
// 
// Description: Instantiate Binder template for ConstantsSourceBinder
//
//
// Author:      Martin Lohner
// Created:     Tue Oct 21 12:20:00 EDT 1998
// $Id: T_ConstantsSourceBinder.cc,v 1.2 2000/04/27 21:17:23 cdj Exp $
//
// Revision history
//
// $Log: T_ConstantsSourceBinder.cc,v $
// Revision 1.2  2000/04/27 21:17:23  cdj
// now works on OSF using MICO ORB
//
// Revision 1.1  1998/11/18 18:41:26  baker
// Rename T_DBSourceBinder -> T_ConstantsSourceBiner
//
// Revision 1.1.1.1  1998/11/16 22:04:26  mkl
// imported ConstantsModule sources
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <map>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ConstantsDelivery/ConstantsSourceBinder.h"

// stl includes
#include <string>
#include <map>

typedef ConstantsSourceBinder _binder_;
#include "DataDelivery/instantiateNotChainableBinder.h"

