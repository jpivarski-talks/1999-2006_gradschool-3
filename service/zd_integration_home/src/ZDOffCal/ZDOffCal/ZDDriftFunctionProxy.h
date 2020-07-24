#if !defined(ZDOFFCAL_DRIFTFUNCTIONPROXY_H)
#define ZDOFFCAL_DRIFTFUNCTIONPROXY_H
// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      ZDDriftFunctionProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDDriftFunctionProxy.h,v 1.1 2003/04/24 18:08:17 huanggs Exp $
//
// Revision history
//
// $Log: ZDDriftFunctionProxy.h,v $
// Revision 1.1  2003/04/24 18:08:17  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:58  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:58  huanggs
// imported source
//
//

// user include files
#include "CommandPattern/Parameter.h"
#include "DataHandler/ProxyTemplate.h"
#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h" 
#include "ZDOffCal/ZDDriftFunction.h"
#include "ZDOffCal/ZDDFDataHolder.h"

class ZDOffCalProducer ;

class ZDDriftFunctionProxy : public ProxyTemplate< ZDDriftFunction >
{
  // friend classes and functions
  
public:
  // constants, enums and typedefs
  typedef ZDDriftFunction value_type;
  
  // Constructors and destructor
  ZDDriftFunctionProxy(ZDOffCalProducer* );
  virtual ~ZDDriftFunctionProxy();
  
  // member functions
  virtual void invalidateCache() ;
  virtual const value_type* faultHandler( const Record& aRecord,
					  const DataKey& aKey );
  

  // const member functions
  
  // static member functions
  
protected:
  // protected member functions
  
  // protected const member functions
  
private:
  // Constructors and destructor
  ZDDriftFunctionProxy( const ZDDriftFunctionProxy& );
  
  // assignment operator(s)
  const ZDDriftFunctionProxy& operator=( const ZDDriftFunctionProxy& );
  
  // private member functions
  
  // private const member functions
  
  // data members
  value_type*  m_DriftFunction ;
  ZDOffCalProducer*  m_Producer;
  ZDDFDataHolder m_dataHolder;
  // static data members
  
};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ZDOffCal/Template/DriftFunctionProxy.cc"
//#endif

#endif /* ZDOFFCAL_DRIFTFUNCTIONPROXY_H */
