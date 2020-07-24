// -*- C++ -*-
#if !defined(DUHPROD_DUHPROD_H)
#define DUHPROD_DUHPROD_H
//
// Package:     <DuhProd>
// Module:      DuhProd
//
/**\class DuhProd DuhProd.h DuhProd/DuhProd.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Dec 11 16:31:22 EST 2003
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

class DuhProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      DuhProd( void );                      // anal1 
      virtual ~DuhProd();                   // anal5 

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
      DuhProd( const DuhProd& );

      // ------------ assignment operator(s) ----------------------
      const DuhProd& operator=( const DuhProd& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* DUHPROD_DUHPROD_H */
