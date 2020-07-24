#if !defined(_ucconstant_FILESOURCEFORMAT__ucconstant_INFO_H)
#define _ucconstant_FILESOURCEFORMAT__ucconstant_INFO_H
// -*- C++ -*-
//
// Package:     _constant_FileSourceFormat
// Module:      _constant_Info
// 
// Description: Used by the ConstantsFileDelivery templated class to
//               create a source that can read _constant_ constants from
//               a file
//
// Usage:
//    <usage>
//
// Author:      Chris D Jones
// Created:     Fri Mar 12 10:14:44 EST 1999
// $Id: _constant_Info.h,v 1.2 1999/10/10 12:58:25 cdj Exp $
//
// Revision history
//
// $Log: _constant_Info.h,v $
// Revision 1.2  1999/10/10 12:58:25  cdj
// forgot to remove BDLclient from skeleton include
//
// Revision 1.1  1999/03/13 22:44:45  cdj
// First submission
//

// system include files

// user include files
#include "DB_constant_.hh"
#include "DataHandler/Stream.h"

// forward declarations
class DummyFriend;

class _constant_Info
{
      friend class DummyFriend; //used to keep the compiler from
                                // issuing a warning

   public:
      typedef DB_constant_ ConstantType;
      
      static const Stream::Type& stream() { return _stream_; }

      static const char* binderTypeName() { return "_lcconstant_file"; }
      static const char* extension() { return "._lcconstant_"; }

   private:
      _constant_Info(); //stop default
      _constant_Info( const _constant_Info& ); // stop default
      const _constant_Info& operator=( const _constant_Info& ); // stop default
};

#endif /* _ucconstant_FILESOURCEFORMAT__ucconstant_INFO_H */
