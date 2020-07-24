C      Include file geant_dr_hit_def
C
C begin_doc
C
C  $Id: geant_dr_hit_def.inc,v 1.2 2000/08/11 19:51:38 dubrovin Exp $
C
C  $Log: geant_dr_hit_def.inc,v $
C  Revision 1.2  2000/08/11 19:51:38  dubrovin
C  Add new parameter kMaxHitTotal = 5000
C
C  Revision 1.1  1999/10/20 21:19:26  jjo
C  File moved from MCResponseProd library
C
C
C  Documentation for include file geant_dr_hit_def
C
C  Purpose: Define parameters to index into Geant DR hits
C  -------- The function DRWireLayerEVSProxy::buildEVSS in DRWireLayerEVSProxy.cc
C           defines the structure of the Geant hit.  So if that files
C           changes this routine will need to be updated.
C
C  Used in routines:
C  ----------------
C
C  Notes:
C  ------
C
C  Author:   John J O'Neill      Created:  Fri Sep 24 15:49:45 EDT 1999
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
C  COMMON BLOCK NAME should be the same as include file name!
C
C begin_var
C  variables for include file geant_dr_hit_def:
C  -----------------------------------

C----6----------------------------------------------------------------72
C   
C  Variables contained in geant_dr_hit_def common block
C  --------------------------------------------
      integer kX_GEANTHIT,
     &        kY_GEANTHIT,
     &        kZ_GEANTHIT,   
     &        kCX_GEANTHIT,  
     &        kCY_GEANTHIT,  
     &        kCZ_GEANTHIT,  
     &        kP_GEANTHIT,   
     &        kS_GEANTHIT,   
     &        kTOF_GEANTHIT, 
     &        kELOS_GEANTHIT,
     &        kDIST_GEANTHIT,
     &        kWIRE_GEANTHIT,
     &        kSINA_GEANTHIT,
     &        kIOND_GEANTHIT


      parameter (
     &     kX_GEANTHIT=1,  
     &     kY_GEANTHIT=2,  
     &     kZ_GEANTHIT=3,  
     &     kCX_GEANTHIT=4, 
     &     kCY_GEANTHIT=5, 
     &     kCZ_GEANTHIT=6, 
     &     kP_GEANTHIT=7,  
     &     kS_GEANTHIT=8, 
     &     kTOF_GEANTHIT=9,
     &     kELOS_GEANTHIT=10,
     &     kDIST_GEANTHIT=11,
     &     kWIRE_GEANTHIT=12,
     &     kSINA_GEANTHIT=13,
     &     kIOND_GEANTHIT=14 )

C kMaxHit - Max number of Geant hits in Layer
      integer   kMaxHit
      parameter(kMaxHit = 2500)

C D.M. 08/11/00 :
C  kMaxHitTotal - Max number of Geant hits in DR3 
C  for mc_dr_responces.F and fill_geant_drhits.F
C  If one want to change this parameter, pay attention, that
C  you have to change k_maxResponce in MCDRResponceProxy.cc
C  to keep compatible the size of all arrays in C++ and F77.

      integer   kMaxHitTotal
      parameter(kMaxHitTotal = 5000)

C  end_var
C 



