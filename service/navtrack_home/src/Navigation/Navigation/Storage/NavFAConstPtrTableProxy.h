#if !defined(NAVIGATION_NAVFACONSTPTRTABLEPROXY_H)
#define NAVIGATION_NAVFACONSTPTRTABLEPROXY_H
// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      NavFAConstPtrTableProxy
// 
/**\class NavFAConstPtrTableProxy NavFAConstPtrTableProxy.h Navigation/NavFAConstPtrTableProxy.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Sat Aug 30 16:03:04 EDT 2003
// $Id: NavFAConstPtrTableProxy.h,v 1.1 2003/08/31 23:25:35 cdj Exp $
//
// Revision history
//
// $Log: NavFAConstPtrTableProxy.h,v $
// Revision 1.1  2003/08/31 23:25:35  cdj
// added the ability to store NavTracks
//

// system include files

// user include files
#include "StorageManagement/SMFAPtrTableStorageHelper.h"
#include "StorageManagement/SMFAPtrTableProxy.h"
#include "DAException/DAException.h"
#include "FrameAccess/FAConstPtrTable.h"

// forward declarations
template <class T>
class NavFAConstPtrTableProxy : public SMFAPtrTableProxy<T>
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      NavFAConstPtrTableProxy(SMFAPtrTableStorageHelper<T>* iHelper):
	 SMFAPtrTableProxy<T>(iHelper) {}

      // ---------- member functions ---------------------------
      const FAPtrTable<T>* faultHandler(const Record& iRecord,
					const DataKey& iKey ) {
	 if( iKey.usage().value()[0]=='\0' &&
	     iKey.production().value()[0]=='\0') {
	    throw DAException("Can not get master Navigation table from storage.  Add NavigationProd to your job.");
	 }
	 if( 0 == data() ) {
	    data() = new FAConstPtrTable<T>();
	 }
	 return SMFAPtrTableProxy<T>::faultHandler(iRecord, iKey);
      }
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      NavFAConstPtrTableProxy( const NavFAConstPtrTableProxy& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const NavFAConstPtrTableProxy& operator=( const NavFAConstPtrTableProxy& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "Navigation/Template/NavFAConstPtrTableProxy.cc"
//#endif

#endif /* NAVIGATION_NAVFACONSTPTRTABLEPROXY_H */
