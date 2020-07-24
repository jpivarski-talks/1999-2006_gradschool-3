/****************************************************************
 *                                                              *
 *                                                              *
 * Test histogram program                                       *
 ****************************************************************/

#include <test.h>


void main() 
{
  HIHistoManager *HistoManager;
  hm1 = new HistoManager;

  HIHistoManager->HIHist1D->HIHbookHist1D->hbook_init();
  
  HIHistoManager->HIHist1D->HIHbookHist1D->hbook1(
  0,"test",20
