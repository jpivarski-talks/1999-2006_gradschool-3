// -*- C++ -*-
//
// Package:     <HbookHistogramModule>
// Module:      HbookHistogramModule_DONT_TOUCH.cc
// 
// Description: factory function to create HbookHistogramModule
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Fri Nov 20 01:16:35 EST 1998
// $Id: HbookHistogramModule_DONT_TOUCH.cc,v 1.1.1.1 1999/09/06 22:05:30 mkl Exp $
//
// Revision history
//
// $Log: HbookHistogramModule_DONT_TOUCH.cc,v $
// Revision 1.1.1.1  1999/09/06 22:05:30  mkl
// imported HbookHistogramModule sources
//
// Revision 1.1.1.1  1998/11/20 07:08:27  mkl
// new HbookHistogramModule
//
//

#include "Experiment/Experiment.h"

// system include files

// user include files
#include "HbookHistogramModule/HbookHistogramModule.h"

// STL classes

//
// constants, enums and typedefs
//

// --------------------------- factory method -------------------
extern "C" { 
   Module* makeModule( void ); 
}

Module*
makeModule( void )
{
   return new HbookHistogramModule;
}
// ------------------------ end factory method ------------------
