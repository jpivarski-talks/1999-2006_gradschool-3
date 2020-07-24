#if !defined(ADRGEOMPROD_ADRGEOMPROD_H)
#define ADRGEOMPROD_ADRGEOMPROD_H
// -*- C++ -*-
//
// Package:     ADRGeomProd
// Module:      ADRGeomProd
// 
// Description: This producer provides proxies to generate the Active
//              DR3.
//
// Usage:
//    Selecting this producer allows you to extract Active DR Geometry
//
//
// Author:      Inga Karliner
// Created:     
//
// Revision history
// 4/28/99 new Proxy registration method
// 12/1/99 I.Karliner New library ADRGeomProd (was in ADRGeom)
// 6/15/00 I.Karliner Eliminate tool box typedefs

// system include files

// user include files
#include "Processor/Processor.h"
//#include "ToolBox/HistogramPackage.h"

// forward declarations

class ADRGeomProd: public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      ADRGeomProd( void );                      // anal1 
      virtual ~ADRGeomProd();                   // anal5 

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

      // register your proxies 
  //      virtual void registerProxies( const Stream::Type& aStream ,
  //				    KeyedProxies& aProxyList );

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      ADRGeomProd( const ADRGeomProd& );

      // ------------ assignment operator(s) ----------------------
      const ADRGeomProd& operator=( const ADRGeomProd& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* ADRGEOMPROD_ADRGEOMPROD_H */
