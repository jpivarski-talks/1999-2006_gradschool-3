#if !defined(DOITPROD_DOITPROD_H)
#define DOITPROD_DOITPROD_H
// -*- C++ -*-
//
// Package:     <DoitProd>
// Module:      DoitProd
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Adam Lyon
// Created:     Fri Jul 23 17:06:16 EDT 1999
// $Id: DoitProd.h,v 1.5 2002/09/27 21:04:23 bkh Exp $
//
// Revision history
//
// $Log: DoitProd.h,v $
// Revision 1.5  2002/09/27 21:04:23  bkh
// For ZD capability
//
// Revision 1.4  2000/06/09 00:42:32  lkg
// New DABoolean parameter UseSilicon, and member fcn to access the parameter
// value (useSilicon()) added.
//
// Revision 1.3  2000/02/08 18:37:38  lyon
// Added DoitModeNumber parameter
//
// Revision 1.2  2000/01/14 02:54:34  lyon
// Add parameters to continue if hits are missing
//
// Revision 1.1.1.1  1999/09/08 18:53:32  lyon
// imported DoitProd sources
//
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "ToolBox/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

// forward declarations

class DoitProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      DoitProd( void );                      // anal1 
      virtual ~DoitProd();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( TBHistoManager& );                  

      // ------------ const member functions ----------------------
      DABoolean continueIfHitsMissing() const
      { 
	return m_continueIfHitsMissing.value() ;
      }
      
      DABoolean continueIfAnyException() const
      { 
	return m_continueIfAnyException.value() ; 
      }

      DABoolean useSilicon() const
      {
	return m_useSilicon.value() ;
      }

      DABoolean useZD() const
      {
	return m_useZD.value() ;
      }

      int diagLevel() const
      {
	return m_diagLevel.value();
      }

      int modeNumber() const
      {
	return m_modeNumber.value();
      }

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      DoitProd( const DoitProd& );

      // ------------ assignment operator(s) ----------------------
      const DoitProd& operator=( const DoitProd& );

      // ------------ private member functions --------------------
      ProxyBase* makeDoitProxy();
      

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------
      Parameter<long>  m_diagLevel;
      Parameter<long>  m_modeNumber;
      Parameter< DABoolean > m_continueIfHitsMissing ;
      Parameter< DABoolean > m_continueIfAnyException ;
      Parameter< DABoolean > m_useSilicon ;
      Parameter< DABoolean > m_useZD ;

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* DOITPROD_DOITPROD_H */
