#if !defined(CONSTANTSDELIVERY_CDNOSERVERFOUNDEXCEPTION_H)
#define CONSTANTSDELIVERY_CDNOSERVERFOUNDEXCEPTION_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      CDNoServerFoundException
// 
/**\class CDNoServerFoundException CDNoServerFoundException.h ConstantsDelivery/CDNoServerFoundException.h

 Description: Exception thrown when unable to connect to a constant's server

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Mon Jun 12 14:22:05 EDT 2000
// $Id: CDNoServerFoundException.h,v 1.2 2000/06/18 17:53:36 cdj Exp $
//
// Revision history
//
// $Log: CDNoServerFoundException.h,v $
// Revision 1.2  2000/06/18 17:53:36  cdj
// added copy constructor for CDNoServerFoundException needed by OSF
//
// Revision 1.1  2000/06/17 19:21:51  cdj
// first submission
//

// system include files
#include <string>

// user include files
#include "DAException/DAException.h"

// forward declarations

class CDNoServerFoundException : public DAExceptionBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      CDNoServerFoundException(const string& iConstantType ) 
	 : m_message( string("No server found for ") + iConstantType ) {}
      //virtual ~CDNoServerFoundException();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      virtual const char* what () const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      //CDNoServerFoundException( const CDNoServerFoundException& ); // allow default

      // ---------- assignment operator(s) ---------------------
      //const CDNoServerFoundException& operator=( const CDNoServerFoundException& ); // allow default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      string m_message;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsDelivery/Template/CDNoServerFoundException.cc"
//#endif

#endif /* CONSTANTSDELIVERY_CDNOSERVERFOUNDEXCEPTION_H */
