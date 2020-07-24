*
* $Id: hit_to_fit.F,v 1.12 2002/09/05 18:57:02 dpp Exp $
*
* $Log: hit_to_fit.F,v $
* Revision 1.12  2002/09/05 18:57:02  dpp
*      -> now using STRFFT=-1 to designate orphaned hits
*      -> now using STSZFT=-1 to designate orphaned hits
*         this was a long standing error that could cause
*         hits not deleted and layers not searched in replace_z_hits
*
* Revision 1.11  2002/05/23 14:21:48  dpp
*      -> OLD_FIT and OLD_HIT declared integer, not logical
*
* Revision 1.10  2001/11/19 23:57:03  dpp
*      -> add new control parameter include file
*
* Revision 1.9  2001/09/14 21:16:30  dpp
*      -> option to remove fit element,
*         also removes pointer from hit element to fit element
*
* Revision 1.8  2001/09/12 22:03:31  dpp
*      -> option to reset at deweights to 1.
*      -> deweight will now only make a change to lower weight
*
* Revision 1.7  2000/10/24 22:45:45  dpp
*      -> ability to change program indicator
*
* Revision 1.6  2000/10/02 20:45:08  dpp
*      -> set SRCFIT, program indicator, for each hit in fit
*
* Revision 1.5  1999/12/09 21:15:41  dpp
* test input directive using parameters
* fill DWTFIT; deweight the fit
* choose between "new entry" and "update entry" with
*    a directive, not the IFIT value;
* do not change the deweight for "update entry"
*
* Revision 1.4  1999/11/03 22:22:22  dpp
* remove some uneeded common
*
* Revision 1.3  1999/09/12 22:24:59  dpp
* new argument to specify action, sets STRFFT STSZFT=0 for orphaned entries
*
* Revision 1.2  1999/08/03 21:55:35  dpp
* NFIT,HITTRK=0 with arg=0, hit buffer saturation
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE HIT_TO_FIT(ACTION,IHIT,IFITIN)
C.......................................................................
C.
C. HIT_TO_FIT - transfer for HIT arrays (tfhitscd) to FIT arrays (tftrakcd)
C.
C. COMMON    : 
C. CALLS     : 
C. CALLED    : 
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.
C. CREATED   : 28-Dec-97
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/controlpar.inc"
#include "doit/duseq/tfctlcde.inc"
#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"

      INTEGER ACTION,IHIT,IFITIN
      INTEGER IFIT,ILYR
      INTEGER STOPIT
      LOGICAL YESFILL
      INTEGER OLD_FIT,OLD_HIT
      INTEGER ARG2_INT
      REAL ARG2_REAL
      EQUIVALENCE (ARG2_INT,ARG2_REAL)

C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------

C-----------------------------------------------------------------------
C make an entry (new or old) in the FIT list
C-----------------------------------------------------------------------
      IF(ACTION.EQ.H2F_NEW_ENTRY)THEN

C-----------------------------------------------------------------------
C make a new entry in the FIT list
C reset the deweight to =1
C-----------------------------------------------------------------------
         IF(NFIT.LE.KLR2TF)THEN
            NFIT=NFIT+1
            IFIT=NFIT
            DWTFIT(IFIT)=1.
            SRCFIT(IFIT)=PROGRAM_INDICATOR
            YESFILL=.TRUE.
         ELSE
            YESFILL=.FALSE.
            
#if defined(CLEO_DPPDIAG)
            PRINT 9901,IFITIN,NFIT
 9901       FORMAT(' HIT_TO_FIT: OVERFLOW:IFITIN,NFIT=',2I5)
            CALL DSF_RFV_INPUT(STOPIT)
#endif
         ENDIF
C-----------------------------------------------------------------------
C remove an entry specified by the fit number
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.H2F_REMOVE_ENTRY)THEN
         IFIT=IFITIN
         IADFIT(IFIT)=0
         PULFIT(IFIT)=0.
         IHIT=TRKHIT(IFIT)
         IF(IHIT.NE.0)THEN
            IF(HITTRK(IHIT).EQ.IFIT)THEN
               HITTRK(IHIT)=0
            ENDIF
         ENDIF
         YESFILL=.FALSE.
            
C-----------------------------------------------------------------------
C modify an existing entry in the FIT list
C do not change the deweight, source
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.H2F_UPDATE_ENTRY)THEN

         IFIT=IFITIN
         YESFILL=.TRUE.

C-----------------------------------------------------------------------
C We got here supposedly because the calling routine noticed that 
C the current HIT address (indexed by IHIT) was equal to the
C address in the FIT array (indexed by IFIT). BUT, the HIT arrays
C may be changed such that the FIT array points at some other HIT
C which used to be the location of the HIT pointing at this hit.
C None of this is a problem if the hit array contents and order do not change.
C
C [ TRKHIT(old_Fit) left ]
C [behind and may leave  ]
C [r-phi point in S-z fit]------
C [  ZERO THIS           ]      \           {FIT location that        }
C                       ______------------< {previously was pointed to}
C                      /                    {by a hit in the location }
C                     /  _______----------> {of the current HIT       }
C                    /  /
C                   /  /
C                  /  /-------[ HITTRK(NHIT) will be replaced ]
C {           } <--  /
C {           }     /
C {current HIT} >--/                               
C {           } ......(new link)..........  {FIT that has hit address }
C                                 _____---< {of current HIT           }
C                                /          {                         }
C [ TRKHIT(IFIT)       ]        /  ____---> {                         }
C [    will be replaced]------ /  /
C                             /  /
C                            /  /
C {hit location that    } <--  /--[ HITTRK(old_Hit) may eventually be zeroed]
C {previously pointed to}     /   [ but stale data may show up in XTS_TRAK  ]
C {the FIT location now } >--/
C {pointed to by the    }
C {current HIT          }
C
C-----------------------------------------------------------------------
         OLD_FIT=HITTRK(IHIT)
         IF(OLD_FIT.GT.0)THEN
            IF(OLD_FIT.NE.IFIT)THEN
               
               OLD_HIT=TRKHIT(IFIT)
               PRINT 9902,IHIT,OLD_FIT,IFIT,OLD_HIT
 9902          FORMAT(' HIT_TO_FIT: scrambled pointers',
     2                ' HIT=',I3,' HITTRK(hit)=',I3,
     3                ' FIT=',I3,' TRKHIT(fit)=',I3)
#if defined(CLEO_DPPDIAG)
               CALL DSF_RFV_INPUT(STOPIT)
#endif                  
                  
               TRKHIT(OLD_FIT)=0
               STRFFT(OLD_FIT)=-1
               STSZFT(OLD_FIT)=-1
               
            ENDIF
         ENDIF

C-----------------------------------------------------------------------
C change the weight; now will only decrease weight
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.H2F_DEWEIGHT)THEN
         IFIT=IFITIN
         ARG2_INT=IHIT
         IF(1./ARG2_REAL. LT. DWTFIT(IFIT))THEN
            DWTFIT(IFIT)=1./ARG2_REAL
            WGTFIT(IFIT)=WGTFIT(IFIT)*DWTFIT(IFIT)
         ENDIF
         YESFILL=.FALSE.

         
C-----------------------------------------------------------------------
C restore all weights to full weight
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.H2F_FULLWEIGHT)THEN
         DO 57 IFIT=1,NFIT
            DWTFIT(IFIT)=1.
            WGTFIT(IFIT)=WGTFIT(IFIT)*DWTFIT(IFIT)
 57      CONTINUE
         YESFILL=.FALSE.
         
         
C-----------------------------------------------------------------------
C change the program indicator
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.H2F_CHANGE_P_I)THEN
         IFIT=IFITIN
         ARG2_INT=IHIT
         SRCFIT(IFIT)=ARG2_INT
         YESFILL=.FALSE.
         
         
C-----------------------------------------------------------------------
C zero the FIT list
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.H2F_ZERO_FIT)THEN
         NFIT=0
         CALL VZERO(HITTRK,NHITDM)
         YESFILL=.FALSE.

C-----------------------------------------------------------------------
C zero the pointers
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.H2F_ZERO_POINTER)THEN
         CALL VZERO(HITTRK,NHITDM)
         CALL VZERO(TRKHIT,KLR2TF)
         CALL VZERO(STRFFT,KLR2TF)
         CALL VZERO(STSZFT,KLR2TF)
         YESFILL=.FALSE.

C-----------------------------------------------------------------------
C test a pointer, 
C-----------------------------------------------------------------------
      ELSEIF(ACTION.EQ.H2F_TEST_POINTER)THEN
         IF(IHIT.LE.0)THEN
            IF(IFITIN.LE.0)THEN
#if defined(CLEO_DPPDIAG)
               PRINT 9903,IHIT,IFITIN
 9903          FORMAT(' HIT_TO_FIT: invalid input for testing POINTER:',
     2                ' IHIT,IFIT=',2I5)
#endif
            ELSE
               IHIT=TRKHIT(IFITIN)
            ENDIF
         ELSE
            IFITIN=HITTRK(IHIT)
         ENDIF
         YESFILL=.FALSE.

C-----------------------------------------------------------------------
C error
C-----------------------------------------------------------------------
      ELSE
         YESFILL=.FALSE.
#if defined(CLEO_DPPDIAG)
         PRINT 9009,ACTION
 9009    FORMAT(' HIT_TO_FIT: undefined operation: ACTION=',I5)
#endif
      ENDIF

C-----------------------------------------------------------------------
C make the change to the FIT list
C-----------------------------------------------------------------------
      IF(YESFILL)THEN
         IADFIT(IFIT)= IADRHT(IHIT)
         IPLFIT(IFIT)= IPLHIT(IHIT)
         XFIT  (IFIT)= XHIT  (IHIT)
         YFIT  (IFIT)= YHIT  (IHIT)
         SFIT  (IFIT)= SHIT  (IHIT)
         ZFIT  (IFIT)= ZHIT  (IHIT)
         DFIT  (IFIT)= DRFHIT(IHIT)
         MESFIT(IFIT)= DRFHIT(IHIT)
         STRFFT(IFIT)= STRFHT(IHIT)
         STSZFT(IFIT)= STSZHT(IHIT)
         HITTRK(IHIT)= IFIT
         TRKHIT(IFIT)= IHIT
         
         ILYR=         IPLHIT(IHIT)
         WGTFIT(IFIT)= WEIGHT_LAYER(ILYR,SAV_HIT_TYP,SAV_FIT_TYP)
     2                 *DWTFIT(IFIT)
      ENDIF

      RETURN
      END




