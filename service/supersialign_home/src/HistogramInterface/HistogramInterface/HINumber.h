#if !defined(HISTOGRAMINTERFACE_HINUMBER_H)
#define HISTOGRAMINTERFACE_HINUMBER_H
// -*- C++ -*-
//
// Package:     HistogramInterface
// Module:      HINumber
// 
// Description: This class serves as the base class for any (IM) number.
//
// Usage:
//    <usage>
//
// Author:      Klaus Honscheid
// Created:     
// $Id: HINumber.h,v 1.3 1999/10/07 18:52:41 hufnagel Exp $
//
// Revision history
//
// $Log: HINumber.h,v $
// Revision 1.3  1999/10/07 18:52:41  hufnagel
// added name method for Numbers
//
// Revision 1.2  1999/10/07 18:05:07  hufnagel
// Numbers now use float instead of double
//
// Revision 1.1  1999/09/07 18:53:32  mkl
// new histogram interface
//
//

// system include files
#include <string>

// user include files

// forward declarations

class HINumber 
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HINumber(	const string&  iName,
		float         iValue = 0 );
      virtual ~HINumber();
      // ---------- member functions ---------------------------
      // Modifiers
      virtual void update( float iEntry );
      virtual void reset();	
      
      // Accessors
      float value() const { return m_value; }
      const string& name() const { return m_name; }
      DABoolean isInitialized() const { return m_isInitialized; }
      
      // ---------- const member functions ---------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HINumber( const HINumber& ); // stop default
      HINumber(); // stop default
      // ---------- assignment operator(s) ---------------------
      const HINumber& operator=( const HINumber& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      string m_name;
      float  m_value;
      DABoolean m_isInitialized;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HINumber.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HINUMBER_H */
