#if !defined(HBOOKHISTOGRAM_HBOOKDIRCONTENTS_H)
#define HBOOKHISTOGRAM_HBOOKDIRCONTENTS_H
// -*- C++ -*-
//
// Package:     HbookHistogram
// Module:      HbookDirContents
// 
// Description: Abstract base class for Hbook classes that are held in
//              an HbookHistDir
//
// Usage:
//    This class is used to guarantee that the HBook directory is properly
//     set before filling the object that inherits from this
//
// Author:      Chris D Jones
// Created:     Mon Sep 27 17:56:22 EDT 1999
// $Id: HbookDirContents.h,v 1.1 1999/09/29 17:55:52 cdj Exp $
//
// Revision history
//
// $Log: HbookDirContents.h,v $
// Revision 1.1  1999/09/29 17:55:52  cdj
// added histogram directories
//

// system include files

// user include files
#include "HbookHistogram/HbookHistDir.h"

// forward declarations

class HbookDirContents
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HbookDirContents( HbookHistDir* iParent ) : m_parent( iParent ) {}
      //virtual ~HbookDirContents();

      // ---------- member functions ---------------------------
      void setDir() { m_parent->makeDirDefault() ; }

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HbookDirContents( const HbookDirContents& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HbookDirContents& operator=( const HbookDirContents& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      HbookHistDir* m_parent;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogram/Template/HbookDirContents.cc"
//#endif

#endif /* HBOOKHISTOGRAM_HBOOKDIRCONTENTS_H */
