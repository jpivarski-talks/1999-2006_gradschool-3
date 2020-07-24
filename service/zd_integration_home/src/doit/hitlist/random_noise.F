*
* $Id: random_noise.F,v 1.4 2003/05/12 16:07:47 dpp Exp $
*
* $Log: random_noise.F,v $
* Revision 1.4  2003/05/12 16:07:47  dpp
*      -> use CLEO_DOITRANDOMNOISE to dummy this routine in CLEO production
*
* Revision 1.3  2003/03/06 16:59:59  dpp
*      -> changed compile switch from CLEO_SUNOS to CLEO_SunOS
*
* Revision 1.2  2003/03/05 17:52:48  dpp
*      -> remove use of PWCD; PWCD can be used for  other derived values
*      -> use EvntHit_OpnHit to indicate usable hits
*      -> use compile switch for the declaration of RAN
*
* Revision 1.1  2002/05/23 14:14:00  dpp
*      -> NEW ROUTINE
*      -> adds random noise onto the event
*
*       note:    cdscrtcd uses parameter EvntHitMax
*                which is set to 2000+2*3000 in cdscrtcd
*                it appears that EvntHit_MapDet in cdscrtcd is filled
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

*-- Author : D. Peterson jan 14,2002
      SUBROUTINE RANDOM_NOISE
#if defined(CLEO_DOITRANDOMNOISE)

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

C to compile in solaris, must be deleted in OSF
#if defined(CLEO_SunOS)
      REAL RAN(2)
#endif

C note: will require stripping out the added hits,
C before CDTRACK, to allow running more than 1 event at a time


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
* ----------Executable code starts here---------------------------------


      DO 19 ILCD=1,47
         IF(EvntHit_1stLyr(ILCD).EQ.0)THEN
            ILASTH(ILCD)=0
         ELSE
            ADRTRY=EvntHit_1stLyr(ILCD)
 11         CONTINUE
            IF(ADRTRY.NE.0)THEN
               ILASTH(ILCD)=ADRTRY
               ADRTRY=EvntHit_NxtLyr(ADRTRY)
               GO TO 11
            ENDIF
         ENDIF
 19   CONTINUE



      DO 99 ITRY=1,1000

C==  EvntHit_MapDet arrays

         ADDRL=INDXCD(1)
         ADDRM=INDXCD(47)+NWIRCD(47)-1
         ADRTRY=ADDRL+(ADDRM-ADDRL)*RAN(IDUM)
         IHW=EvntHit_MapDet(ADRTRY)
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

            IF(EvntHit_Num.LT.EvntHitMax)THEN
               IF(ILCD.NE.0)THEN
                  EvntHit_Num=EvntHit_Num+1
                  IHW=EvntHit_Num
                  EvntHit_MapDet(ADRTRY)=IHW
                  IF(EvntHit_1stLyr(ILCD).EQ.0 )THEN
                     EvntHit_1stLyr(ILCD)=IHW 
                  ELSE
                     EvntHit_NxtLyr(ILASTH(ILCD))=IHW
                  ENDIF
                  EvntHit_NxtLyr(IHW)=0
                  ILASTH(ILCD)=IHW
                  print 1002,ilcd,disttmp
 1002             format('   NEW WIRE      LAY=',I3,' drift=',F9.6)
               ENDIF
            ENDIF
         ELSE
            IF(DISTTMP.GT.EvntHit_Drift(IHW))THEN
               IHW=0
                  print 1003,ilcd,disttmp,EvntHit_Drift(IHW)
 1003             format(' KEEP OLD, LAY=',I3,' drift=',F9.6,
     2                 '  OLD DRIFT=',F9.6)
            ELSE
                  print 1004,ilcd,disttmp,EvntHit_Drift(IHW)
 1004             format(' REPLACE, LAY=',I3,' drift=',F9.6,
     2                 '  OLD DRIFT=',F9.6)

            ENDIF
         ENDIF


         IF(IHW.NE.0)THEN
            EvntHit_OpnHit(IHW)=  7
            EvntHit_rawTDC(IHW)=  10000.
            EvntHit_rawTIM(IHW)=  TIMETMP
            EvntHit_corTIM(IHW)=  TIMETMP
            EvntHit_Charge(IHW)=  100.
            EvntHit_CDlayr(IHW)=  ILCD
            EvntHit_laWire(IHW)=  IWIR
            EvntHit_PhiWir(IHW)=  PHIFCD(ILCD)+IWIR*CELLCD(ILCD)
            EvntHit_X(IHW)=       RCD(ILCD)*COS(EvntHit_PhiWir(IHW))
            EvntHit_Y(IHW)=       RCD(ILCD)*SIN(EvntHit_PhiWir(IHW))
            EvntHit_Z(IHW)=       0 .
            EvntHit_Drift(IHW)=   DISTTMP
            EvntHit_NxtCls(IHW)=  0
            EvntHit_NHitDev(IHW)= 0
            EvntHit_Qual(IHW)  =  0
            EvntHit_nMult(IHW)=   1
            EvntHit_TimEarly(IHW)=0
            EvntHit_TimLate(IHW)= 0
            EvntHit_rawADC(IHW)=  100
C EvntHit_corPhi, pwcd is now left unused
         ENDIF

 99   CONTINUE


C end of compliation flag CLEO_DOITRANDOMNOISE
#endif
      RETURN
      END









