// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MIFCNInternal
// 
// Description: The FCN function for Minuit -- this function is called by
//              Minuit itself. You should NOT call it directly.
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Thu Nov 18 16:14:51 EST 1999
// $Id: MIFCNInternal.cc,v 1.1.1.1 1999/12/09 15:34:51 lyon Exp $
//
// Revision history
//
// $Log: MIFCNInternal.cc,v $
// Revision 1.1.1.1  1999/12/09 15:34:51  lyon
// Imported MinuitInterface sources
//

#include "Experiment/Experiment.h"
#include "Experiment/fortran_types.h"
#include "Experiment/report.h"
#include <assert.h>

#include "MinuitInterface/MinuitInterface.h"
#include "MinuitInterface/MIFcn.h"

extern "C"
{
  void MIFCNInternal(FInteger* npar, FDouble* grad, FDouble* fval, 
	     FDouble* xval, FInteger* iflag, void* futil)

  {

    enum flags { kInit = 1, kGrad, kFinal};

    // Get the MinuitInterface instance
    MinuitInterface* mi = MinuitInterface::instance();

    // Get the fcn
    MIFcn* fcn = mi->fcn();

    // Are we initializing?
    if ( *iflag == kInit ) 
    {
      fcn->initialize();
    }
    
    // Do we need to handle derivatives?
    if ( *iflag == kGrad )
    {
      fcn->derivatives(xval, grad);
    }

    // ALWAYS iterate
    double fcnVal = fcn->iterate(xval);
    *fval = fcnVal;
    
    // Finalize?
    if ( *iflag == kFinal )
    {
      fcn->finalize();
    }
  }
}
