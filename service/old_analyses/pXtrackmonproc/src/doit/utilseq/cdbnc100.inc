*
* $Id: cdbnc100.inc,v 1.1 1999/11/29 21:44:17 lyon Exp $
*
* $Log: cdbnc100.inc,v $
* Revision 1.1  1999/11/29 21:44:17  lyon
* Changes to include files to remove references to old Cleo II include areas
*
* Revision 1.2  1997/02/21 17:47:41  clib
* Remove BUNSPC, now in tfbnch.inc.
*
* Revision 1.1  1997/02/21 15:56:55  clib
* New release.  Bunch finder results from tracking (was DUBN).
*
*
*  Bunch finder results from DUET
*
*  cdbnc100.inc (was dubnch.inc)
* 
#if !defined(CLEO_NOCOMDOC)
*
* DUBNCH  Documentation
*
* IBNTD      - The Final Bunch # assigned to the event.
* IBNDUF     - Bunch # best determined from DUET runs 
* IBNTFD(5)  - Bunch # from TFBN using only TOF hits matched
*              to tracks in each DUET under diff. bunch hypothesis
* IBNDU(5)   - Bunch # from only DUET under diff. bunch hypothesis
* NTGDBN(5)  - N_good track for each DUET run
* IBNHIS(5)  - Bunch finding history
*
*      IBNHIS(1)  : history for 2-bunch data
*         =  1   DUET is run once with IBNTF, which turns
*                out to be the correct bunch
*            2   DUET is run twice, IBNTF is the correct bunch
*            3   DUET is run twice, IBNTF is no good.
*                IBNTD .ne. IBNTF
*            0   Can't determined the bunch number
*                IBNTD is set to 1
* IFLDBN     - Duet bunch finder flag  (0 OFF, 1 ON)
*
*
* TFBDLR(5)  - log ratios using TOF hits that are matched to 
*              charged tracks in each DUET run
* TFBDCF(5)  - Confidence level from TFBDLR()
* DUEVAL(5)  - Combination of residual/hit multiplicity used
*              in assigning IBNDU()
* DUEVLF     - Variable constructed from DUEVAL() for making 
*              final DUET decision (IBNDUF)
#endif
*
      INTEGER IBNTFD, IBNDU, NTGDBN, IBNDUF, IBNTD, IBNHIS, IFLDBN

      COMMON /IDBNCM/ 
     +  IBNTD,IBNDUF,IBNTFD(5),IBNDU(5),NTGDBN(5),IBNHIS(5),IFLDBN

      REAL TFBDLR, TFBDCF, DUEVAL, DUEVLF

      COMMON /RDBNCM/
     +  TFBDLR(5),TFBDCF(5),DUEVAL(5),DUEVLF
* --------------------------------------------------------------------












