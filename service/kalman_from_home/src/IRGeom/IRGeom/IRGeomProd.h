#if !defined(IRGEOM_IRGEOMPROD_H)
#define IRGEOM_IRGEOMPROD_H
// -*- C++ -*-
//
// Package:     <IRGeom>
// Module:      IRGeomProd
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Lawrence Gibbons
// Created:     Thu Jan  7 14:54:35 EST 1999
// $Id: IRGeomProd.h,v 1.1.1.1 1999/07/22 14:38:42 lkg Exp $
//
// Revision history
//
// $Log: IRGeomProd.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:42  lkg
// First release
//
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "ToolBox/HistogramPackage.h"

// forward declarations

class IRGeomProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      IRGeomProd( void );                      // anal1 
      virtual ~IRGeomProd();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( TBHistoManager& );                  

      // ------------ const member functions ----------------------

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      IRGeomProd( const IRGeomProd& );

      // ------------ assignment operator(s) ----------------------
      const IRGeomProd& operator=( const IRGeomProd& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* IRGEOM_IRGEOMPROD_H */
