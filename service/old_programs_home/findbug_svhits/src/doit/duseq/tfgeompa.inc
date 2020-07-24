*
* $Id: tfgeompa.inc,v 1.8 2001/09/14 21:19:02 dpp Exp $
*
* $Log: tfgeompa.inc,v $
* Revision 1.8  2001/09/14 21:19:02  dpp
*      -> add directive to remove fit entry
*
* Revision 1.7  2001/09/12 22:09:20  dpp
*      -> add directive to restore hits in fit to full weight
*      -> expand use of program indicator
*
* Revision 1.6  2000/10/24 22:43:20  dpp
*      -> add HIT_TO_FIT directive for changing program indicator
*
* Revision 1.5  2000/08/18 18:34:10  dpp
*     -> add number of new DOIT scatters
*
* Revision 1.4  1999/12/09 21:03:03  dpp
* add parameters for directives to hit_to_fit
*  add directive to deweight hit
*
* Revision 1.3  1999/11/17 21:42:28  dpp
* add USE_ACCESS to control WIRES_2_HITS counter
*
* Revision 1.2  1999/03/10 15:52:22  lkg
* Remove spurious "X" introduced in front of one of the comment lines
*
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
*
* PARAMETERS Defining Duet geometry.
*
#if !defined(CLEO_NOCOMDOC)

*V2.00 KCANTF and KCATTF replace NCANTF and NCATTF
*Because of this, NLYRTF, NLR2TF and NGYMTF became KLYRTF, KLR2TF and KGYMTF
*And NLYRCF, NLYRLF became KLYRCF and KLYRLF

* KCANTF - Max. Number of TFIND anode   layers
* KCATTF - Max. Number of TFIND cathode layers
* KLYRTF - Max. Number of TFIND total   layers
*
* KLR2TF - Twice the max. total number of layers
*
* NSCATF - Maximum number of discreet   scattering planes
* NSCCTF - Maximum number of continuous scattering media

* M_SCAT - Maximum number of new DOIT scatters

* NLVLTYP - number of level type, ie pat-rec, TFXFIT; for hit finding controls
* NHITTYP - number of hit type, ie WIRE, HIT;  for hit finding controls
* NFITTYP - number of fit type, ie CF, LF;  for hit finding controls
*
* USE_PAT_REC      - control of hit finding LVLTYP: pattern rec requested
* USE_TFXFIT       - control of hit finding LVLTYP: TFXFIT requested
* USE_ACCESS       - control of hit finding LVLTYP: access internals
* USE_WIRE         - control of hit finding HITTYP: WIREs are requested
* USE_HIT          - control of hit finding HITTYP: HITs are requested
* USE_CF           - control of hit finding FITTYP: CF use
* USE_LF           - control of hit finding FITTYP: LF use
* H2F_ZERO_FIT     - control of HIT_TO_FIT: zero the fit and HITTRK
* H2F_ZERO_POINTER - control of HIT_TO_FIT: zero HITTRK and TRKFIT
* H2F_TEST_POINTER - control of HIT_TO_FIT: test either HITTRK and TRKFIT
* H2F_NEW_ENTRY    - control of HIT_TO_FIT: make new entry in fit
* H2F_UPDATE_ENTRY - control of HIT_TO_FIT: update existing entry in fit
* H2F_DEWEIGHT     - control of HIT_TO_FIT: deweight existing entry in fit
* H2F_FULLWEIGHT   - control of HIT_TO_FIT: reset all entries to full weight
* H2F_CHANGE_P_I   - control of HIT_TO_FIT: change program indicator
* H2F_REMOVE_ENTRY - control of HIT_TO_FIT: remove , by fit index
* KABG2_CLEAR      - KABG2 - clear all kinks
* KABG2_PHI        - KABG2 - create kink in phi
* KABG2_CURV       - KABG2 - create kink in curvature
* KABG2_ELOSS      - KABG2 - create energy loss
* KABG2_CALC       - KABG2 - calculate transformation
* KABG2_FLOAT      - KABG2 - kink variable floats in fit
* KABG2_FIX        - KABG2 - kink variable is fixed 
* KABG2_XI_RECALC  - KABG2 - recalculate XI 
* KABG2_XI_OK      - KABG2 - XI is OK, do not recalculate 
* FT_DEL_BAD       - FAST_TRAK_DEL_BAD_AND_ADD - delete bad hits
* FT_NO_DELETE     - FAST_TRAK_DEL_BAD_AND_ADD - do not delete bad hits
* FT_USE_CELL_FRAC - FAST_TRAK_DEL_BAD_AND_ADD - use cell fraction roads
* FT_USE_SIGMAS    - FAST_TRAK_DEL_BAD_AND_ADD - use layer sigma roads

#endif

      INTEGER    KCANTF         , KCATTF
      PARAMETER( KCANTF = KCANCD, KCATTF = KCATCD )
      INTEGER    KLYRTF                , KLR2TF
      PARAMETER( KLYRTF = KCANTF+KCATTF, KLR2TF = 2*(KLYRTF+KQDVCD) )
      INTEGER    KGYMTF
      PARAMETER( KGYMTF =(KLR2TF*(KLR2TF+1))/2 )
      INTEGER    KLYRCF         ,   KLYRLF
      PARAMETER( KLYRCF = KCAXCD,   KLYRLF = KCSTCD+KCATCD)


      INTEGER    NSCATF         , NSCCTF
      PARAMETER( NSCATF = 4, NSCCTF = 4 )
      INTEGER    M_SCAT
      PARAMETER( M_SCAT = 10 )

      INTEGER    NLVLTYP    , NHITTYP    , NFITTYP
      PARAMETER( NLVLTYP=  2, NHITTYP=  2, NFITTYP=  2 )

      INTEGER   USE_PAT_REC   , USE_TFXFIT   , USE_ACCESS
      PARAMETER(USE_PAT_REC= 1, USE_TFXFIT=2 , USE_ACCESS=3 )

      INTEGER   USE_WIRE   , USE_HIT
      PARAMETER(USE_WIRE= 1, USE_HIT=  2)
      INTEGER   USE_CF     , USE_LF
      PARAMETER(USE_CF= 1  , USE_LF= 2 )

      INTEGER H2F_ZERO_FIT,H2F_ZERO_POINTER,H2F_TEST_POINTER
      INTEGER H2F_NEW_ENTRY,H2F_UPDATE_ENTRY,H2F_REMOVE_ENTRY
      INTEGER H2F_DEWEIGHT,H2F_FULLWEIGHT
      INTEGER H2F_CHANGE_P_I

      PARAMETER (H2F_ZERO_FIT    = 0)
      PARAMETER (H2F_ZERO_POINTER= 1)
      PARAMETER (H2F_TEST_POINTER= 2)
      PARAMETER (H2F_NEW_ENTRY   = 5)
      PARAMETER (H2F_UPDATE_ENTRY= 6)
      PARAMETER (H2F_DEWEIGHT    = 7)
      PARAMETER (H2F_CHANGE_P_I  = 8)
      PARAMETER (H2F_FULLWEIGHT  = 9)
      PARAMETER (H2F_REMOVE_ENTRY=10)

      INTEGER 
     + 	KABG2_CLEAR ,KABG2_PHI   ,KABG2_CURV   ,KABG2_ELOSS ,
     +  KABG2_CALC  ,KABG2_FLOAT ,KABG2_FIX    ,
     +  KABG2_XI_RECALC          ,KABG2_XI_OK         
      PARAMETER (KABG2_CLEAR     = 1)
      PARAMETER (KABG2_PHI       = 2)
      PARAMETER (KABG2_CURV      = 3)
      PARAMETER (KABG2_ELOSS     = 4)
      PARAMETER (KABG2_CALC      = 5)
      PARAMETER (KABG2_FLOAT     = 6)
      PARAMETER (KABG2_FIX       = 7)
      PARAMETER (KABG2_XI_RECALC = 8)
      PARAMETER (KABG2_XI_OK     = 9)

      INTEGER 
     +  FT_DEL_BAD       ,FT_NO_DELETE    ,
     +  FT_USE_CELL_FRAC ,FT_USE_SIGMAS
      PARAMETER (FT_DEL_BAD      =31)
      PARAMETER (FT_NO_DELETE    =32)
      PARAMETER (FT_USE_CELL_FRAC=33)
      PARAMETER (FT_USE_SIGMAS   =34)
  

* ----------------------------------------------------------









