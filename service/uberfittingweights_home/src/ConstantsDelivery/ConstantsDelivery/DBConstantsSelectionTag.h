#if !defined(CONSTANTSDELIVERY_DBCONSTANTSSELECTIONTAG_H)
#define CONSTANTSDELIVERY_DBCONSTANTSSELECTIONTAG_H
// -*- C++ -*-
//
// Package:     ConstantsDelivery
// Module:      DBConstantsSelectionTag
// 
// Description: Holds selection information needed by DBConstants
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Tue Aug 18 14:09:28 EDT 1998
// $Id: DBConstantsSelectionTag.h,v 1.7 2000/04/27 21:14:03 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsSelectionTag.h,v $
// Revision 1.7  2000/04/27 21:14:03  cdj
// now works on OSF using MICO ORB
//
// Revision 1.6  1998/11/24 20:12:55  baker
// Change normal constructor for ConstantsSourceBinder
// to allow specification of the desired Federated
// Database.
// Added a Constructor for DBConstantsRecordInfo
// that takes a single string variable.
//
// Revision 1.5  1998/11/24 15:09:33  baker
// Remove CLEORB and CLEORBDB from include paths
// for generated header files
//
// Revision 1.4  1998/11/05 18:52:10  baker
// Another batch of updates to get this library finally compiled
// *AND* linkable
//
// Revision 1.3  1998/11/04 15:39:22  baker
// Continuing updates to get this library compiled
//
// Revision 1.2  1998/09/30 20:40:19  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:29:02  baker
// // September 30, Rich Baker
// // Renamed header files from former DataBaseDelivery
// // for new ConstantsDelivery library
//
// Revision 1.1  1998/08/19 03:17:39  cdj
// first submission
//

// system include files
#include <string>
#include <vector>

// user include files
#include "STLUtility/fwd_vector.h"
// 11/24/98 Rich Baker
// Remove "CLEORB" from include path for generated CORBA headers
#include "CLEORB.hxx" //defines CLEORB_CLIENT_HH
#include CLEORB_CLIENT_HH(DBVersion)

// forward declarations
#include "C++Std/fwd_string.h"

class DBConstantsSelectionTag
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DBConstantsSelectionTag(const STL_VECTOR(string)& iSelTags,
		     const DBVersion::SelectType iType = DBVersion::Ignore );
// 11/24/98 Rich Baker
// Add new constructor that takes only a single string instead
// of requiring a vector of strings
      DBConstantsSelectionTag(const string& iSelTags,
		     const DBVersion::SelectType iType = DBVersion::Ignore );
      DBConstantsSelectionTag( const DBConstantsSelectionTag& rhs );

      virtual ~DBConstantsSelectionTag();

      // ---------- member functions ---------------------------
      const DBConstantsSelectionTag& operator=( 
	    const DBConstantsSelectionTag& rhs );
      // ---------- const member functions ---------------------
      const STL_VECTOR( string )& selectionTags() const;

      DBVersion::SelectType type() const;

      string toString() const;
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DBConstantsSelectionTag();  // Stop default
      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      STL_VECTOR(string)    m_selTags;
      DBVersion::SelectType m_type;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsDelivery/Template/DBConstantsSelectionTag.cc"
//#endif

#endif /* CONSTANTSDELIVERY_DBCONSTANTSSELECTIONTAG_H */
