#if !defined(HISTOGRAMINTERFACE_HINUMBERWITHERRORS_H)
#define HISTOGRAMINTERFACE_HINUMBERWITHERRORS_H
// -*- C++ -*-
//
// Package:     HistogramInterface
// Module:      HINumberWithErrors
// 
// Description: This class inherits from the HINumber class and adds error
// 		fields.
//
// Usage:
//    <usage>
//
// Author:      Klaus Honscheid
// Created:     
// $Id: HINumberWithErrors.h,v 1.3 1999/10/07 18:52:42 hufnagel Exp $
//
// Revision history
//
// $Log: HINumberWithErrors.h,v $
// Revision 1.3  1999/10/07 18:52:42  hufnagel
// added name method for Numbers
//
// Revision 1.2  1999/10/07 18:05:07  hufnagel
// Numbers now use float instead of double
//
// Revision 1.1  1999/09/07 18:53:33  mkl
// new histogram interface
//
//

// system include files
#include <string>

// user include files

// forward declarations

class HINumberWithErrors
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HINumberWithErrors( const string&  iName,
			  float         iValue=0,
			  float         iPositiveError=0,
			  float         iNegativeError=0 );
      virtual ~HINumberWithErrors();

      // ---------- member functions ---------------------------
      // Modifiers
      virtual void update( float iEntry,
			   float iPosErr=0,
			   float iNegErr=0 );
      virtual void reset();

      // Accessors
      float value() const { return m_value; }
      float positiveError() const { return m_positiveError; }
      float negativeError() const { return m_negativeError; }
      const string& name() const { return m_name; }
      DABoolean isInitialized() const { return m_isInitialized; }

      // ---------- const member functions ---------------------
   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HINumberWithErrors( const HINumberWithErrors& ); // stop default
      HINumberWithErrors(); // stop default
      // ---------- assignment operator(s) ---------------------
      const HINumberWithErrors& operator=( const HINumberWithErrors& ); 
      // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      string m_name;
      float m_value;
      float m_positiveError;
      float m_negativeError;
      DABoolean m_isInitialized;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HINumberWithErrors.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HINUMBERWITHERRORS_H */
