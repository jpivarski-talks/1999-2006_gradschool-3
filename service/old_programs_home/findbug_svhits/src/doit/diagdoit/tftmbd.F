*
* $Id: tftmbd.F,v 1.1.1.1 1998/02/06 19:11:41 dpp Exp $
*
* $Log: tftmbd.F,v $
* Revision 1.1.1.1  1998/02/06 19:11:41  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:45  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:00:58  zfiles
* first version of doit in CVS
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*CMZ :  5.54/07 25/09/91  08.52.30  by  Steve Schaffner
*CMZ :  5.53/04 14/06/90  15.57.39  by  Steve Schaffner
*CMZ :  5.51/00 11/08/89  14.23.05  by  Steve Schaffner
*CMZ :  5.50/02 15/05/89  17.48.16  by  Steve Schaffner
*-- Author :
      SUBROUTINE TFTMBD

C-->  TF INITIALIZATION OF TIMER NAMES

C     ==================================================================
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif

#include "doit/duseq/tfindpar.inc"
#include "doit/duseq/tftime.inc"

C     ==================================================================

C-->  THE 2 DIGIT # IN THE SUB NAME GIVES ITS POSITION ON THE ARRAYS

      SUBTTL(1) = 'TOTAL  1'
      SUBTTL(2) = 'DUET2  2'
      SUBTTL(3) = 'DUET3  3'
      SUBTTL(4) = 'DUET   4'
      SUBTTL(5) = 'DRHITW 5'
C      SUBTTL(6) = 'CDGET  6'
      SUBTTL(6) = 'CDFTCH 6'
      SUBTTL(7) = 'TFCORR 7'
      SUBTTL(8) = 'TF3FIT 8'
      SUBTTL(9) = 'CDHIST 9'
      SUBTTL(10) = 'CDNEWT10'
      SUBTTL(11) = 'V0FIND11'
      SUBTTL(12) = 'PHFIND12'
      SUBTTL(13) = 'CDSTOR13'
      SUBTTL(14) = 'TFIND 14'
      SUBTTL(15) = 'CFDRDG15'
      SUBTTL(16) = 'CFIND 16'
      SUBTTL(17) = 'CFDRDG17'
      SUBTTL(18) = 'TFCORR18'
      SUBTTL(19) = 'TFGETR19'
      SUBTTL(20) = 'CFLINK20'
      SUBTTL(21) = 'TFTREE21'
      SUBTTL(22) = 'TFORST22'
      SUBTTL(23) = 'TFCLMB23'
      SUBTTL(24) = 'TFNEWT24'
      SUBTTL(25) = 'CFTFIT25'
      SUBTTL(26) = 'CFCIRF26'
      SUBTTL(27) = 'TFSORT27'
      SUBTTL(28) = 'CFTCUT28'
      SUBTTL(29) = 'CFRFIT29'
      SUBTTL(30) = 'CFHIST30'
      SUBTTL(31) = 'CFIND 31'
      SUBTTL(32) = 'CFIND 32'
      SUBTTL(33) = 'CFHITS33'
      SUBTTL(34) = 'TFCORR34'
      SUBTTL(35) = 'TFGETR35'
      SUBTTL(36) = 'CFLINK36'
      SUBTTL(37) = 'TFTREE37'
      SUBTTL(38) = 'TFORST38'
      SUBTTL(39) = 'TFCLMB39'
      SUBTTL(40) = 'TFNEWT40'
      SUBTTL(41) = 'CFTFIT41'
      SUBTTL(42) = 'CFCIRF42'
      SUBTTL(43) = 'TFSORT43'
      SUBTTL(44) = 'CFTCUT44'
      SUBTTL(45) = 'CFRFIT45'
      SUBTTL(46) = 'CFHIST46'
      SUBTTL(47) = 'LFIND 47'
      SUBTTL(48) = 'LFIND 48'
      SUBTTL(49) = 'LFHITS49'
      SUBTTL(50) = 'TFCORR50'
      SUBTTL(51) = 'TFGETR51'
      SUBTTL(52) = 'LFLINK52'
      SUBTTL(53) = 'TFTREE53'
      SUBTTL(54) = 'TFORST54'
      SUBTTL(55) = 'TFCLMB55'
      SUBTTL(56) = 'TFNEWT56'
      SUBTTL(57) = 'LFTFIT57'
      SUBTTL(58) = 'LFLINF58'
      SUBTTL(59) = 'TFSORT59'
      SUBTTL(60) = 'LFTCUT60'
      SUBTTL(61) = 'LFRFIT61'
      SUBTTL(62) = 'LFHIST62'

      RETURN
      END
* <><><><><><><><><><><><><><><><><><><><><><><><><><><><><>
#include "doit/duseq/dudeckd.inc"
