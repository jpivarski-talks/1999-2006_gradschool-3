#if !defined(DATAHANDLER_PROXYTEMPLATE_H)
#define DATAHANDLER_PROXYTEMPLATE_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      ProxyTemplate
// 
/**\class ProxyTemplate ProxyTemplate.h DataHandler/ProxyTemplate.h

 Description: Interface to obtain the contents of a DataField

 Usage:
 This is the interface to the measured or derived quantities that
    define some part of the detectors state.  The client does not
    interact with these directly bit uses the FrameAccess mechanism to
    retrieve the required quantities.  Programmers who wish to make
    new quantities available do so by providing subclasses of this
    class.

    The following two methods must be implemented:

    virtual const T* faultHandler( const Record& , const DataKey& ) = 0;
    virtual void invalidateCache() = 0;
*/
// Author:      Chris D. Jones
// Created:     Thu Sep 24 10:47:55 EDT 1998
// $Id: ProxyTemplate.h,v 1.2 1999/12/10 14:12:14 cdj Exp $
//
// Revision history
//
// $Log: ProxyTemplate.h,v $
// Revision 1.2  1999/12/10 14:12:14  cdj
// added throwing exceptions when Proxy's faultHandler returns 0
//
// Revision 1.1  1998/09/25 19:31:49  cdj
// no longer use ProxyDict
//

// system include files

// user include files
#include "DataHandler/ProxyBase.h"

// forward declarations
class Record;
class DataKey;

template< class T > 
class ProxyTemplate : public ProxyBase
{
   public:
      // ---------- constants, enums and typedefs --------------
      typedef T value_type;

      // ---------- Constructors and destructor ----------------
      virtual ~ProxyTemplate();

      // ---------- const member functions ---------------------
      virtual const T* get( const Record& iRecord,  
			    const DataKey& iKey ) const ;

   protected:
      ProxyTemplate();

      // ---------- protected member functions -----------------
      virtual void invalidateCache( void ) = 0;
      virtual const T* faultHandler( const Record& iRecord , 
				     const DataKey& iKey )=0;

      /** override this function if you want a 0 return value from 
	   faultHandler to throw different exception then 
	   DHProxyErrorException<T>
       */
      virtual void throwFaultHandlerException( const Record& iRecord,
					       const DataKey& iKey ) const;
   private:
      // ---------- Constructors and destructor ----------------
      ProxyTemplate( const ProxyTemplate<T>& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ProxyTemplate& operator=( const ProxyTemplate<T>& ); // stop default

      // ---------- data members -------------------------------
      const T* m_cache;

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "DataHandler/Template/ProxyTemplate.cc"
#endif

#endif /* DATAHANDLER_PROXYTEMPLATE_H */
