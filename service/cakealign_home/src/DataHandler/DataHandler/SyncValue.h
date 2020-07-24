#if !defined(DATAHANDLER_SYNCVALUE_H)
#define DATAHANDLER_SYNCVALUE_H
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      SyncValue
// 
/**\class SyncValue SyncValue.h DataHandler/SyncValue.h

 Description: Designates the position of a Record in the DataStream.

 Usage:
    DataSourceControllers use SyncValues to synchronize a data stream.

    Requirmement: monotonically increasing over the lifetime of the 
                  experiment


    NOTE: Although time is designed in the interface, it is not actually
          implemented yet.

*/
//
// Author:      Chris D. Jones
// Created:     Sun Mar 16 16:52:32 EST 1997
// $Id: SyncValue.h,v 1.18 2002/08/16 14:32:39 cleo3 Exp $
//
// Revision history
//
// $Log: SyncValue.h,v $
// Revision 1.18  2002/08/16 14:32:39  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.17  1999/10/09 21:51:16  cdj
// updated to doxygen style comments
//
// Revision 1.16  1999/04/09 17:26:55  mkl
// fix output operator
//
// Revision 1.15  1999/01/21 15:27:12  mkl
// include iostream directly rather than just forward declaring ostream
//
// Revision 1.14  1999/01/19 23:30:15  mkl
// added ostream operator
//
// Revision 1.13  1998/06/24 17:14:24  cdj
// added operator> and operator>=
//
// Revision 1.12  1998/03/26 19:40:36  mkl
// switched to UInt64/UInt32 for time/run-,event numbers; include time for comparison operators
//
// Revision 1.11  1997/11/19 15:16:13  sjp
// Added kBeginningOfTime constant
//
// Revision 1.10  1997/09/18 21:10:37  cdj
// changed Boolean to DABoolean
//
// Revision 1.9  1997/08/30 19:01:11  cdj
// kMaxValue is now a static const unsigned long (needed by HP compiler)
//
// Revision 1.8  1997/07/09 18:00:07  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.7  1997/06/29 20:12:58  cdj
// Added const SyncValue kUndefined
//
// Revision 1.6  1997/05/30 17:06:50  cdj
// added the accessor methods runNumber(), eventNumber(), and time()
//
// Revision 1.5  1997/05/06 20:25:35  mkl
// got rid of run(), event(), time() methods
//
// Revision 1.4  1997/05/06 18:34:59  mkl
// added run(), event(), and time() return functions
//
// Revision 1.3  1997/04/01 21:00:34  cdj
// Added operator!= and simplified some of the other operators
//
// Revision 1.2  1997/03/18 16:17:17  cdj
// Constructor now has default arguements for all its arguements.
//
// Revision 1.1  1997/03/17 19:26:54  cdj
// First submission
//


// system include files
#include "C++Std/iostream.h"

// user include files
#include "Experiment/types.h"

// forward declarations

class SyncValue
{
      // friend classses and functions
      friend ostream& operator<< ( ostream& os, const SyncValue& iSyncValue );

   public:
      // constants, enums and typedefs
      static const UInt64 kMaxTimeValue;
      static const UInt32 kMaxRunEventValue;

      static const SyncValue kUndefined; //Uses the default values
      static const SyncValue kBeginningOfTime;

      // Constructors and destructor
      SyncValue( UInt32 iRun = 0,
		 UInt32 iEvent = 0,
		 UInt64 iTime = 0 );

      virtual ~SyncValue();

      // member functions

      // const member functions
      DABoolean operator< ( const SyncValue& iRHS ) const;
      DABoolean operator<=( const SyncValue& iRHS ) const;
      DABoolean operator==( const SyncValue& iRHS ) const;
      DABoolean operator!=( const SyncValue& iRHS ) const;
      DABoolean operator> ( const SyncValue& iRHS ) const {
	 return ( iRHS < *this ); }
      DABoolean operator>=( const SyncValue& iRHS ) const {
	 return ( iRHS <= *this ); }

      UInt32 runNumber( void ) const;
      UInt32 eventNumber( void ) const;
      UInt64 time( void ) const;

      // static member functions

   protected:
      // protected member functions

      // protected const member functions

   private:
      // Constructors and destructor
      //SyncValue( const SyncValue& );

      // assignment operator(s)
      //const SyncValue& operator=( const SyncValue& );

      // private member functions

      // private const member functions

      // data members
      UInt32 m_run, m_event;
      UInt64 m_time;

      // static data members

};

// inline function definitions
inline 
ostream& 
operator<< ( ostream& os, const SyncValue& iSyncValue )
{
   os << "run/event/time=" 
      << iSyncValue.runNumber() << "/" 
      << iSyncValue.eventNumber() << "/" 
      << iSyncValue.time();
   return os;
}

#endif /* DATAHANDLER_SYNCVALUE_H */
