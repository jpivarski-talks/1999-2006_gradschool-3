#include "Experiment/Experiment.h"

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/TRSeedTrack.h"

// STL includes
#include <vector>

#include "FrameAccess/instantiate_macros.h" 
INSTANTIATE_MANY( TRSeedTrack ) 

#include "FrameAccess/instantiate_many.h"

#include "TrackRoot/Storage/ManyTRSeedTrackStorageHelper.h"
  //#include "TrackRoot/TRSeedTrackStorageHelper.h"
SM_REGISTER_STORAGE(ManyTRSeedTrackStorageHelper)
  //SM_REGISTER_STORAGE(TRSeedTrackStorageHelper)
