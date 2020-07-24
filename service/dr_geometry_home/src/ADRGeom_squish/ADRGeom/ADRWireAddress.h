#if !defined(ADRGEOM_ADRWIREADDRESS_H)
#define ADRGEOM_ADRWIREADDRESS_H
// -*- C++ -*-
//
// Package:     ADRGeom
// Module:      ADRWireAddress
// 
// Description: Contains the information necessary to find a wire in the DR
//  
// Usage:
//              Used by ADRSenseWireStore
//
// Author:      Inga Karliner/ copied from Chris D. Jones DRWireExample
// Created:     9/21/98
//
// Revision history
//     
//
// system include files

// user include files

// forward declarations

class ADRWireAddress
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      ADRWireAddress( unsigned int iLayerID, unsigned int iWireID );
      virtual ~ADRWireAddress() {}

      // member functions
      void setLayerID( unsigned int iID );
      void setWireID( unsigned int iID );

      // const member functions
      unsigned int layerID() const;
      unsigned int wireID() const;

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      ADRWireAddress( const ADRWireAddress& );

      // assignment operator(s)
      const ADRWireAddress& operator=( const ADRWireAddress& );

      // private member functions

      // private const member functions

      // data members
      unsigned int m_layerID;
      unsigned int m_wireID;

      // static data members

};

// inline function definitions
inline
ADRWireAddress::ADRWireAddress( unsigned int iLayerID,
				unsigned int iWireID ) :
   m_layerID(iLayerID),
   m_wireID(iWireID)
{}

inline
void 
ADRWireAddress::setLayerID( unsigned int iID )
{ m_layerID = iID; }

inline
void 
ADRWireAddress::setWireID( unsigned int iID )
{ m_wireID = iID; }

inline
unsigned int 
ADRWireAddress::layerID() const
{ return m_layerID; }

inline
unsigned int 
ADRWireAddress::wireID() const
{ return m_wireID; }

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ADRGeom/Template/ADRWireAddress.cc"
//#endif

#endif /* ADRGEOM_ADRWIREADDRESS_H */
