//
//

{
  TFile* file = new TFile("/exp/LHCb/data/mc/D278590","READ");
  file->ls();

  
  TTree* tree = (TTree*)file->Get("_Event_MC_Particles");
  tree->ls();
  tree->Print();

  file->Close();
}
