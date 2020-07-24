#if !defined(DATAHANDLER_PROXYBASE_H)
#define DATAHANDLER_PROXYBASE_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      ProxyBase
// 
// Description: Base class for Proxies held by Record
//
// Usage:
//    <usage>
//
// Author:      Chris D. Jones
// Created:     Thu Sep 24 10:47:33 EDT 1998
// $Id: ProxyBase.h,v 1.1 1998/09/25 19:31:46 cdj Exp $
//
// Revision history
//
// $Log: ProxyBase.h,v $
// Revision 1.1  1998/09/25 19:31:46  cdj
// no longer use ProxyDict
//

// system include files

// user include files

// forward declarations

class ProxyBase
{
   public:
      // ---------- Constructors and destructor ----------------
      ProxyBase();
      virtual ~ProxyBase();

      // ---------- member functions ---------------------------
      void invalidate() {
	 clearCacheIsValid(); invalidateCache(); }
      
      // ---------- const member functions ---------------------
      DABoolean cacheIsValid() const { return m_cacheIsValid; }
      
   protected:

      // ---------- protected member functions -----------------
      virtual void invalidateCache( void ) = 0;
      // indicates that the Proxy should invalidate any cached
      // information as it has "expired"

      void    setCacheIsValid()    { m_cacheIsValid = true; }
      void    clearCacheIsValid()  { m_cacheIsValid = false; }

   private:
      // ---------- Constructors and destructor ----------------
      ProxyBase( const ProxyBase& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ProxyBase& operator=( const ProxyBase& ); // stop default

      // ---------- data members -------------------------------
      DABoolean m_cacheIsValid;

};

// inline function definitions

#endif /* DATAHANDLER_PROXYBASE_H */
