#if !defined(CHISQFITPRODUCER_CHISQTRACKFITTERPROXY_H)
#define CHISQFITPRODUCER_CHISQTRACKFITTERPROXY_H
// -*- C++ -*-
//
// Package:     <ChisqFitProducer>
// Module:      ChisqTrackFitterProxy
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Sat Jun 12 14:59:27 EDT 1999
// $Id: ChisqTrackFitterProxy.h,v 1.4 2000/01/11 20:18:37 wsun Exp $
//
// Revision history
//
// $Log: ChisqTrackFitterProxy.h,v $
// Revision 1.4  2000/01/11 20:18:37  wsun
// Replaced control flags with a pointer to ChisqFitProducer.
//
// Revision 1.3  1999/07/07 16:45:09  wsun
// Added mechanism for applying constraints in track fitter.
//
// Revision 1.2  1999/06/24 22:56:26  wsun
// Added use of HIMagField.
//
// Revision 1.1.1.1  1999/06/16 02:48:25  wsun
// First submission.
//
//

// system include files

// user include files
#include "ProxyBind/ProxyBindableTemplate.h"
#include "ChisqFitter/ChisqTrackFitter.h"

// forward declarations
class ChisqFitProducer ;

class ChisqTrackFitterProxy : public ProxyBindableTemplate< ChisqTrackFitter >
{
      // friend classes and functions

   public:
      // constants, enums and typedefs
      typedef ChisqTrackFitter value_type;

      // Constructors and destructor
      ChisqTrackFitterProxy( ChisqFitProducer* aChisqFitProducer = 0 );
      virtual ~ChisqTrackFitterProxy();

      // member functions

      // This function gets bound to the beginrun stream.
      void updateMagField( const Record& iRecord ) ;

      // use this form of a function when calling 'bind'
      // void boundMethod( const Record& iRecord );

      // const member functions

      // static member functions

   protected:
      // protected member functions
      virtual void invalidateCache() ;
      virtual const value_type* faultHandler( const Record& aRecord,
					      const DataKey& aKey );

      // protected const member functions

   private:
      // Constructors and destructor
      ChisqTrackFitterProxy( const ChisqTrackFitterProxy& );

      // assignment operator(s)
      const ChisqTrackFitterProxy& operator=( const ChisqTrackFitterProxy& );

      // private member functions

      //this function has already been written for you
      void bind(
		void (ChisqTrackFitterProxy::*iMethod)( const Record& ),
		const Stream::Type& iStream );

      // private const member functions

      // data members
      value_type* m_ChisqTrackFitter ;

      ChisqFitProducer* m_chisqFitProducer ;

      // static data members

};

// inline function definitions

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "ChisqFitProducer/Template/ChisqTrackFitterProxy.cc"
//#endif

#endif /* CHISQFITPRODUCER_CHISQTRACKFITTERPROXY_H */
