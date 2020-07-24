#if !defined(PACKAGE_SIGEOMETRYPROXY_H)
#define PACKAGE_SIGEOMETRYPROXY_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      SiGeometryProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Alexander Undrus
// Created:     Fri May 22 22:36:26 EDT 1998
// $Id: SiGeometryProxy.h,v 1.1.1.1 1998/08/18 07:42:04 cleo3 Exp $
//
// Revision history
//
// $Log: SiGeometryProxy.h,v $
// Revision 1.1.1.1  1998/08/18 07:42:04  cleo3
// imported SiGeom sources
//
//

// system include files

// user include files
#include "DataHandler/DataField.h"
#include "DetectorGeometry/DGDetectorStore.h"

// forward declarations

class SiGeometryProxy : public DataField< DGDetectorStore >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DGDetectorStore value_type;

      // Constructors and destructor
      SiGeometryProxy();
      virtual ~SiGeometryProxy();

      // member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const IfdKey& aKey );

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      SiGeometryProxy( const SiGeometryProxy& );

      // assignment operator(s)
      const SiGeometryProxy& operator=( const SiGeometryProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_SiGeometry ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "package/Template/SiGeometryProxy.cc"
//#endif

#endif /* PACKAGE_SIGEOMETRYPROXY_H */
