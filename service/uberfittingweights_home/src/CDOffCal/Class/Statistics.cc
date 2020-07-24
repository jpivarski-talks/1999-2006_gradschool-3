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
#include "CDOffCal/Statistics.h"

// STL classes
#include <vector>
#include <map>

static const char* const kFacilityString   = "CDOffCal.Statistics" ;

Statistics::Statistics()
{
  for(int i=0;i<kMaxFlag-1;++i){m_counters[i]=0;}
}
Statistics::~Statistics()
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
void Statistics::counter(int flag, string comment)
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





