// -*- C++ -*-
#if !defined(FAKEATRACKPROD_FAKEATRACKPROD_H)
#define FAKEATRACKPROD_FAKEATRACKPROD_H
//
// Package:     <FakeATrackProd>
// Module:      FakeATrackProd
//
/**\class FakeATrackProd FakeATrackProd.h FakeATrackProd/FakeATrackProd.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri Jun 27 13:31:29 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "HistogramInterface/HistogramPackage.h"

// forward declarations

class FakeATrackProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      FakeATrackProd( void );                      // anal1 
      virtual ~FakeATrackProd();                   // anal5 

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
      FakeATrackProd( const FakeATrackProd& );

      // ------------ assignment operator(s) ----------------------
      const FakeATrackProd& operator=( const FakeATrackProd& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* FAKEATRACKPROD_FAKEATRACKPROD_H */
