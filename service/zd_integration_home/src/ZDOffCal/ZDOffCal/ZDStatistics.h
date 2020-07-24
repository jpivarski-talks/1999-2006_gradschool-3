// Author:      Guangshun Huang
// Created:     Thu Jan 9 2003
// $Id: ZDStatistics.h,v 1.1 2003/04/24 18:08:18 huanggs Exp $
//
// Revision history
//
// $Log: ZDStatistics.h,v $
// Revision 1.1  2003/04/24 18:08:18  huanggs
// add new files
//
// Revision 1.2  2003/04/24 14:51:59  huanggs
// ZD calsses
//
// Revision 1.1.1.1  2003/02/03 20:11:58  huanggs
// imported source
//
#if !defined(ZDOFFCAL_ZDSTATISTICS_H)
#define ZDOFFCAL_ZDSTATISTICS_H

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif

#include <vector>
#include <map>

#include "STLUtility/fwd_vector.h"
#include "STLUtility/fwd_map.h"

class ZDStatistics
{
public:
  ZDStatistics();
  ~ZDStatistics();
  void counter (int flag, string comment);
private:
  enum { kMaxFlag = 201,
	 kMaxCommentLength = 100 };
  int m_counters[kMaxFlag];
  char m_comments[kMaxFlag][kMaxCommentLength];  
};
#endif
