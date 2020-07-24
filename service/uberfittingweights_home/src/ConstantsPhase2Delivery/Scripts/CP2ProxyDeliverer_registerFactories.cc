// -*- C++ -*-
//
// Package:     <ConstantsPhase2Delivery>
// Module:      CP2ProxyDeliverer_registerFactories
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Valentine Kouznetsov
// Created:     Tue Jan 28 12:47:41 EST 2003
// $Id: CP2ProxyDeliverer_registerFactories.cc,v 1.1.1.1 2003/01/28 21:49:55 vk Exp $
//
// Revision history
//
// $Log: CP2ProxyDeliverer_registerFactories.cc,v $
// Revision 1.1.1.1  2003/01/28 21:49:55  vk
// imported ConstantsPhase2Delivery sources
//
//
// First version which works with new DB API.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ConstantsPhase2Delivery/CP2ProxyDeliverer.h"
#include "ConstantsPhase2Delivery/DBCP2ProxyFactoryTemplate.h"

_stream_incs_

_constant_incs_

#include "DBConstants.hxx"

// STL classes

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ConstantsPhase2Delivery.CP2ProxyDeliverer_registerFactories" ;

//
// static data member definitions
//


void
CP2ProxyDeliverer::registerProxyFactories()
{
   //put all the code necessary to register each factory here

_register_proxies_

}

#include "ConstantsPhase2Delivery/Template/DBCP2ProxyFactoryTemplate.cc"
#include "ConstantsPhase2Delivery/Template/DBCP2Proxy.cc"

_instantiate_factories_
