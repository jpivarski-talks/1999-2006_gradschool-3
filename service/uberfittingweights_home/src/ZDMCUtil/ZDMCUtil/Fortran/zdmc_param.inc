C      Include file zdmc_param
C
C begin_doc
C
C  Documentation for include file zdmc_param
C
C  Purpose: Parameters for ZD MC routines
C  --------
C
C  Used in routines:
C  ----------------
C
C  Notes:
C  ------
C
C  Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
C  -------
C
C  Major revisions:
C  ----------------
C
C end_doc
C
C
C  Remember NO NESTED INCLUDES (except for parameters)
C
C
C begin_var
C  variables for include file zdmc_param:
C  -----------------------------------

C  ================
C  ANODE PARAMETERS
C  ================
C  kNZDLayers - number of ZD layers  
C  kNZDWires  - number of ZD anode wires
      integer kNZDLayers, kNZDWires
      parameter ( kNZDLayers = 6, kNZDWires = 300)


C  kCoseffSize - array size for ZDMC Coseff constants
      integer kCoseffSize
      parameter (kCoseffSize = kNZDLayers)

C  kHitJumpSize - array size for ZDMC HitJump constants
      integer kHitJumpSize
      parameter (kHitJumpSize = kNZDLayers)

C  kIonsSize - array size for ZDMC Ions constants
      integer kIonsSize
      parameter (kIonsSize = kNZDLayers)

C  kLyreffSize - array size for ZDMC Lyreff constants
      integer kLyreffSize
      parameter (kLyreffSize = kNZDLayers)

C  kPhlayrSize - array size for ZDMC Phlayr constants
C  kNPhLayer_PerLayer - Number of PhLayer constants in each layer
      integer kPhlayrSize, kNPhLayer_PerLayer
      parameter (kNPhLayer_PerLayer=20)
      parameter (kPhlayrSize = kNPhLayer_PerLayer * kNZDLayers )

C  kPhwireSize - array size for ZDMC Phwire constants
      integer kPhwireSize
      parameter (kPhwireSize = kNZDWires)

C  kResolutionsSize - array size for ZDMCResolutions constants
C     NOT SURE WHERE SIZE COMES FROM: 20 BINS/LAYER = 2000 IN CLEO2 ???
c 01/08/2001 D.M. CLEO III : 20 BINS/LAYER *47 LAYERS = 940
* 03/28/2003 hgs: this will be 120 for the ZD
      integer kResolutionsSize
      parameter (kResolutionsSize = 120)

C  kSpeFudSize - array size for ZDMCSpefud constants
      integer kSpeFudSize
      parameter (kSpeFudSize = kNZDLayers) 

C  kWireEffSize - array size for ZDMCWireff constants
      integer kWireEffSize
      parameter (kWireEffSize =  kNZDWires)


C  ==================
C  CATHODE PARAMETERS 
C  ==================
C  kNZDCathodes - number of ZD Cathode layers  
      integer kNZDCathodeLayers
      parameter (kNZDCathodeLayers=1)

C  kEffSize - array size for ZDMC Eff constants
      integer kEffSize
      parameter (kEffSize = kNZDCathodeLayers)

C  kSmearSize - array size for ZDMCSmear constants
      integer kSmearSize
      parameter (kSmearSize = kNZDCathodeLayers)

C  kTmlayrSize - array size for ZDMCTmlayr cosntants
C  REMOVE THIS AS CLEO3 DOESN'T HAVE TIMING INFO FOR CATHODES
      integer kTmlayrSize
      parameter (kTmlayrSize = kNZDCathodeLayers)

C  end_var
