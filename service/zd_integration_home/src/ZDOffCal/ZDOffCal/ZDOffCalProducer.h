#if !defined(PACKAGE_ZDOFFCALPRODUCER_H)
#define PACKAGE_ZDOFFCALPRODUCER_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      ZDOffCalProducer
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDOffCalProducer.h,v 1.1.1.1 2003/02/03 20:11:58 huanggs Exp $
//
// Revision history
//
// $Log: ZDOffCalProducer.h,v $
// Revision 1.1.1.1  2003/02/03 20:11:58  huanggs
// imported source
//
//

// user include files
#include "Processor/Processor.h"
#include "CommandPattern/Parameter.h"

// forward declarations

class ZDOffCalProducer : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      ZDOffCalProducer( void );                      // anal1 
      virtual ~ZDOffCalProducer();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // ------------ const member functions ----------------------

      inline double getMinTightTime() const {return m_MinTightTime.value()    ;};
      inline double getMaxTightTime() const {return m_MaxTightTime.value()    ;};
      inline double getMinLooseTime() const {return m_MinLooseTime.value()    ;};
      inline double getMaxLooseTime() const {return m_MaxLooseTime.value()    ;};
      inline double getGlobalT0()    const {return m_GlobalT0.value()         ;};
      inline double getTDCQuantum()  const {return m_TDCQuantum.value()       ;};
      inline double getResolution()  const {return m_Resolution.value()       ;};
      inline double getDebugLevel()  const {return m_DebugLevel.value()       ;};
      inline long   getType()        const {return m_DriftFunctionType.value();};

      DABoolean shiftTimeToCenter() const {return m_shiftTimeToCenter.value();};
      //
      //
      // 
      void signalPropBeta_Set(double signalPropBeta) { m_signalPropBeta_RealValue = signalPropBeta; };
      double signalPropBeta_Par() const { return m_signalPropBeta.value(); };
      double signalPropBeta_Val() const { return m_signalPropBeta_RealValue; };
      
      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      ZDOffCalProducer( const ZDOffCalProducer& );

      // ------------ assignment operator(s) ----------------------
      const ZDOffCalProducer& operator=( const ZDOffCalProducer& );

      // ------------ private member functions --------------------
      ProxyBase* makeDriftFunctionProxy() ;

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------
      Parameter<long>      m_DriftFunctionType  ; 
      Parameter<long>      m_DebugLevel         ; 
      Parameter<double>    m_MinTightTime       ;
      Parameter<double>    m_MaxTightTime       ;
      Parameter<double>    m_MinLooseTime       ;
      Parameter<double>    m_MaxLooseTime       ;
      Parameter<double>    m_GlobalT0           ;
      Parameter<double>    m_TDCQuantum         ;
      Parameter<double>    m_Resolution         ;

      Parameter< DABoolean > m_shiftTimeToCenter ;
      Parameter< double > m_signalPropBeta ;
      double m_signalPropBeta_RealValue    ;

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* PACKAGE_ZDOFFCALPRODUCER_H */
