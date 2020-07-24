#if !defined(NAVIGATION_NAVRICHSERVER_MACRO_H)
#define NAVIGATION_NAVRICHSERVER_MACRO_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavRichServer_Macro
// 
// Description: code generating macro for NavRichServer
//
// Usage: Only by NavRichServer.cc
//
// Author:      Tomasz Skwarnicki
// Created:     Fri Jul 20 17:28:56 EDT 2000
// $Id: NavRichServer_macro.h,v 1.1 2000/07/27 01:32:06 ts Exp $
//
// Revision history
//
// $Log: NavRichServer_macro.h,v $
// Revision 1.1  2000/07/27 01:32:06  ts
// NavRich added
//
//
#include "C3Rich/RichTypes.h"

#define NAVRICHSER( Q , Z , Y ) \
FAItem< Q > NavRichServer::Z(UInt32 aId ) const { if( !Y.valid() ){ extract( m_record, const_cast< NavRichServer* >( this )->Y ) ; }\
if( Y.valid() ){  return FAItem< Q >( &Y [aId] ); } else { return FAItem< Q >( 0 );} }

#endif /* NAVIGATION_NAVRICHSERVER_MACRO_H */

