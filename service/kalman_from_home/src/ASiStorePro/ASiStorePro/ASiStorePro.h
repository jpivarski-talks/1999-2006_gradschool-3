#if !defined(ASISTOREPRO_ASISTOREPRO_H)
#define ASISTOREPRO_ASISTOREPRO_H
// -*- C++ -*-
//
// Package:     ASiStorePro
// Module:      ASiStorePro
// 
// Description: this producer provides proxy to generate Si Store
//
// Usage:
//    <usage>
//
// Author:      Alexander Undrus
// Created:     Sat Jan  9 12:06:13 EST 1999
// $Id: ASiStorePro.h,v 1.4 1999/07/03 22:27:10 cleo3 Exp $
//
// Revision history
//
// $Log: ASiStorePro.h,v $
// Revision 1.4  1999/07/03 22:27:10  cleo3
//  ASiSensorAddress is transferred from ASiSensor package, ASiStorePro becomes producer, all references to ASiSensor are eliminated
//
// Revision 1.3  1999/06/28 22:37:43  cleo3
//  print diagnostics if param printkey = 1
//
// Revision 1.2  1999/05/18 03:45:22  cleo3
// *** empty log message ***
//
// Revision 1.1.1.1  1999/01/23 02:28:03  undrus
// imported c3tr sources
//
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "CommandPattern/Parameter.h"
#include "CommandPattern/Menu.h"
// forward declarations
class ProxyBase;

class ASiStorePro : public Producer
{
      // ------------ friend classses and functions ---------------
friend class ASiStoreConstants;
   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      ASiStorePro( void );                      // anal1 
      virtual ~ASiStorePro();                   // anal5 

      // ------------ member functions ----------------------------
      int printkey() const;
      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms

      // ------------ const member functions ----------------------

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

   private:
      // ------------ Constructors and destructor -----------------
      ASiStorePro( const ASiStorePro& );
  
      // ------------ assignment operator(s) ----------------------
      const ASiStorePro& operator=( const ASiStorePro& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------
      ProxyBase* makeASiStoreProxy() const;

      // ------------ data members --------------------------------
      Parameter<int>   m_printkey;
      // ------------ static data members -------------------------

};

// inline function definitions
inline
int ASiStorePro::printkey() const
{ return m_printkey.value(); }

#endif /* ASISTOREPRO_ASISTOREPRO_H */





