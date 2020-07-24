#if !defined(IRGEOM_IRGEOMETRYPROXY_H)
#define IRGEOM_IRGEOMETRYPROXY_H
// -*- C++ -*-
//
// Package:     IRGeom
// Module:      IRGeometryProxy
// 
// Description: Provide the interaction region geometry
//
// Usage:
//    <usage>
//
// Author:      Lawrence Gibbons
// Created:     Mon Jan  4 08:44:07 EST 1999
// $Id: IRGeometryProxy.h,v 1.1.1.1 1999/07/22 14:38:40 lkg Exp $
//
// Revision history
//
// $Log: IRGeometryProxy.h,v $
// Revision 1.1.1.1  1999/07/22 14:38:40  lkg
// First release
//
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "DetectorGeometry/DGDetectorStore.h"

// forward declarations

class IRGeometryProxy : public ProxyTemplate< DGDetectorStore >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DGDetectorStore value_type;

      // Constructors and destructor
      IRGeometryProxy();
      virtual ~IRGeometryProxy();

      // member functions

      // const member functions

      // static member functions

   protected:
      // protected member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const DataKey& aKey );

      // protected const member functions

   private:
      // Constructors and destructor
      IRGeometryProxy( const IRGeometryProxy& );

      // assignment operator(s)
      const IRGeometryProxy& operator=( const IRGeometryProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type* m_IRGeometry ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "IRGeom/Template/IRGeometryProxy.cc"
//#endif

#endif /* IRGEOM_IRGEOMETRYPROXY_H */
