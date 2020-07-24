// -*- C++ -*-
//
// Package:     <ZDOffCal>
// Module:      ZDStatistics
//
// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDStatistics.cc,v 1.1 2003/04/24 18:08:12 huanggs Exp $
//
// Revision history
//
// $Log: ZDStatistics.cc,v $
// Revision 1.1  2003/04/24 18:08:12  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:52  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:57  huanggs
// imported source
//

#include "Experiment/Experiment.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// system include files
#include <stdlib.h>   // for ::exit
#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "ZDOffCal/ZDStatistics.h"

// STL classes
#include <vector>
#include <map>

static const char* const kFacilityString   = "ZDOffCal.ZDStatistics" ;

ZDStatistics::ZDStatistics()
{
  for(int i=0;i<kMaxFlag-1;++i){m_counters[i]=0;}
}
ZDStatistics::~ZDStatistics()
{
for(int i=0;i<kMaxFlag-1;++i)
  {
     if ( m_counters[i] > 0 ) 
       {
  	report( INFO, kFacilityString ) 
	  << i << " " << m_counters[i] << " - "  
	  << m_comments[i] << endl ;
       }
  }
}
void ZDStatistics::counter(int flag, string comment)
{
  if ( 0 <= flag && flag < kMaxFlag-1 )
    {
      if ( m_counters[flag] == 0 ) 
	{
	  comment.copy(m_comments[flag],kMaxCommentLength,0) ;
	}
      ++m_counters[flag];
    }
}
