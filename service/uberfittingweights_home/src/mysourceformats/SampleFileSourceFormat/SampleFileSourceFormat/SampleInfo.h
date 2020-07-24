#if !defined(REPLACEUPPERCASEFILESOURCEFORMAT_REPLACEUPPERCASEINFO_H)
#define REPLACEUPPERCASEFILESOURCEFORMAT_REPLACEUPPERCASEINFO_H
// -*- C++ -*-
//
// Package:     REPLACEMIXEDCASEFileSourceFormat
// Module:      REPLACEMIXEDCASEInfo
// 
// Description: Used by the ConstantsFileDelivery templated class to
//               create a source that can read REPLACEMIXEDCASE constants from
//               a file
//
// Usage:
//    <usage>
//
// Author:      REPLACEUSERNAME
// Created:     REPLACETIMEB
// $Id: REPLACEMIXEDCASEInfo.h,v 1.0 REPLACETIMEA REPLACEUSERID Exp $
//
// Revision history
//
// $Log: REPLACEMIXEDCASEInfo.h,v $

// system include files

// user include files
#include "DBREPLACEMIXEDCASE.hh"

// Choose your stream
REPLACEWITHLISTOFSTREAMS

// forward declarations
class DummyFriend;

class REPLACEMIXEDCASEInfo
{
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      typedef DBREPLACEMIXEDCASE ConstantType;
      
      static const Stream::Type& stream() { return /* which stream? */; }

      static const char* binderTypeName() { return "REPLACELOWERCASEfile"; }
      static const char* extension() { return ".REPLACELOWERCASE"; }

   private:
      REPLACEMIXEDCASEInfo(); //stop default
      REPLACEMIXEDCASEInfo( const REPLACEMIXEDCASEInfo& ); // stop default
      const REPLACEMIXEDCASEInfo& operator=( const REPLACEMIXEDCASEInfo& ); // stop default
};

#endif /* REPLACEUPPERCASEFILESOURCEFORMAT_REPLACEUPPERCASEINFO_H */
