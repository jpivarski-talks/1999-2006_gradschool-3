#if !defined(HISTOGRAMVIEWER_HVCOMMUNICATE_H)
#define HISTOGRAMVIEWER_HVCOMMUNICATE_H
// -*- C++ -*-
//
// Package:     <HistogramViewer>
// Module:      HVCommunicate
// 
/**\class HVCommunicate HVCommunicate.h HistogramViewer/HVCommunicate.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Tue May 14 15:11:05 EDT 2002
// $Id$
//
// Revision history
//
// $Log$

// system include files

// user include files

// forward declarations

class HVCommunicate
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVCommunicate();
      virtual ~HVCommunicate();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

      static double pos_thresh;
      static double neg_thresh;

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVCommunicate( const HVCommunicate& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVCommunicate& operator=( const HVCommunicate& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramViewer/Template/HVCommunicate.cc"
//#endif

#endif /* HISTOGRAMVIEWER_HVCOMMUNICATE_H */
