#if !defined(CONSTANTSDELIVERY_CONSTANTSSTREAMMAP_H)
#define CONSTANTSDELIVERY_CONSTANTSSTREAMMAP_H
// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      ConstantsStreamMap
// 
/**\class ConstantsStreamMap ConstantsStreamMap.h ConstantsDelivery/ConstantsStreamMap.h

 Description: Name space for functions that relate Constant types to Streams

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Apr 12 15:28:03 EDT 2000
// $Id: ConstantsStreamMap.h,v 1.2 2000/06/20 18:41:02 cdj Exp $
//
// Revision history
//
// $Log: ConstantsStreamMap.h,v $
// Revision 1.2  2000/06/20 18:41:02  cdj
// when using Online MetaVersion tag, use pregenerated list of Constants types
//
// Revision 1.1  2000/04/14 20:52:30  cdj
// now looks up what constants are in the MetaVersion tag
//

// system include files
#include <vector>
#include <string>

// user include files
#include "DataHandler/Stream.h"

// forward declarations
#include "STLUtility/fwd_vector.h"
class DummyFriend;

class ConstantsStreamMap
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      friend class DummyFriend;

      // ---------- Constructors and destructor ----------------
       //virtual ~ConstantsStreamMap();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------
      static Stream::Type streamWithConstant( const string& iConstantName );

      static STL_VECTOR(string) constantsInStream( const Stream::Type& iStream );

      static const STL_VECTOR(string)& onlineConstants();

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
     ConstantsStreamMap();
     ConstantsStreamMap( const ConstantsStreamMap& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const ConstantsStreamMap& operator=( const ConstantsStreamMap& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "ConstantsDelivery/Template/ConstantsStreamMap.cc"
//#endif

#endif /* CONSTANTSDELIVERY_CONSTANTSSTREAMMAP_H */
