*
* $Id: statusbits.F,v 1.2 2002/05/23 14:36:40 dpp Exp $
*
* $Log: statusbits.F,v $
* Revision 1.2  2002/05/23 14:36:40  dpp
*      -> new status bits to indicate use of limited radius solution
*      -> new flag to fetch status bits from storage
*
* Revision 1.1  2001/11/19 23:01:18  dpp
*     -> NEW ROUTINE
*      -> encapsulate bit assignments for status bits
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"

      SUBROUTINE STATUSBITS(action,ind,val)
C......................................................................
C.
C. STATUSBITS - access status bits
C.
C. COMMON    : 
C. CALLS     :
C. CALLED    : DUFIND
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : 16-Oct-2001
C......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tfhitscd.inc"
#include "doit/duseq/tftrakcd.inc"
#include "doit/duseq/statusbits.inc"

      INTEGER ACTION,IND,VAL
      LOGICAL OPENED/.FALSE./
      INTEGER WORD(MSTATUS),BIT1(MSTATUS),LENG(MSTATUS)


C23456789 123456789 123456789 123456789 123456789 123456789 123456789 12
C-----------------------------------------------------------------------
C executable code starts here
C-----------------------------------------------------------------------
      IF(.NOT.OPENED)THEN
C-----------------------------------------------------------------------
	WORD(kstSFseedind)=   1
        BIT1(kstSFseedind)=     1
        LENG(kstSFseedind)=       1
	WORD(kstALT1seedind)= 1
        BIT1(kstALT1seedind)=   2
        LENG(kstALT1seedind)=     1
	WORD(kstALT2seedind)= 1
        BIT1(kstALT2seedind)=   3
        LENG(kstALT2seedind)=     1
	WORD(kstSFpassnum)=   1
        BIT1(kstSFpassnum)=     4
        LENG(kstSFpassnum)=       3
	WORD(kstSFmergetype)= 1
        BIT1(kstSFmergetype)=   7
        LENG(kstSFmergetype)=     2
	WORD(kstSFmergeind)=  1
        BIT1(kstSFmergeind)=    9
        LENG(kstSFmergeind)=      1
	WORD(kstSFchainnum)=  1
        BIT1(kstSFchainnum)=   10
        LENG(kstSFchainnum)=      8
	WORD(kstFTnhitwirfit)=1
        BIT1(kstFTnhitwirfit)= 18
        LENG(kstFTnhitwirfit)=    6
	WORD(kstFTrmD0conwf)= 1
        BIT1(kstFTrmD0conwf)=  24
        LENG(kstFTrmD0conwf)=     1
	WORD(kstFTtaddKNKwf)= 1
        BIT1(kstFTtaddKNKwf)=  25
        LENG(kstFTtaddKNKwf)=     1
	WORD(kstFTcwKNKwf)=   1
        BIT1(kstFTcwKNKwf)=    26
        LENG(kstFTcwKNKwf)=        1
	WORD(kstFTcwKNKaddc)= 1
        BIT1(kstFTcwKNKaddc)=  27
        LENG(kstFTcwKNKaddc)=     1
	WORD(kstFTtaddKNKhf)= 1
        BIT1(kstFTtaddKNKhf)=  28
        LENG(kstFTtaddKNKhf)=     1
	WORD(kstFTcwKNKhf)=   1
        BIT1(kstFTcwKNKhf)=    29
        LENG(kstFTcwKNKhf)=       1
	WORD(kstFTcwKNKdbaa)= 1
        BIT1(kstFTcwKNKdbaa)=  30
        LENG(kstFTcwKNKdbaa)=     1
C-----------------------------------------------------------------------
	WORD(kstFT1D0con)=    2
        BIT1(kstFT1D0con)=      1
        LENG(kstFT1D0con)=        1
	WORD(kstFT1Z0con)=    2
        BIT1(kstFT1Z0con)=      2
        LENG(kstFT1Z0con)=        1
	WORD(kstFT1altamb)=   2
        BIT1(kstFT1altamb)=     3
        LENG(kstFT1altamb)=       1
	WORD(kstFT1altambNUL)=2
        BIT1(kstFT1altambNUL)=  4
        LENG(kstFT1altambNUL)=    1
	WORD(kstFT1nhitlamb)= 2
        BIT1(kstFT1nhitlamb)=   5
        LENG(kstFT1nhitlamb)=     6
	WORD(kstFT2D0con)=    2
        BIT1(kstFT2D0con)=     11
        LENG(kstFT2D0con)=        1
	WORD(kstFT2Z0con)=    2
        BIT1(kstFT2Z0con)=     12
        LENG(kstFT2Z0con)=        1
	WORD(kstFT2altamb)=   2
        BIT1(kstFT2altamb)=    13
        LENG(kstFT2altamb)=       1
	WORD(kstFT2altambNUL)=2
        BIT1(kstFT2altambNUL)= 14
        LENG(kstFT2altambNUL)=    1
	WORD(kstFT2nhitlamb)= 2
        BIT1(kstFT2nhitlamb)=  15
        LENG(kstFT2nhitlamb)=     6
	WORD(kstDUtfxfitCF1)= 2
        BIT1(kstDUtfxfitCF1)=  21
        LENG(kstDUtfxfitCF1)=     1
	WORD(kstDUrepZhaCF1)= 2
        BIT1(kstDUrepZhaCF1)=  22
        LENG(kstDUrepZhaCF1)=     1
	WORD(kstFTRneedrec)=  2
        BIT1(kstFTRneedrec)=   23
        LENG(kstFTRneedrec)=      1
	WORD(kstFTRrecFT1)=   2
        BIT1(kstFTRrecFT1)=    24
        LENG(kstFTRrecFT1)=       1
	WORD(kstFTRrecFT2)=   2
        BIT1(kstFTRrecFT2)=    25
        LENG(kstFTRrecFT2)=       1
	WORD(kstFTRnewrep)=   2
        BIT1(kstFTRnewrep)=    26
        LENG(kstFTRnewrep)=       1
	WORD(kstDUtfxfitCF2)= 2
        BIT1(kstDUtfxfitCF2)=  27
        LENG(kstDUtfxfitCF2)=     1
	WORD(kstDUrepZhaCF2)= 2
        BIT1(kstDUrepZhaCF2)=  28
        LENG(kstDUrepZhaCF2)=     1
	WORD(kstDUrZ0biasaCF)=2
        BIT1(kstDUrZ0biasaCF)= 29
        LENG(kstDUrZ0biasaCF)=    1
C-----------------------------------------------------------------------
	WORD(kstDUtfxfitLFnt)=3
        BIT1(kstDUtfxfitLFnt)=  1
        LENG(kstDUtfxfitLFnt)=    1
	WORD(kstDUtfxfitLFzt)=3
        BIT1(kstDUtfxfitLFzt)=  2
        LENG(kstDUtfxfitLFzt)=    1
	WORD(kstDUtfxfitLFzu)=3
        BIT1(kstDUtfxfitLFzu)=  3
        LENG(kstDUtfxfitLFzu)=    1
	WORD(kstBSZHboot)=    3
        BIT1(kstBSZHboot)=      4
        LENG(kstBSZHboot)=        1

	WORD(kstFTlimitrTEST)=3
        BIT1(kstFTlimitrTEST)=  5
        LENG(kstFTlimitrTEST)=    1
	WORD(kstFTlimitrSOL)= 3
        BIT1(kstFTlimitrSOL)=   6
        LENG(kstFTlimitrSOL)=     1
	WORD(kstFTlimitrSEL)= 3
        BIT1(kstFTlimitrSEL)=   7
        LENG(kstFTlimitrSEL)=     1


	WORD(kstCDJjoin)=     3
        BIT1(kstCDJjoin)=      11
        LENG(kstCDJjoin)=         1
	WORD(kstCDJdied)=     3
        BIT1(kstCDJdied)=      12
        LENG(kstCDJdied)=         1
	WORD(kstTF3ent)=      3
        BIT1(kstTF3ent)=       13
        LENG(kstTF3ent)=          1
	WORD(kstTF3knk1allow)=3
        BIT1(kstTF3knk1allow)= 14
        LENG(kstTF3knk1allow)=    1
	WORD(kstTF3knk1kill)= 3
        BIT1(kstTF3knk1kill)=  15
        LENG(kstTF3knk1kill)=     1
	WORD(kstTF3stLOOPit)=3
        BIT1(kstTF3stLOOPit)=  16
        LENG(kstTF3stLOOPit)=     1
	WORD(kstTF3knk2activ)=3
        BIT1(kstTF3knk2activ)= 17
        LENG(kstTF3knk2activ)=    1
	WORD(kstTF3knk2kill)= 3
        BIT1(kstTF3knk2kill)=  18
        LENG(kstTF3knk2kill)=     1
	WORD(kstTF3notCONV)=  3
        BIT1(kstTF3notCONV)=   19
        LENG(kstTF3notCONV)=      1
	WORD(kstTF3yesCONV)=  3
        BIT1(kstTF3yesCONV)=   20
        LENG(kstTF3yesCONV)=      1
	WORD(kstTF3hitsdel)=  3
        BIT1(kstTF3hitsdel)=   21
        LENG(kstTF3hitsdel)=      3
	WORD(kstTF3iter)=     3
        BIT1(kstTF3iter)=      24
        LENG(kstTF3iter)=         4
	WORD(kstTF3err)=      3
        BIT1(kstTF3err)=       28
        LENG(kstTF3err)=          4
C-----------------------------------------------------------------------
        OPENED=.TRUE.
      ENDIF

      IF(ACTION.EQ.kSTfillcs)THEN
         CALL SBYT(VAL,STATCS(WORD(ind)),BIT1(ind),LENG(ind))
      ELSEIF(ACTION.EQ.kSTfillcf)THEN
         CALL SBYT(VAL,STATCF(WORD(ind)),BIT1(ind),LENG(ind))
      ELSEIF(ACTION.EQ.kSTfillcd)THEN
      ELSEIF(ACTION.EQ.kSTcstocf)THEN
      ELSEIF(ACTION.EQ.kSTcftocd)THEN
      ELSEIF(ACTION.EQ.kSTfetchcf)THEN
         VAL=0
         CALL CBYT(STATCF(WORD(ind)),BIT1(ind),VAL,1,LENG(ind))
      ENDIF

      RETURN
      END






