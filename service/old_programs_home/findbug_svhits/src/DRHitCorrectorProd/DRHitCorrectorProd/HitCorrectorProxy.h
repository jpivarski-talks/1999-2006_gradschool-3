#if !defined(DRHITCORRECTORPROD_HITCORRECTORPROXY_H)
#define DRHITCORRECTORPROD_HITCORRECTORPROXY_H
// -*- C++ -*-
//
// Package:     <DRHITCORRECTORPROD>
// Module:      HitCorrectorProxy
// 
// Description: gets constants,geometry,calibration,instantiates
//              deletes HitCorrector as needed
//
// Usage:
//    <usage>
//
// Author:      Ken McLean
// Created:     Thu Aug 19 15:15:56 EST 1998
// $Id: HitCorrectorProxy.h,v 1.4 2000/10/31 22:56:39 mclean Exp $
//
// Revision history
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
#include "DRHitCorrectorProd/HitCorrector.h"
//
// forward declarations
//
class DRHitCorrectorProd ;

class HitCorrectorProxy : public DataField< HitCorrector >
{  
   public:
      // constants, enums and typedefs
      typedef HitCorrector value_type;
  
      // Constructors and destructor
      HitCorrectorProxy( DRHitCorrectorProd* aProd ) ;
      virtual ~HitCorrectorProxy();
  
      // member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const IfdKey& aKey );
   protected:

   private:
      // Constructors and destructor
      HitCorrectorProxy( const HitCorrectorProxy& );
  
      // assignment operator(s)
      const HitCorrectorProxy& operator=( const HitCorrectorProxy& );
  
      // data members
      value_type* m_HitCorrector ;
      DRHitCorrectorProd* m_prod ;
};
#endif /* DRHITCORRECTORPROD_HITCORRECTORPROXY_H */
