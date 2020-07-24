*
* $Id: tftimedo.inc,v 1.1.1.1 1998/02/06 19:11:49 dpp Exp $
*
* $Log: tftimedo.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:49  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:25  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:09  zfiles
* first version of doit in CVS
*
*
* TFTIME Documentation
*
* tftimedo.inc
*
C
* NTIMDM - Number of timed subroutines
*
* ITTOTL - Address of IZANA2 timer
* ITIZA2 - Address of IZANA2 timer
* ITIZA3 - Address of IZANA3 timer
* ITDUE2 - Address of DUET2  timer
* ITDUE3 - Address of DUET3  timer
* ITIZSV - Address of IZSAVE & IZHITW timer
* ITGT20 - Address of GET DIN 20 timer
* ITDUET - Address of DUET/TRACK timer
* ITDRHW - Address of DRHITW timer
* ITCDFT - Address of CDFTCH timer
*
* ITTFCR - Address of TFCORR timer
* IT3FIT - Address of TF3FIT timer
* ITCDHS - Address of CDHIST timer
* ITCDNT - Address of CDNEWT timer
* ITDKFN - Address of DKFIND timer
* ITPHFN - Address of PHFIND timer
* ITCDST - Address of CDSTOR timer
*
* ITTFND - Base address of TFIND  timers
* KTIMDG - Base address of CFDRDG timers
* KTIMCF - Base address of CFIND  timers
* KTIMLF - Base address of LFIND  timers
*
* ITFIND - Relative address of CFIND  & LFIND  timers
* ITHITS - Relative address of CFHITS & LFHITS timers
* ITCORR - Relative address of CFCORR & LFCORR timers
* ITGETR - Relative address of TFGETR timers
* ITLINK - Relative address of CFLINK & LFLINK timers
* ITTREE - Relative address of TFTREE timers
* ITFRST - Relative address of TFORST timers
* ITCLMB - Relative address of TFCLMB timers
* ITNEWT - Relative address of TFNEWT timers
* ITTFIT - Relative address of CFTFIT & LFTFIT timers
* ITFIT  - Relative address of CFFIT  & LFFIT  timers
* ITSORT - Relative address of TFSORT timers
* ITTCUT - Relative address of CFTCUT & LFTCUT timers
* ITRFIT - Relative address of CFRFIT & LFRFIT timers
* ITHIST - Relative address of CFHIST & LFHIST timers
*
* /TFTIME/ Accumulates statistics for TFIND, LFIND & CFIND
*
* ISUB                - Relative address of a subroutine in /TFTIME/
* KTIMES              - Base address of CFIND or LFIND   in /TFTIME/
* NCALLS(KTIMES+ISUB) - # of times the subroutine was called
* ITIMES(KTIMES+ISUB) - Time in msec in that subroutine used
* SUBTTL(KTIMES+ISUB) - Name of the subroutine timed
*
* ----------------------------------------------------------
#include "doit/duseq/duseqd.inc"
