*
* $Id: svzfil.inc,v 1.2 1996/09/30 05:48:13 jjo Exp $
*
* $Log: svzfil.inc,v $
* Revision 1.2  1996/09/30 05:48:13  jjo
* Modify svgeompa.inc to use longer variable names per CLEO3 standards
*        SVNSTR -> NSTRIP_SV3, SVLYRS -> NLAYER_SV3, SVMWFR -> NWAFER_SV3
*        NSVWP? -> NWAF_PHI?_SV3, NSVWZ? -> NWAF_Z?_SV3
*        SV_CHAN_ACTIVE  -> NCHAN_ACTIVE_SV3, SV_CHAN_TOTAL  -> NCHAN_TOTAL_SV3
*
* Revision 1.1.1.1  1995/11/01 21:18:38  lkg
* Import renamed CLEO3 include repository: cl3seq
*
* Revision 1.1.1.1  1994/10/04  20:59:05  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.02/58 21/08/94  14.45.16  by  Dan Payne
*-- Author : Dan Payne
***************** Parameters for SVX ZFILES constants stuff *************
*                                                                       *
*      CxxxSV : Character path name for entity xxx                      *
*                                                                       *
*      MxxxSV : Number of words expected for entity xxx                 *
*                                                                       *
*      AxxxSV : Access mode for entity xxx                              *
*                                                                       *
*      UxxxSV : User superkeys for entity xxx                           *
*                                                                       *
*  NOTES: 1) Intended for use with the sequences:                       *
*            //SEQ/ZFILES/ZFGUGPSQ, ZFGUGP used for accessing ZFILES    *
*            entities.                                                  *
*         2) Requires sequence //SEQ/SVSEQ/SVGEOMPA in the declarations *
*            part of the routine.                                       *
*                                                                       *
*************************************************************************
C
C Alignment constants
C
      CHARACTER*(*) CSO1SV, CSO2SV, CSO3SV, CSD1SV, CSD2SV, CSD3SV,
     $              COO1SV, COO2SV, COO3SV, COD1SV, COD2SV, COD3SV,
     $              CWO1SV, CWO2SV, CWO3SV, CWD1SV, CWD2SV, CWD3SV

      PARAMETER (   CSO1SV='/CLEO/SV/GEOM/POSITIONS/SIDEV.OFF1',
     $              CSO2SV='/CLEO/SV/GEOM/POSITIONS/SIDEV.OFF2',
     $              CSO3SV='/CLEO/SV/GEOM/POSITIONS/SIDEV.OFF3',
     $              CSD1SV='/CLEO/SV/GEOM/POSITIONS/SIDEV.DIR1',
     $              CSD2SV='/CLEO/SV/GEOM/POSITIONS/SIDEV.DIR2',
     $              CSD3SV='/CLEO/SV/GEOM/POSITIONS/SIDEV.DIR3',
     $              COO1SV='/CLEO/SV/GEOM/POSITIONS/OCTANT.OFF1',
     $              COO2SV='/CLEO/SV/GEOM/POSITIONS/OCTANT.OFF2',
     $              COO3SV='/CLEO/SV/GEOM/POSITIONS/OCTANT.OFF3',
     $              COD1SV='/CLEO/SV/GEOM/POSITIONS/OCTANT.DIR1',
     $              COD2SV='/CLEO/SV/GEOM/POSITIONS/OCTANT.DIR2',
     $              COD3SV='/CLEO/SV/GEOM/POSITIONS/OCTANT.DIR3',
     $              CWO1SV='/CLEO/SV/GEOM/POSITIONS/WAFER.OFF1',
     $              CWO2SV='/CLEO/SV/GEOM/POSITIONS/WAFER.OFF2',
     $              CWO3SV='/CLEO/SV/GEOM/POSITIONS/WAFER.OFF3',
     $              CWD1SV='/CLEO/SV/GEOM/POSITIONS/WAFER.DIR1',
     $              CWD2SV='/CLEO/SV/GEOM/POSITIONS/WAFER.DIR2',
     $              CWD3SV='/CLEO/SV/GEOM/POSITIONS/WAFER.DIR3'  )

      INTEGER       MSO1SV, MSO2SV, MSO3SV, MSD1SV, MSD2SV, MSD3SV,
     $              MOO1SV, MOO2SV, MOO3SV, MOD1SV, MOD2SV, MOD3SV,
     $              MWO1SV, MWO2SV, MWO3SV, MWD1SV, MWD2SV, MWD3SV

      PARAMETER(MSO1SV = 1,      MSO2SV = 1,      MSO3SV = 1,
     $          MSD1SV = 1,      MSD2SV = 1,      MSD3SV = 1,
     $          MOO1SV = SVSIDS, MOO2SV = SVSIDS, MOO3SV = SVSIDS,
     $          MOD1SV = SVSIDS, MOD2SV = SVSIDS, MOD3SV = SVSIDS,
     $          MWO1SV=NWAFER_SV3, MWO2SV=NWAFER_SV3, MWO3SV=NWAFER_SV3,
     $          MWD1SV=NWAFER_SV3, MWD2SV=NWAFER_SV3, MWD3SV=NWAFER_SV3)

      CHARACTER*(*) ASO1SV, ASO2SV, ASO3SV, ASD1SV, ASD2SV, ASD3SV,
     $              AOO1SV, AOO2SV, AOO3SV, AOD1SV, AOD2SV, AOD3SV,
     $              AWO1SV, AWO2SV, AWO3SV, AWD1SV, AWD2SV, AWD3SV

      PARAMETER (   ASO1SV = 'BR', ASO2SV = 'BR', ASO3SV = 'BR',
     $              ASD1SV = 'BR', ASD2SV = 'BR', ASD3SV = 'BR',
     $              AOO1SV = 'BR', AOO2SV = 'BR', AOO3SV = 'BR',
     $              AOD1SV = 'BR', AOD2SV = 'BR', AOD3SV = 'BR',
     $              AWO1SV = 'BR', AWO2SV = 'BR', AWO3SV = 'BR',
     $              AWD1SV = 'BR', AWD2SV = 'BR', AWD3SV = 'BR' )

      INTEGER       USO1SV, USO2SV, USO3SV, USD1SV, USD2SV, USD3SV,
     $              UOO1SV, UOO2SV, UOO3SV, UOD1SV, UOD2SV, UOD3SV,
     $              UWO1SV, UWO2SV, UWO3SV, UWD1SV, UWD2SV, UWD3SV

      PARAMETER (   USO1SV = 0, USO2SV = 0, USO3SV = 0,
     $              USD1SV = 0, USD2SV = 0, USD3SV = 0,
     $              UOO1SV = 0, UOO2SV = 0, UOO3SV = 0,
     $              UOD1SV = 0, UOD2SV = 0, UOD3SV = 0,
     $              UWO1SV = 0, UWO2SV = 0, UWO3SV = 0,
     $              UWD1SV = 0, UWD2SV = 0, UWD3SV = 0 )
C
C Calibration constants
C

C Pedestals, Pedestal widths, Gains, Channel status

      CHARACTER*(*) CPEDSV, CWIDSV, CSLPSV, CBADSV
      PARAMETER (   CPEDSV='/CLEO/SV/CALIB/PULSER.PED',
     $              CWIDSV='/CLEO/SV/CALIB/PULSER.WIDTH',
     $              CSLPSV='/CLEO/SV/CALIB/PULSER.SLOPE',
     $              CBADSV='/CLEO/SV/CALIB/PULSER.BAD' )

      INTEGER       MPEDSV, MWIDSV, MSLPSV, MBADSV
      PARAMETER (   MPEDSV = SVTCAN, MWIDSV = SVTCAN,
     $              MSLPSV = SVTCAN, MBADSV = SVTCAN )

      CHARACTER*(*) APEDSV, AWIDSV, ASLPSV, ABADSV
      PARAMETER (   APEDSV = 'BR', AWIDSV = 'BR',
     $              ASLPSV = 'BR', ABADSV = 'BR' )

      INTEGER       UPEDSV,     UWIDSV,     USLPSV,     UBADSV
      PARAMETER (   UPEDSV = 0, UWIDSV = 0, USLPSV = 0, UBADSV = 0 )

C Common-mode-noise (CMN) (Mean and width for each CAMEX)

      CHARACTER*(*) CMCMSV, CWCMSV
      PARAMETER (   CMCMSV='/CLEO/SV/CALIB/CMN.MEAN',
     $              CWCMSV='/CLEO/SV/CALIB/CMN.WIDTH' )

      INTEGER       MMCMSV, MWCMSV
      PARAMETER (   MMCMSV = SVMCMX, MWCMSV = SVMCMX )

      CHARACTER*(*) AMCMSV, AWCMSV
      PARAMETER (   AMCMSV = 'BR', AWCMSV = 'BR' )

      INTEGER       UMCMSV,     UWCMSV
      PARAMETER (   UMCMSV = 0, UWCMSV = 0 )

C Thresholds for sparsification

      CHARACTER*(*) CTHRSV
      PARAMETER (   CTHRSV='/CLEO/SV/CALIB/THRESHOLDS.THRESH' )

      INTEGER       MTHRSV
      PARAMETER (   MTHRSV = SVTCAN )

      CHARACTER*(*) ATHRSV
      PARAMETER (   ATHRSV = 'BR' )

      INTEGER       UTHRSV
      PARAMETER (   UTHRSV = 0 )
C
C Logical Address (LA) --> Electronic Address (EA) map
C
      CHARACTER*(*) CCRTSV, CCHNSV
      PARAMETER (   CCRTSV='/CLEO/SV/LA_TO_EA_MAP.CRATE',
     $              CCHNSV='/CLEO/SV/LA_TO_EA_MAP.CHAN' )

      INTEGER       MCRTSV, MCHNSV
      PARAMETER (   MCRTSV = SVTCAN, MCHNSV = SVTCAN )

      CHARACTER*(*) ACRTSV, ACHNSV
      PARAMETER (   ACRTSV = 'BR', ACHNSV = 'BR' )

      INTEGER       UCRTSV,     UCHNSV
      PARAMETER (   UCRTSV = 0, UCHNSV = 0 )





