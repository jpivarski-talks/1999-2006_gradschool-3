#if !defined(CALIBRATEDZDHITPROD_CALIBRATEDZDHITPROD_H)
#define CALIBRATEDZDHITPROD_CALIBRATEDZDHITPROD_H
// -*- C++ -*-
//
// Package:     <CalibratedZDHitProd>
// Module:      CalibratedZDHitProd
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Brian Heltsley
// Created:     Wed Sep 22 14:17:43 EDT 1999
// $Id: CalibratedZDHitProd.h,v 1.1.1.1 2002/09/27 20:34:03 bkh Exp $
//
// Revision history
//
// $Log: CalibratedZDHitProd.h,v $
// Revision 1.1.1.1  2002/09/27 20:34:03  bkh
// imported CalibratedZDHitProd sources
//
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "HistogramInterface/HistogramPackage.h"

// forward declarations

class CalibratedZDHitProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      CalibratedZDHitProd( void );                      // anal1 
      virtual ~CalibratedZDHitProd();                   // anal5 

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
      CalibratedZDHitProd( const CalibratedZDHitProd& );

      // ------------ assignment operator(s) ----------------------
      const CalibratedZDHitProd& operator=( const CalibratedZDHitProd& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* CALIBRATEDZDHITPROD_CALIBRATEDZDHITPROD_H */
