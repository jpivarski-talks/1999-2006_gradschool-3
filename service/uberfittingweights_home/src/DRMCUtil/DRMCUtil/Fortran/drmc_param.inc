C      Include file drmc_param
C
C begin_doc
C
C  $Id: drmc_param.inc,v 1.4 2001/01/10 16:39:27 dubrovin Exp $
C
C  $Log: drmc_param.inc,v $
C  Revision 1.4  2001/01/10 16:39:27  dubrovin
C  Set kResolutionsSize = 940 in stead of 2000
C
C  Revision 1.3  1999/10/26 19:04:01  jjo
C  Update to Cleo3 geometry.  Uses ADRGeom instead of hardwired numbers
C
C  Revision 1.2  1999/10/15 01:49:21  jjo
C  Add kNDRLayers
C
C  Revision 1.1  1999/09/20 22:47:19  jjo
C  Import files into cvs
C
C
C  Documentation for include file drmc_param
C
C  Purpose: Parameters for DR MC routines
C  --------
C
C  Used in routines:
C  ----------------
C
C  Notes:
C  ------
C
C  Author:   John J O'Neill      Created:  Mon Sep 13 14:15:58 EDT 1999
C  -------
C
C  Major revisions:
C  ----------------
C  10/21/99 JJO
C    Convertion to cleo3 geometry:
C    Total number of wires 12240 -> 9796
C    Total number of layers   51 -> 47
C
C end_doc
C
C
C  Remember NO NESTED INCLUDES (except for parameters)
C
C
C begin_var
C  variables for include file drmc_param:
C  -----------------------------------

C  ================
C  ANODE PARAMETERS
C  ================
C  kNDRLayers - number of DR layers  
C  kNDRWires  - number of DR anode wires
      integer kNDRLayers, kNDRWires
      parameter ( kNDRLayers = 47, kNDRWires = 9796)


C  kCoseffSize - array size for DRMC Coseff constants
      integer kCoseffSize
      parameter (kCoseffSize = kNDRLayers)

C  kHitJumpSize - array size for DRMC HitJump constants
      integer kHitJumpSize
      parameter (kHitJumpSize = kNDRLayers)

C  kIonsSize - array size for DRMC Ions constants
      integer kIonsSize
      parameter (kIonsSize = kNDRLayers)

C  kLyreffSize - array size for DRMC Lyreff constants
      integer kLyreffSize
      parameter (kLyreffSize = kNDRLayers)

C  kPhlayrSize - array size for DRMC Phlayr constants
C  kNPhLayer_PerLayer - Number of PhLayer constants in each layer
      integer kPhlayrSize, kNPhLayer_PerLayer
      parameter (kNPhLayer_PerLayer=20)
      parameter (kPhlayrSize = kNPhLayer_PerLayer * kNDRLayers )

C  kPhwireSize - array size for DRMC Phwire constants
      integer kPhwireSize
      parameter (kPhwireSize = kNDRWires)

C  kResolutionsSize - array size for DRMCResolutions constants
C     NOT SURE WHERE SIZE COMES FROM: 20 BINS/LAYER = 2000 IN CLEO2 ???
c 01/08/2001 D.M. CLEO III : 20 BINS/LAYER *47 LAYERS = 940
      integer kResolutionsSize
      parameter (kResolutionsSize = 940)

C  kSpeFudSize - array size for DRMCSpefud constants
      integer kSpeFudSize
      parameter (kSpeFudSize = kNDRLayers) 

C  kWireEffSize - array size for DRMCWireff constants
      integer kWireEffSize
      parameter (kWireEffSize =  kNDRWires)


C  ==================
C  CATHODE PARAMETERS 
C  ==================
C  kNDRCathodes - number of DR Cathode layers  
      integer kNDRCathodeLayers
      parameter (kNDRCathodeLayers=1)

C  kEffSize - array size for DRMC Eff constants
      integer kEffSize
      parameter (kEffSize = kNDRCathodeLayers)

C  kSmearSize - array size for DRMCSmear constants
      integer kSmearSize
      parameter (kSmearSize = kNDRCathodeLayers)

C  kTmlayrSize - array size for DRMCTmlayr cosntants
C  REMOVE THIS AS CLEO3 DOESN'T HAVE TIMING INFO FOR CATHODES
      integer kTmlayrSize
      parameter (kTmlayrSize = kNDRCathodeLayers)

C  end_var
