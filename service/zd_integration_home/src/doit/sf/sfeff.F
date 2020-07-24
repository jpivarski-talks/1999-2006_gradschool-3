*
* $Id: sfeff.F,v 1.7 2003/03/05 18:18:11 dpp Exp $
*
* $Log: sfeff.F,v $
* Revision 1.7  2003/03/05 18:18:11  dpp
*      -> new assignments for negative INUCHN
*      -> changed all variable names in cdscrtcd to have common root
*
*  -> HEY! sfeff has non-buffered access to EvntHit_MapDet
*         look for anything starting with "EvntHit_"
*
* Revision 1.6  2002/09/05 18:48:11  dpp
*      -> remove local ICHAIN
*
* Revision 1.5  2001/08/23 14:40:58  dpp
* just leave the name, return and end without sfdiag
*
* Revision 1.4  2001/08/21 14:08:41  dpp
*      -> move toward F77
*      -> changes for CLEO3
*      -> remove sfdiag switch that had blanked out entire code
*
* Revision 1.3  1998/07/09 01:42:09  lyon
* Retrofitted for sf_data_access.inc
*
* Revision 1.2  1998/05/08 14:14:08  pg
*   Replaced "seq/zfiles/*.inc" with "cl3seq/zfiles/*.inc".
*
* Revision 1.1.1.1  1998/02/06 19:11:46  dpp
* DOIT first release.
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  6.00/07 31/08/93  14.40.47  by  Rob Kutschke
*CMZ :  5.54/15 13/11/91  12.20.40  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.33  by  Dan Peterson
*CMZ :  2.00/00 08/05/89  15.59.03  by  Dan Peterson
*-- Author :
 2    SUBROUTINE SFEFF
C......................................................................
C.
C. SFEFF  -  calculate efficiencies from SF chains
C.
C. COMMON    : /STEPCn/
C. CALLS     :
C. CALLED    : SFIND
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 20-Jul-88
C. LAST MOD  : 06-Oct-88
C.
C. Modification Log.
C......................................................................

#if defined(CLEO_SFDIAG)

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

      SAVE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------
#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"

C...... NAMES
      CHARACTER*6 CRNAME,CRNAM1,CRNAM2,CRNAM3
      PARAMETER (CRNAM1='SFEFF1')
      PARAMETER (CRNAM2='SFEFF2')
      PARAMETER (CRNAM3='SFEFF3')

C...... LOCAL VARIABLE DOCUMENTAION
C HCNTSF...COUNTERS FOR EFFECIENCY
C          1 HIT IS TRIED
C          2 HIT IS TRIED AND FOUND
C          3 HIT IS TRIED AND FOUND,  BUT WITH LOW PULSE HEIGHT
C          4 HIT IS TRIED AND FOUND,  AND IS A DOUBLE HIT
C          5 HIT IS NOT TRIED BUT FOUND ON, NOISE
C IADR.....SF ADDRESS
C SVADR....SV ADDRESS
C ILAY.....AXIAL LAYER NUMBER
C IWIR.....WIRE NUMBER
C J........LOOP INDEX
C
C IY.......RESPONSE TO CONTINUE DISPLAYING LAYER EFFICIENCIES
C ILYR......SF LAYER NUMBER
C ILCD......CD LAYER NUMBER
C ILDR......DR LAYER NUMBER
C ISUM1.....SUM OF TRYS
C ISUM2.....SUM OF HITS
C ISUM3.....SUM OF HITS WITH LOW PULSE HEIGHT
C ISUM4.....SUM OF DOUBLES
C ISUM5.....SUM OF NOISE
C ISM1ZD...SUM OF TRYS IN ZD
C ISM2ZD...SUM OF HITS IN ZD
C ISM3ZD...SUM OF HITS WITH LOW PULSE HEIGHT IN ZD
C ISM4ZD...SUM OF DOUB IN ZD
C ISM1DR...SUM OF TRYS IN DR
C ISM2DR...SUM OF HITS IN DR
C ISM3DR...SUM OF HITS WITH LOW PULSE HEIGHT IN DR
C ISM4DR...SUM OF DOUB IN DR
C FRAC2......EFFECIENCY OF LAYER
C FRAC3......FRACTION OF HITS WITH LOW PULSE HEIGHT
C FRAC4......FRACTION OF DOUBLE HITS
C FRAC5......FRACTION OF NOISE
C LADR.....BEGINNING ADR
C MADR.....MAXIMUM ADR
C IUNITE...UNIT NUMBER
C KILLA....FLAG TO TURN OFF QUESTIONS
C YESPRINT.FLAG TO PRINT
C TDCVAL...RAW TDC VALUE
C PSVAL....TIME IN PICOSECONDS
C IFIRST...FLAG FOR FIRST EXE
C ILSAVE...DR LAYERS TO SAVE HITS
C IHSAVE...HITS SAVED ON LAYERS
C TDCSAV...TDCVAL OF SAVED HITS
C DRFSAV...SAVED DRIFT DISTANCE
C IWSAVE...WIRE NUMBER OF SAVED HITS
C DISSUM...DISTANCE SUM
C IDISSM...INTEGER OF DISSUM
C HISNUM...CALCULATED HISTOGRAM NUMBER
C IWIRDF...WIRE NUMBER DIFFERENCE
C COUNTR...COUNTER FOR NUMBER OF TIMES CALLED
C CNT_WAIT.NUMBER OF ENTRIES TO WAT FOR NEXT PRINTOUT
C CNT_NEXT.COUNTR TOTAL FOR NEXT PRINTOUT
C CON_COND.CONTINUE CONDITION
      INTEGER HCNTSF(5,MHITSF)
      INTEGER IADR,SVADR,IH
      INTEGER ILAY,IWIR,J

      INTEGER IY,ILYR,TDCVAL
      REAL    PSVAL
      INTEGER ILCD,ILDR
      INTEGER ISUM1,ISUM2,ISUM3,ISUM4,ISUM5
      INTEGER ISM1ZD,ISM2ZD,ISM3ZD,ISM4ZD
      INTEGER ISM1DR,ISM2DR,ISM3DR,ISM4DR
 
      REAL FRAC2,FRAC3,FRAC4,FRAC5
      INTEGER LADR,MADR
      INTEGER IUNITE
      LOGICAL KILLA,YESPRINT
      INTEGER IHSAVE(3),TDCSAV(3),IWSAVE(3)
      INTEGER IDISSM,HISNUM,IWIRDF,NWIRL
      REAL DRFSAV(3),DISSUM
      LOGICAL CONT_COND

C dr layer
      INTEGER ILSAVE(3),OFFSSV
      DATA ILSAVE/13,14,15/
      DATA OFFSSV/-8/

      DATA KILLA/.FALSE./

      INTEGER COUNTR,CNT_WAIT,CNT_NEXT
      DATA COUNTR,CNT_NEXT/0,0/

      Real PsSave(3),EntInr
      real HUnits
C----------------------Executable code starts here---------------------

      ENTRY SFEFF1
      CRNAME=CRNAM1
      COUNTR=COUNTR+1

C record status of the hits: lookedfor, found, double, noise
      CALL VZERO(IHSAVE,3)

C loop over the chains
      IF(NCHAIN.LT.1)RETURN

      DO 175 ICHAIN=1,NCHAIN

C cut on chain length, start and stop, note DR is SF layers 17 to 56
         IF(LENCHN(ICHAIN).GE.20)THEN

C Starting layer requitement
            ILAY=SFLSFA(LINCHN(ICHAIN))
            IF(ILAY.LT.7)THEN
C Ending layer requirement
               ILAY=SFLSFA(MINCHN(ICHAIN))
               IF(ILAY.GT.41)THEN
C Entrance angle cut
                  EntInr=abs(LVLCHN(1,iChain)/1024.)
                  IF(EntInr.LT.0.20)THEN
      
C loop over hits in the chain
                     IADR=LINCHN(ICHAIN)
 110                 CONTINUE
                     IF(IADR.NE.0)THEN

C increment counter if the hit was looked for
                        HCNTSF(1,IADR)=HCNTSF(1,IADR)+1

C INUEFF will be -ICHAIN if the hit is missing
                        IF(INUEFF(IADR).EQ.ICHAIN)THEN

C increment counter if the hit was found
                           HCNTSF(2,IADR)=HCNTSF(2,IADR)+1

C increment counter if the hit was LOW PULSE HEIGHT
                           IF(INUCHN(IADR).EQ.-6)THEN
                              HCNTSF(3,IADR)=HCNTSF(3,IADR)+1
                           ENDIF

C increment counter if the hit was double or more
                           IF(DOUBPT(IADR).NE.0)THEN
                              HCNTSF(4,IADR)=HCNTSF(4,IADR)+1
                           ENDIF

C check the CD hit map, IH will be zero if there is not hit, strange
                           ILAY=SFLSFA(IADR)
                           IF(ILAY.GE.0)THEN

      sf_data_input_sf_adr=IADR
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
      IH=sf_data_output_hit_adr

                              IF(IH.GT.0)THEN

C check the SF layer and wire, ILAY=0 for JUNK and VD_ALTERNATE_READOUT
                                 IWIR=SFWSFA(IADR)
                                 ILCD=CDLASF(ILAY)

                                 IF(ILCD.LT.0)THEN
                                    ILDR=0
                                 ELSE
                                    ILDR=ILCD+1-LCANDR

      sf_data_input_hit_adr=IH
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
      TDCVAL=sf_data_output_tdc_raw

      sf_data_input_hit_adr=IH
#define SF_DATA_TMPSPsS
#include "doit/sf_data_access/sf_data_access.inc"
      PSVAL=sf_data_output_r_tim_ps

      sf_data_input_hit_adr=IH
#define SF_DATA_ADCH
#include "doit/sf_data_access/sf_data_access.inc"
      HUNITS=sf_data_output_ph_happy
                                 
                                 ENDIF

C set up the triple time correlation for chosen layers
                                 IF (ILDR.GT.0)THEN
                                    CONT_COND=.TRUE.
                                    J=0
 117                                J=J+1
                                    IF((J.LE.3).AND.CONT_COND)THEN
                                       IF(ILDR.EQ.ILSAVE(J))THEN
                                          IWSAVE(J)=IWIR
                                          IHSAVE(J)=IH
                                          TDCSAV(J)=TDCVAL
                                          PsSave(J)=PSVAL
                                          DRFSAV(J)=EvntHit_Drift(IH)
                                          CONT_COND=.FALSE.
                                       ENDIF
                                       GO TO 117
                                    ENDIF
                                 ENDIF

C end of condition: (IH.GT.0)   
                              ENDIF
C end of condition: (ILAY.GE.0)
                           ENDIF
C end of condition: (INUEFF(IADR).EQ.ICHAIN)
                        ENDIF

C end of loop over hits in the chain
                        IF(IADR.NE.MINCHN(ICHAIN))THEN
                           IADR=ISTFOR(IADR)
                           GO TO 110
                        ENDIF
C end of condition: (IADR.NE.0)
                     ENDIF
                  
C continue the triple time correlation for chosen layers
                     DISSUM=0.
                     CONT_COND=.TRUE.
                     DO 145 J=1,3
C require 3 hits
                        IF(CONT_COND)THEN
                           IF(IHSAVE(J).NE.0)THEN
                              IF(J.EQ.2)THEN
                                 DISSUM=DISSUM+DRFSAV(J)*1000.
                              ELSE
                                 DISSUM=DISSUM+DRFSAV(J)*1000./2.
                              ENDIF
                           ELSE
                              CONT_COND=.FALSE.
                           ENDIF
                        ENDIF
 145                 CONTINUE
                     
C require outer hits to be the same wire number
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12  
                     IF(CONT_COND)THEN
                        IWIRDF=IWSAVE(3)-IWSAVE(1)+OFFSSV
                        NWIRL =NWIRCD(ILSAVE(2) -1+LCANDR)
                        IF(IWIRDF.GT. NWIRL/2)IWIRDF=IWIRDF-NWIRL
                        IF(IWIRDF.LT.-NWIRL/2)IWIRDF=IWIRDF+NWIRL
                        IF(IWIRDF.EQ.0)THEN
                           HISNUM=DRFSAV(2)*2000+1.
                           IF(HISNUM.LT.1)HISNUM=1
                           IF(HISNUM.GT.14)HISNUM=14
                        ENDIF
                     ENDIF

C end of condition: chain entrance angle
                  ENDIF
C end of condition: chain end
               ENDIF
C end of condition: chain start
            ENDIF
C end of condition: chain length
         ENDIF
C end of loop over chains
 175  CONTINUE

C extra hits are noise
      DO 185 IADR=1,MHITSF

         sf_data_input_sf_adr=IADR
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
         IH=sf_data_output_hit_adr
         
         IF(IH.GT.0)THEN
            IF(INUEFF(IADR).EQ.0)THEN
               HCNTSF(5,IADR)=HCNTSF(5,IADR)+1
            ENDIF
         ENDIF
 185  CONTINUE

      RETURN



C entry to print to screen
C entry to write a file
      ENTRY SFEFF2
      CRNAME=CRNAM2

      IF((.NOT.KILLA).AND.(COUNTR.GE.CNT_NEXT))THEN

 302     PRINT 3001,CRNAME
 3001    FORMAT(' ',A6,':',
     1        ' DO YOU REALLY WANT THE LAYER EFFICIENCIES?',
     2        '   "Y",   "K"ill,  "M"ore ')
         CALL DSF_RFV_INPUT(IY)
         IF(IY.EQ.'Y')THEN
            YESPRINT=.TRUE.
         ELSEIF(IY.EQ.'null')THEN
            YESPRINT=.FALSE.
         ELSEIF(IY.EQ.'N')THEN
            YESPRINT=.FALSE.
         ELSEIF(IY.EQ.'K')THEN
            YESPRINT=.FALSE.
            KILLA=.TRUE.
         ELSEIF(IY.EQ.'M')THEN
            PRINT 3011,COUNTR
 3011       FORMAT(' COUNTR=',I9,
     2           'type in the number of entries to process')
            ACCEPT 3012,CNT_WAIT
 3012       FORMAT(I)
            CNT_NEXT=COUNTR+CNT_WAIT
            PRINT 3013,CNT_WAIT,CNT_NEXT
 3013       FORMAT(' YOU SAID TO WAIT FOR ',I8,' ENTRIES;',
     2           ' WILL STOP AT ',I9)
            YESPRINT=.FALSE.
         ELSE
            GO TO 302
         ENDIF
         
         IF(YESPRINT)THEN
            IUNITE=37
C     CALL GETLUN(IUNITE,CRNAME)
            OPEN(UNIT=IUNITE,FILE='sfeff.dat',STATUS='UNKNOWN')
      
      
            ISM1ZD=0
            ISM2ZD=0
            ISM3ZD=0
            ISM4ZD=0
            ISM1DR=0
            ISM2DR=0
            ISM3DR=0
            ISM4DR=0
      
            PRINT 3003,        CRNAME
            WRITE(IUNITE,3003) CRNAME
 3003       FORMAT(' ',A6,':'/
     2           '   SFLAY CDLAY DRLAY    HIT    TRY   FRAC',
     3           '  LOWPH   FRAC  DOUBL   FRAC    NOISE    FRAC'/
     2           '                                  per try',
     3           '       per hit       per hit        per event')
      
      
            DO 340 ILYR=1,MLSFDR
               ILCD=CDLASF(ILYR)
         
C  DR layer
               ILDR=0
               IF((ILCD.GE.LCANDR).AND.(ILCD.LE.MCANDR))
     1              ILDR=ILCD+1-LCANDR

C address limits for this layer  and  initialize the sums
               LADR=INSFSF(ILYR)
               MADR=LADR+NWRSF(ILYR)-1

C loop over the addresses
               ISUM1=0
               ISUM2=0
               ISUM3=0
               ISUM4=0
               ISUM5=0
               DO 310 IADR=LADR,MADR
                  ISUM1=ISUM1+HCNTSF(1,IADR)
                  ISUM2=ISUM2+HCNTSF(2,IADR)
                  ISUM3=ISUM3+HCNTSF(3,IADR)
                  ISUM4=ISUM4+HCNTSF(4,IADR)
                  ISUM5=ISUM5+HCNTSF(5,IADR)
 310           CONTINUE

C accumulate the sums for the devices
               IF(ILCD.LE.0)THEN
                  ISM1ZD=ISM1ZD+ISUM1
                  ISM2ZD=ISM2ZD+ISUM2
                  ISM3ZD=ISM3ZD+ISUM3
                  ISM4ZD=ISM4ZD+ISUM4
                  
               ELSEIF(ILCD.LT.47)THEN
                  ISM1DR=ISM1DR+ISUM1
                  ISM2DR=ISM2DR+ISUM2
                  ISM3DR=ISM3DR+ISUM3
                  ISM4DR=ISM4DR+ISUM4
               ENDIF

C calculate and write the fractions for the layer
               FRAC2=0.
               FRAC3=0.
               FRAC4=0.
               IF(ISUM1.GT.0)THEN
                  FRAC2=1.*ISUM2/ISUM1
                  IF(ISUM2.GT.0)THEN
                     FRAC3=1.*ISUM3/ISUM2
                     FRAC4=1.*ISUM4/ISUM2
                  ENDIF
               ENDIF
               FRAC5=1.*ISUM5/(COUNTR*NWRSF(ILYR))
         
               PRINT 3004,        ILYR,ILCD,ILDR,
     1              ISUM2,ISUM1,FRAC2,ISUM3,FRAC3,
     2              ISUM4,FRAC4,ISUM5,FRAC5
               WRITE(IUNITE,3004) ILYR,ILCD,ILDR,
     1              ISUM2,ISUM1,FRAC2,ISUM3,FRAC3,
     2              ISUM4,FRAC4,ISUM5,FRAC5
 3004          FORMAT(2X,3I6,I7,I7,F7.3,I7,F7.3,I7,F7.3,I7,F9.5)
 340        CONTINUE

C END LOOP OVER SF LAYERS

 
C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12  
C calculate and write fractions for the devices
            FRAC2=0.
            IF(ISM1ZD.GT.0)FRAC2=1.*ISM2ZD/ISM1ZD
            FRAC3=0.
            IF(ISM2ZD.GT.0)FRAC3=1.*ISM3ZD/ISM2ZD
            IF(ISM2ZD.GT.0)FRAC4=1.*ISM4ZD/ISM2ZD
            PRINT 3007,        ISM2ZD,ISM1ZD,FRAC2,ISM3ZD,FRAC3,
     2           ISM4ZD,FRAC4
            WRITE(IUNITE,3007) ISM2ZD,ISM1ZD,FRAC2,ISM3ZD,FRAC3,
     2           ISM4ZD,FRAC4
      
            FRAC2=0.
            IF(ISM1DR.GT.0)FRAC2=1.*ISM2DR/ISM1DR
            FRAC3=0.
            IF(ISM2DR.GT.0)FRAC3=1.*ISM3DR/ISM2DR
            IF(ISM2DR.GT.0)FRAC4=1.*ISM4DR/ISM2DR
            
            PRINT 3008,        ISM2DR,ISM1DR,FRAC2,ISM3DR,FRAC3,
     2           ISM4DR,FRAC4
            WRITE(IUNITE,3008) ISM2DR,ISM1DR,FRAC2,ISM3DR,FRAC3,
     2           ISM4DR,FRAC4
      
 3007       FORMAT(14X,'ALL ZD',I7,I7,F7.3,I7,F7.3,I7,F7.3)
 3008       FORMAT(14X,'ALL DR',I7,I7,F7.3,I7,F7.3,I7,F7.3)

            CLOSE(UNIT=IUNITE)

         ENDIF

         PRINT 3015,CRNAME,COUNTR
 3015    FORMAT(' LEAVING ',A6,':  COUNTR=',I6)
      ENDIF
#endif

      RETURN
      END
