#if !defined(RUNSTATISTICSSOURCEMODULE_INFOXANDSTREAM_H)
#define RUNSTATISTICSSOURCEMODULE_INFOXANDSTREAM_H
// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      infoXandStream
// 
/**\function infoX2StreamType infoXandStream.h RunStatisticsSourceModule/infoXandStream.h

 Description: converts from InfoX::Type to Stream::Type

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Wed Jan 26 15:45:28 EST 2000
// $Id: infoXandStream.h,v 1.2 2000/05/08 19:52:14 cdj Exp $
//
// Revision history
//
// $Log: infoXandStream.h,v $
// Revision 1.2  2000/05/08 19:52:14  cdj
// now compiles on OSF with MICO
//
// Revision 1.1.1.1  2000/01/29 20:47:36  cdj
// imported RunStatisticsSourceModule source
//

// system include files

// user include files
#include "DataHandler/Stream.h"
#include "CLEORB.hxx" //defines CLEORB_CLIENT_HH
#include CLEORB_CLIENT_HH(InfoX)

// forward declarations
const Stream::Type& infoX2StreamType( const InfoX::Type& iType );

InfoX::Type stream2InfoXType( const Stream::Type& iType );


#endif /* RUNSTATISTICSSOURCEMODULE_INFOXANDSTREAM_H */
