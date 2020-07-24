#if !defined(SIGEOM_SIALIGNEDGEOMETRYPROXY_H)
#define SIGEOM_SIALIGNEDGEOMETRYPROXY_H
// -*- C++ -*-
//
// Package:     SiAlignedGeom
// Module:      SiAlignedGeometryProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//

// system include files

// user include files
//#include "DataHandler/ProxyTemplate.h"
#include "DataHandler/DataField.h"
//#include "DataHandler/ProxyTemplate.h"
#include "DetectorGeometry/DGDetectorStore.h"
#include "DetectorGeometry/DGPlacedVolumePtr.h"
#include "DataHandler/SyncValue.h"

// forward declarations
class SiGeom;

class SiAlignedGeometryProxy : public DataField< DGDetectorStore >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef DGDetectorStore value_type;

      // Constructors and destructor
      SiAlignedGeometryProxy();
//      SiAlignedGeometryProxy();
      virtual ~SiAlignedGeometryProxy();

      // member functions
      SmallCount clamshell(SmallCount ladder) const;
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
      SiAlignedGeometryProxy( const SiAlignedGeometryProxy& );

      // assignment operator(s)
      const SiAlignedGeometryProxy& operator=( const SiAlignedGeometryProxy& );

      // private member functions

      // private const member functions

      // data members
      value_type m_detectorStore ;
      SyncValue m_baseGeometrySyncValue ;
//      const SiGeom& m_producer;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "package/Template/SiAlignedGeometryProxy.cc"
//#endif

#endif /* SIGEOM_SIALIGNEDGEOMETRYPROXY_H */


