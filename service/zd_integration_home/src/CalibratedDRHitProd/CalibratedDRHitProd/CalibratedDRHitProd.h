#if !defined(CALIBRATEDDRHITPROD_CALIBRATEDDRHITPROD_H)
#define CALIBRATEDDRHITPROD_CALIBRATEDDRHITPROD_H
// -*- C++ -*-
//
// Package:     <CalibratedDRHitProd>
// Module:      CalibratedDRHitProd
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Wed Sep 22 14:17:43 EDT 1999
// $Id: CalibratedDRHitProd.h,v 1.3 2001/11/29 22:17:39 bkh Exp $
//
// Revision history
//
// $Log: CalibratedDRHitProd.h,v $
// Revision 1.3  2001/11/29 22:17:39  bkh
// Restored raw dr hit proxy back to RawData producer
//
// Revision 1.1.1.1  1999/09/23 02:44:15  cdj
// imported CalibratedDRHitProd sources
//
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "HistogramInterface/HistogramPackage.h"

// forward declarations

class CalibratedDRHitProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      CalibratedDRHitProd( void );                      // anal1 
      virtual ~CalibratedDRHitProd();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // ------------ const member functions ----------------------

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      CalibratedDRHitProd( const CalibratedDRHitProd& );

      // ------------ assignment operator(s) ----------------------
      const CalibratedDRHitProd& operator=( const CalibratedDRHitProd& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* CALIBRATEDDRHITPROD_CALIBRATEDDRHITPROD_H */
