#if !defined(CDOFFCAL_STATISTICS_H)
#define CDOFFCAL_STATISTICS_H

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#endif

#include <vector>
#include <map>

#include "STLUtility/fwd_vector.h"
#include "STLUtility/fwd_map.h"

class Statistics
{
public:
  Statistics();
  ~Statistics();
  void counter (int flag, string comment);
private:
  enum { kMaxFlag = 201,
	 kMaxCommentLength = 100 };
  int m_counters[kMaxFlag];
  char m_comments[kMaxFlag][kMaxCommentLength];  
};
#endif





