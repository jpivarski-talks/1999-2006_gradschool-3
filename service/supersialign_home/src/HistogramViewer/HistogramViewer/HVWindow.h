#if !defined(PACKAGE_HVWINDOW_H)
#define PACKAGE_HVWINDOW_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      HVWindow
// 
/**\class HVWindow HVWindow.h package/HVWindow.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Sat Jul 14 14:24:54 EDT 2001
// $Id: HVWindow.h,v 1.1.1.1 2001/07/30 14:11:32 cdj Exp $
//
// Revision history
//
// $Log: HVWindow.h,v $
// Revision 1.1.1.1  2001/07/30 14:11:32  cdj
// importing
//

// system include files

// user include files
#include <qwidget.h>

// forward declarations
class QListView;
class QWorkspace;

class HVWindow : public QWidget
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HVWindow(const HIHistogramManager* iManager, QWidget* iParent=0 );
      virtual ~HVWindow();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HVWindow( const HVWindow& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HVWindow& operator=( const HVWindow& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/HVWindow.cc"
//#endif

#endif /* PACKAGE_HVWINDOW_H */
