//
// To produce a file with ROOT that Rio_test
// must be able to read.
//
//     OS> root
//     root[] .x Rio_ROOT_write.C
//     root[] .qqq
//
//     OS> Rio_test -read
//
//

{
  TFile* file = new TFile("hello.root","RECREATE");
  file->ls();

  TDirectory* dir = file->mkdir("trees");
  dir->cd();

  TTree* tree = new TTree("myTree","My tree");

  struct {
    Int_t v1;
    Float_t v3;
  } st;

  TNamed* named = new TNamed;

  tree->Branch("st",&st,"v1/I:v3/F");
  TTree::SetBranchStyle(0);
  //tree->Branch("nameds","TNamed",&named);

  for(int count=0;count<500;count++) {
    st.v1 = count;
    st.v3 = 3.0F * count;
    tree->Fill();
  }

  file->Write();

  file->Close();
}
