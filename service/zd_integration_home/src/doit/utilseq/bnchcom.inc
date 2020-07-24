*
* $Id: bnchcom.inc,v 1.1 1999/11/29 21:44:15 lyon Exp $
*
* $Log: bnchcom.inc,v $
* Revision 1.1  1999/11/29 21:44:15  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.6  1997/02/19 18:33:22  dlk
* DLK: Added array IQBUN(5,9) which specifies the bunch currents by bunch
*      and train number.  This information will eventually be supplied by
*      CESR, and is used by the bunch finder TFBN.
*
* Revision 1.5  1995/10/26 02:55:15  zfiles
* Individual hits info stored.
*
* Revision 1.4  1995/10/10  14:53:37  zfiles
* Reorganized completely by D. Kim.
*
* Revision 1.3  1995/09/19  10:56:37  zfiles
* New version from D. kim
*
* Revision 1.2  1995/09/09  20:30:56  zfiles
* Added a variable on bnchcom.inc
*
* Revision 1.1  1994/11/07  17:55:15  zfiles
* New common blocks for the CESR bunch train.
*
*
*CMZ :          11/10/94  12.40.02  by  H. Adrian Cho
* ------------------------------------------
* COMMON BLOCKS FOR BUNCH FINDING ROUTINE
*
* bnchcom.inc
*
#if !defined(CLEO_NOCOMDOC)
#include "seq/bnchseq/bnchdoc.inc"
#endif
* ------------------------------------------
C
C....... CESR BUNCH-FINDER STUFF

      INTEGER MTRNCS,MBUNCS
      PARAMETER(MTRNCS=9)
      PARAMETER(MBUNCS=3)
* NTRNCS - Number of trains reported by CESR (e.g. 9)
* NBUNCS _ Number of bunches per train (e.g. 2, 3, or 5)
* DELTCS - Time interval between bunches (e.g. 28000.0 psec)
* IBUNQ  - Charge in uAmps by bunch and train.  Dimensioned 5x9
      INTEGER NTRNCS,NBUNCS
      REAL DELTCS
      INTEGER IBUNQ
C
C....... TOF BUNCH-FINDER STUFF

      REAL TFSTTF
      INTEGER IBUNTF, NHTTFB, NHTTFE
      REAL P1OP2, CONFTF
      REAL TFBNBP, TFBNEP
      INTEGER TFBNBC, TFBNEC
C
C....... DR BUNCH-FINDER STUFF

      LOGICAL LPSSHF
      REAL TFSTDR
      INTEGER IBUNDR,IMODDR
      INTEGER MCHNBF
      PARAMETER (MCHNBF=5)
      INTEGER NCHNBF,NJNKBF
      INTEGER LENCBF,MFITBF,IBNCBF
      REAL RMSCBF,ARMSBF,SRMSBF,RRMSBF

C
C....... SUMMARY STUFF (END RESULTS)

      INTEGER IBUNTD,IDRTF

C
C-------------------------------------------

      COMMON/CESRBF/NTRNCS,NBUNCS,DELTCS,IBUNQ(5,9)

      COMMON/TOFBF/TFSTTF,IBUNTF,NHTTFB,NHTTFE,P1OP2,CONFTF,
     1             TFBNBP(64),TFBNEP(56),TFBNBC(64),TFBNEC(56)
C
      COMMON/DRBF/ LPSSHF,TFSTDR,IBUNDR,IMODDR,NCHNBF,NJNKBF,
     1             LENCBF(MCHNBF),MFITBF(MCHNBF),IBNCBF(MCHNBF),
     2             ARMSBF(MBUNCS),SRMSBF(MBUNCS),RRMSBF(MBUNCS),
     3             RMSCBF(MCHNBF,MBUNCS)
C
      COMMON/SUMMBF/IBUNTD,IDRTF




