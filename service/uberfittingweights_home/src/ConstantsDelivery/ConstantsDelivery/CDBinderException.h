#if !defined(CONSTANTSDELIVERY_CDBINDEREXCEPTION_H)
#define CONSTANTSDELIVERY_CDBINDEREXCEPTION_H
// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      CDBinderException
// 
/**\class CDBinderException CDBinderException.h ConstantsDelivery/CDBinderException.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Thu Apr 13 09:54:30 EDT 2000
// $Id: CDBinderException.h,v 1.1 2000/04/14 20:52:29 cdj Exp $
//
// Revision history
//
// $Log: CDBinderException.h,v $
// Revision 1.1  2000/04/14 20:52:29  cdj
// now looks up what constants are in the MetaVersion tag
//

// system include files

// user include files
#include "DAException/DAException.h"

// forward declarations

class CDBinderException : public DAException
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      CDBinderException( const char* iMessage ) :
	 DAException( iMessage ) {}
      //virtual ~CDBinderException();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //CDBinderException( const CDBinderException& ); // stop default

      // ---------- assignment operator(s) ---------------------
      //const CDBinderException& operator=( const CDBinderException& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsDelivery/Template/CDBinderException.cc"
//#endif

#endif /* CONSTANTSDELIVERY_CDBINDEREXCEPTION_H */
