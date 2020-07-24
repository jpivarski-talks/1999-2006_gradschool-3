#if !defined(DATAHANDLER_DATAKEYTAGS_H)
#define DATAHANDLER_DATAKEYTAGS_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      DataKeyTags
// 
// Description: The set of Tags used by DataKey
//
// Usage:
//    These Tags are used by DataKey
//
// Author:      Chris D. Jones
// Created:     Wed Sep 23 16:41:09 EDT 1998
// $Id: DataKeyTags.h,v 1.2 1998/09/25 22:11:44 cdj Exp $
//
// Revision history
//
// $Log: DataKeyTags.h,v $
// Revision 1.2  1998/09/25 22:11:44  cdj
// can now get back the value
//
// Revision 1.1  1998/09/25 19:31:43  cdj
// no longer use ProxyDict
//

// system include files

// user include files
#include "HeteroContainer/HCTypeTag.h"

// forward declarations
class DataKey;

typedef HCTypeTag<DataKey> TypeTag;

class SimpleStringTag {
   public:
      SimpleStringTag( const char* iString ) : m_string(iString ) {}
      SimpleStringTag() : m_string("") {}
      DABoolean operator==( const SimpleStringTag& iRHS ) const ;
      DABoolean operator<( const SimpleStringTag& iRHS ) const ;

      const char* value() const { return m_string; }

   private:
      const char* m_string;
};

class UsageTag : public SimpleStringTag {
   public:
      UsageTag( const char* iUsage ) : SimpleStringTag(iUsage) {}
      UsageTag() : SimpleStringTag() {}
};

class ProductionTag : public SimpleStringTag {
   public:
      ProductionTag( const char* iUsage ) : SimpleStringTag(iUsage) {}
      ProductionTag() : SimpleStringTag() {}
};

class IdTags {
   public:
      IdTags() {};
      IdTags( const UsageTag& iUsage, 
	      const ProductionTag& iProduction ) :
	 m_usage( iUsage), m_production( iProduction ) {}
      
      const UsageTag& usage() const { return m_usage; }
      const ProductionTag& production() const { return m_production;}
      
   private:
      UsageTag m_usage;
      ProductionTag m_production;
};


// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DataHandler/Template/DataKeyTags.cc"
//#endif

#endif /* DATAHANDLER_DATAKEYTAGS_H */
