#if !defined(DATAHANDLER_DATAFIELD_H)
#define DATAHANDLER_DATAFIELD_H
// -*- C++ -*-
//
// DataHandler: DataHandler
// Module:      DataField
// 
// Description: Interface to obtain the contents of a DataField
//              THIS CLASS IS JUST FOR HISTORIC BACKWARDS COMPATABILITY
//
//
// Usage:
// This is the interface to the measured or derived quantities that
//    define some part of the detectors state.  The client does not
//    interact with these directly bit uses the ProxyDict mechanism to
//    retrieve the required quantities.  Programmers who wish to make
//    new quantities available do so by providing subclasses of this
//    class.
//
//    The following two methods must be implemented:
//
//    virtual const T* faultHandler( const Record& iRecord ) = 0;
//    virtual void invalidateCache() = 0;
//
// Author:      Simon Patton, Martin Lohner
// Created:     Fri Nov  7 13:35:09 EST 1997
// $Id: DataField.h,v 1.16 2000/06/30 20:57:37 cdj Exp $
//
// Revision history
//
// $Log: DataField.h,v $
// Revision 1.16  2000/06/30 20:57:37  cdj
// removed last dependence on ProxyDict
//
// Revision 1.15  1998/09/25 19:31:40  cdj
// no longer use ProxyDict
//
// Revision 1.14  1998/07/09 14:54:47  cdj
// cxx 6.0 can't handle virtual functions defined in class decleration
//
// Revision 1.13  1998/01/20 21:25:47  mkl
// need key in new faultHandler
//
// Revision 1.12  1998/01/20 20:11:25  mkl
// added new pure virtual function faultHandler with Record signature to shield from ProxyDict mechanism
//
// Revision 1.11  1998/01/13 19:11:32  mkl
// forwarded ProxyDict implementable functions
//
// Revision 1.10  1997/11/19 15:24:45  sjp
// Insulation class, to stop direct connection to ProxyDict
//

// system include files

// user include files
#include "DataHandler/ProxyTemplate.h"
#include "DataHandler/DataKey.h"

// forward declarations

//necessary for backwards compatability
typedef DataKey IfdKey;

template <class T>
class DataField : public ProxyTemplate< T >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs

      // Constructors and destructor
      DataField() {}
      //virtual ~DataField();

      // member functions

      // const member functions

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      DataField( const DataField< T >& ); // stop default

      // assignment operator(s)
      const DataField& operator=( const DataField< T >& ); // stop default

      // private member functions

      // private const member functions

      // data members

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "DataHandler/Template/DataField.cc"
//#endif

#endif /* DATAHANDLER_DATAFIELD_H */
