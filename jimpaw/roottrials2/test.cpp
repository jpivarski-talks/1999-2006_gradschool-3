// export LD_LIBRARY_PATH=$C3_OTHER/Root/lib/
// g++ test.cpp -I $C3_OTHER/Root/include $C3_OTHER/Root/lib/lib{Core,Cint,Gpad,Graf,Hist,Matrix,Tree}.so -o testit

#include <stdio.h>

#include "TObject.h"
#include "TFile.h"
#include "TList.h"
#include "TDirectory.h"
#include "TNtuple.h"
#include "TLeafF.h"
#include "TIterator.h"
#include "TKey.h"

int main(int argc, char* argv[]) {
   TObject* tmp;

   TFile* file = new TFile("lauras.root");
//    TList* ls = file->GetListOfKeys();
//    TIterator* iter = ls->MakeIterator();
//    do {
//       TKey* key = (TKey*)((*iter)());
//       TObject* obj = key->ReadObj();

//       printf("pointer: %d, name: %s, type: %s\n", obj, obj->GetName(), obj->ClassName());
//    } while (iter->Next() != NULL);
//    printf("pointer to directory: %d\n", file->Get("DtoPiLNuProc"));
   
   TDirectory* dir = (TDirectory*)(file->Get("DtoPiLNuProc"));
   TIterator* iter = dir->GetListOfKeys()->MakeIterator();
   TNtuple* ntuple;
   while (TKey* key = (TKey*)(iter->Next())) {
      TObject* obj = key->ReadObj();
      ntuple = (TNtuple*)(obj);

      printf("pointer: %d, name: %s, title: %s, type: %s\n", obj, obj->GetName(), obj->GetTitle(), obj->ClassName());
   }

   long entries = ntuple->GetEntries();
   printf("Name = %s, Nvar = %d, entries = %d\n", ntuple->GetName(), ntuple->GetNvar(), entries);
   iter = ntuple->GetListOfLeaves()->MakeIterator();
   TLeafF* favorite = NULL;
//    while (TLeafF* leaf = (TLeafF*)(iter->Next())) {
// //     printf("pointer: %d, name: %s, title: %s, type: %s\n", leaf, leaf->GetName(), leaf->GetTitle(), leaf->ClassName());
//       if (strcmp(leaf->GetName(), "deltae") == 0) { favorite = leaf; break; }
//    }
   favorite = (TLeafF*)(ntuple->GetLeaf("deltae"));

   ntuple->SetBranchStatus("*", 0);
   ntuple->SetBranchStatus("deltae", 1);

   TNtuple* nt2 = (TNtuple*)(file->FindObjectAny("nt2"));
   nt2->SetBranchStatus("*", 1);

   for (long i = 0;  i < entries;  i++) {
      int dataread = ntuple->GetEntry(i, 0);
      printf("dataread: %d, name: %s value: %g\n", dataread, favorite->GetName(), favorite->GetValue());
   }


}
