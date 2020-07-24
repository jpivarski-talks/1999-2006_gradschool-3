#if !defined(ZDHITCORRECTORPROD_ZDHITCORRECTORPROXY_H)
#define ZDHITCORRECTORPROD_ZDHITCORRECTORPROXY_H
// -*- C++ -*-
//
// Package:     <ZDHITCORRECTORPROD>
// Module:      ZDHitCorrectorProxy
// 
// Description: gets constants,geometry,calibration,instantiates
//              deletes ZDHitCorrector as needed
//
// Usage:
//    <usage>
//
// Author:      Guangshun Huang
// Created:     Thu Feb 27 2003
// $Id: ZDHitCorrectorProxy.h,v 1.1.1.1 2003/08/30 00:17:08 wsun Exp $
//
// Revision history
//
// $Log: ZDHitCorrectorProxy.h,v $
// Revision 1.1.1.1  2003/08/30 00:17:08  wsun
// Imported ZDHitCorrectorProd sources.
//
//
// system include files
//
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
//
// user include files
//
#include "CommandPattern/Parameter.h"
#include "DataHandler/DataField.h"
#include "ZDHitCorrectorProd/ZDHitCorrector.h"
//
// forward declarations
//
class ZDHitCorrectorProd ;

class ZDHitCorrectorProxy : public DataField< ZDHitCorrector >
{  
   public:
      // constants, enums and typedefs
      typedef ZDHitCorrector value_type;
  
      // Constructors and destructor
      ZDHitCorrectorProxy( ZDHitCorrectorProd* aProd ) ;
      virtual ~ZDHitCorrectorProxy();
  
      // member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const IfdKey& aKey );
   protected:

   private:
      // Constructors and destructor
      ZDHitCorrectorProxy( const ZDHitCorrectorProxy& );
  
      // assignment operator(s)
      const ZDHitCorrectorProxy& operator=( const ZDHitCorrectorProxy& );
  
      // data members
      value_type* m_ZDHitCorrector ;
      ZDHitCorrectorProd* m_prod ;
};
#endif /* ZDHITCORRECTORPROD_ZDHITCORRECTORPROXY_H */
