// -*- C++ -*-
#if !defined(ZDALIGNMOD_ZDALIGNMOD_H)
#define ZDALIGNMOD_ZDALIGNMOD_H
//
// Package:     ZDAlignMod
// Module:      ZDAlignMod
//
/**\class ZDAlignMod ZDAlignMod.h ZDAlignMod/ZDAlignMod.h
 
 Description: Suez Module that allows you to control the "event" loop

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu Oct 23 09:37:30 EDT 2003
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "CommandPattern/Module.h"
#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
//The data object you wish to change on each iteration
//#include "CleoDB/DBRunHeader.h"  

// forward declarations

class ZDAlignMod : public FrameIteratorModuleBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      ZDAlignMod();
      virtual ~ZDAlignMod();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      ///override this function to do the actual iterations
      virtual void iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd );

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      ZDAlignMod( const ZDAlignMod& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ZDAlignMod& operator=( const ZDAlignMod& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      //This is the object you change every iteration
      //DBRunHeader m_header;
      //Holds the value and passes it onto the Proxy
      //FIHolder<DBRunHeader> m_headerHolder;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ZDAlignMod/Template/ZDAlignMod.cc"
//#endif

#endif /* ZDALIGNMOD_ZDALIGNMOD_H */
