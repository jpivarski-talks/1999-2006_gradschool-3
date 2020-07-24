// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      ConstantsProxyDeliverer_registerFactories
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Tue Aug 18 23:00:20 EDT 1998
// $Id: ConstantsProxyDeliverer_registerFactories.cc,v 1.2 2000/04/27 21:14:14 cdj Exp $
//
// Revision history
//
// $Log: ConstantsProxyDeliverer_registerFactories.cc,v $
// Revision 1.2  2000/04/27 21:14:14  cdj
// now works on OSF using MICO ORB
//
// Revision 1.1  1999/05/13 21:59:48  cdj
// Added autogeneration of Proxies
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ConstantsDelivery/ConstantsProxyDeliverer.h"
#include "ConstantsDelivery/DBConstantsProxyFactoryTemplate.h"

_stream_incs_

_constant_incs_

#include "DBConstants.hxx"

// STL classes

//
// constants, enums and typedefs
//
// 11/5/98 Comment out kFacilityString to avoid multiple declaration
//static const char* const kFacilityString = "ConstantsDelivery.ConstantsProxyDeliverer_registerFactories" ;

//
// static data member definitions
//


void
ConstantsProxyDeliverer::registerProxyFactories()
{
   //put all the code necessary to register each factory here

_register_proxies_

}

#include "ConstantsDelivery/Template/DBConstantsProxyFactoryTemplate.cc"
#include "ConstantsDelivery/Template/DBConstantsProxy.cc"

_instantiate_factories_
