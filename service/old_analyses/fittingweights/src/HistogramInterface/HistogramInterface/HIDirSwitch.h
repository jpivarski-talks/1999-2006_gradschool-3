#if !defined(HISTOGRAMINTERFACE_HIDIRSWITCH_H)
#define HISTOGRAMINTERFACE_HIDIRSWITCH_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIDirSwitch
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Tue Dec  1 15:35:36 EST 1998
// $Id: HIDirSwitch.h,v 1.2 1999/09/07 18:53:26 mkl Exp $
//
// Revision history
//
// $Log: HIDirSwitch.h,v $
// Revision 1.2  1999/09/07 18:53:26  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:03  cdj
// first submission
//

// system include files

// user include files

// forward declarations

class HIDirSwitch
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIDirSwitch();
      virtual ~HIDirSwitch();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIDirSwitch( const HIDirSwitch& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIDirSwitch& operator=( const HIDirSwitch& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HISTOGRAMINTERFACE/Template/HIDirSwitch.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIDirSWITCH_H */
