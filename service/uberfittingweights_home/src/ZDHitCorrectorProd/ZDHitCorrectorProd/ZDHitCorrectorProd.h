#if !defined(ZDHITCORRECTORPROD_ZDHITCORRECTORPROD_H)
#define ZDHITCORRECTORPROD_ZDHITCORRECTORPROD_H
// -*- C++ -*-
//
// Package:     <ZDHitCorrectorProd>
// Module:      ZDHitCorrectorProd
// 
// Description:
//
// Usage:
//   
//
// Author:      Guangshun Huang
// Created:     Thu Feb 27 2003
// $Id: ZDHitCorrectorProd.h,v 1.1.1.1 2003/08/30 00:17:08 wsun Exp $
//
// Revision history
//
// $Log: ZDHitCorrectorProd.h,v $
// Revision 1.1.1.1  2003/08/30 00:17:08  wsun
// Imported ZDHitCorrectorProd sources.
//
//
//
// system include files
//
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
//
// user include files
//
#include "Processor/Producer.h"
#include "ToolBox/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

// forward declarations
#include "STLUtility/fwd_vector.h"
class ZDHitCorrectorProxy ;

class ZDHitCorrectorProd : public Producer
{
   public:
      // ------------ Constructors and destructor -----------------
      ZDHitCorrectorProd( void );                      // anal1 
      virtual ~ZDHitCorrectorProd();                   // anal5 

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( TBHistoManager& );                  

      DABoolean applyStandardCorrections() const
      { return m_applyStandardCorrections.value() ; }

      DABoolean disableAllCorrections() const
      { return m_disableAllCorrections.value() ; }

      DABoolean applySignalPropCorr() const
      { return m_applySignalPropCorr.value() ; }

      DABoolean applyTimeOfFlightCorr() const
      { return m_applyTimeOfFlightCorr.value() ; }

      DABoolean applyPulseHeightCorr() const
      { return m_applyPulseHeightCorr.value() ; }

      DABoolean applyEntranceAngleCorr() const
      { return m_applyEntranceAngleCorr.value() ; }

   protected:
      ProxyBase* makeProxy();

   private:
      // ------------ Constructors and destructor -----------------
      ZDHitCorrectorProd( const ZDHitCorrectorProd& );

      // ------------ assignment operator(s) ----------------------
      const ZDHitCorrectorProd& operator=( const ZDHitCorrectorProd& );

      // ------------ data members --------------------------------
      ZDHitCorrectorProxy*  m_ZDHitCorrectorProxy ;  

      Parameter< DABoolean > m_applyStandardCorrections ;
      Parameter< DABoolean > m_disableAllCorrections ;
      Parameter< DABoolean > m_applySignalPropCorr ;
      Parameter< DABoolean > m_applyTimeOfFlightCorr ;
      Parameter< DABoolean > m_applyPulseHeightCorr ;
      Parameter< DABoolean > m_applyEntranceAngleCorr ;
};
#endif /* ZDHITCORRECTORPROD_ZDHITCORRECTORPROD_H */
