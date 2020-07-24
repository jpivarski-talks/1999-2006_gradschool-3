// -*- C++ -*-
//
// Package:     <Suez>
// Module:      testBoolean
// 
// Description: test if Boolean is properly implemented
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner, Brian Heltsley
// Created:     Thu Mar 12 10:40:25 EST 1998
// $Id: testBoolean.cc,v 1.1 1998/03/12 16:16:17 mkl Exp $
//
// Revision history
//
// $Log: testBoolean.cc,v $
// Revision 1.1  1998/03/12 16:16:17  mkl
// added testing for Boolean problems (true .neq. .not.false)
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "Suez/testBoolean.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Suez.testBoolean" ;

void 
testBoolean()
{
   if( true != !false )
   {
      report( EMERGENCY, kFacilityString ) 
	 << " true is not !false; disaster!" << endl;
      assert( false ) ;
      exit(1) ;
   }
}
