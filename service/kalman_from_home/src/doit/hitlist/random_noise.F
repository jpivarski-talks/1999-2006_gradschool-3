*
* $Id: random_noise.F,v 1.1 2002/05/23 14:14:00 dpp Exp $
*
* $Log: random_noise.F,v $
* Revision 1.1  2002/05/23 14:14:00  dpp
*      -> NEW ROUTINE
*      -> adds random noise onto the event
*
*       note:    cdscrtcd uses parameter MHWCD
*                which is set to 2000+2*3000 in cdscrtcd
*                it appears that iadrhw in cdscrtcd is filled
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author : D. Peterson jan 14,2002
      SUBROUTINE RANDOM_NOISE

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE
#include "doit/duseq/tfindpar.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
#include "doit/duseq/cdscrtcd.inc"

#include "doit/sfseq/sfpar.inc"
#include "doit/sfseq/sfcom.inc"
#include "doit/sfseq/sfextra.inc"

      INTEGER ITRY
      INTEGER ADDRL,ADDRM,IHW
      INTEGER IDUM,ADRTRY,HWNEXT
      INTEGER IL,ILCD,IWIR
      REAL DISTTMP,TIMETMP
      INTEGER ILASTH(KLYRCD)

c to compile in solaris, must be deleted in OSF
      REAL RAN(2)

C note: will require stripping out the added hits,
C before CDTRACK, to allow running more than 1 event at a time


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------


      DO 19 ILCD=1,47
         IF(EXFHCD(ILCD).EQ.0)THEN
            ILASTH(ILCD)=0
         ELSE
            ADRTRY=EXFHCD(ILCD)
 11         CONTINUE
            IF(ADRTRY.NE.0)THEN
               ILASTH(ILCD)=ADRTRY
               ADRTRY=EXNHCD(ADRTRY)
               GO TO 11
            ENDIF
         ENDIF
 19   CONTINUE



      DO 99 ITRY=1,1000

C== IADRHW arrays

         ADDRL=INDXCD(1)
         ADDRM=INDXCD(47)+NWIRCD(47)-1
         ADRTRY=ADDRL+(ADDRM-ADDRL)*RAN(IDUM)
         IHW=IADRHW(ADRTRY)
         DISTTMP=.007 *RAN(IDUM)
         TIMETMP=DISTTMP/ (.000030/1.E-9) * 1000.

         ILCD=0
         IL=0
 31      IL=IL+1
         IF(
     1        (ILCD.EQ.0).AND.
     2        (IL.LE.47))THEN
            IF(
     1           (ADRTRY.GE.INDXCD(IL)).AND.
     2           (ADRTRY.LT.INDXCD(IL)+NWIRCD(IL)))THEN
               ILCD=IL
               IWIR=ADRTRY-INDXCD(IL)
            ENDIF
            GO TO 31
         ENDIF

         print 1001,itry,ADRTRY
 1001    format(' random_noise, itry=',I5,' ADRTRY=',I5)

         IF(IHW.EQ.0)THEN

            IF(NHTWCD.LT.MHWCD)THEN
               IF(ILCD.NE.0)THEN
                  NHTWCD=NHTWCD+1
                  IHW=NHTWCD
                  IADRHW(ADRTRY)=IHW
                  IF(EXFHCD(ILCD).EQ.0 )THEN
                     EXFHCD(ILCD)=IHW 
                  ELSE
                     EXNHCD(ILASTH(ILCD))=IHW
                  ENDIF
                  EXNHCD(IHW)=0
                  ILASTH(ILCD)=IHW
                  print 1002,ilcd,disttmp
 1002             format('   NEW WIRE      LAY=',I3,' drift=',F9.6)
               ENDIF
            ENDIF
         ELSE
            IF(DISTTMP.GT.DWCD(IHW))THEN
               IHW=0
                  print 1003,ilcd,disttmp,DWCD(IHW)
 1003             format(' KEEP OLD, LAY=',I3,' drift=',F9.6,
     2                 '  OLD DRIFT=',F9.6)
            ELSE
                  print 1004,ilcd,disttmp,DWCD(IHW)
 1004             format(' REPLACE, LAY=',I3,' drift=',F9.6,
     2                 '  OLD DRIFT=',F9.6)

            ENDIF
         ENDIF


         IF(IHW.NE.0)THEN
            TDCWCD(IHW)= 10000.
            TDWCD(IHW)=  TIMETMP
            TCWCD(IHW)=  TIMETMP
            QWCD(IHW)=   100.
            ICYLCD(IHW)= ILCD
            IWIRCD(IHW)= IWIR
            PHIWCD(IHW)= PHIFCD(ILCD)+IWIR*CELLCD(ILCD)
            XWCD(IHW)=   RCD(ILCD)*COS(PHIWCD(IHW))
            YWCD(IHW)=   RCD(ILCD)*SIN(PHIWCD(IHW))
            ZWCD(IHW)=   0.
            DWCD(IHW)=   DISTTMP
            PWCD(IHW)=   100.
            INXHCD(IHW)= 0
            DVHTCD(IHW)= 0
            QUALCD(IHW)= 0
            NTDCCD(IHW)= 1
            EARLYCD(IHW)=0
            LATECD(IHW)= 0
            EXRADC(IHW)= 100.
         ENDIF

 99   CONTINUE




      RETURN
      END









