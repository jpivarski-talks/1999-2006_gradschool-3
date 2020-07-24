// g++ test.cpp -I Rio/v3r1/ Rio/v3r1/Linux-c++/*.o -o testit && ./testit

#include <iostream>
#include <string>

#include "Rio/Interfaces/IObject.h"
#include "Rio/Interfaces/IClass.h"
#include "Rio/Interfaces/IDirectory.h"
#include "Rio/Interfaces/IKey.h"
#include "Rio/Core/Arguments.h"
#include "Rio/Core/Class.h"
#include "Rio/Core/File.h"
#include "Rio/Core/Printer.h"
#include "Rio/Tree/TreeDictionary.h"
#include "Rio/Tree/Tree.h"

class TNtuple : public Rio::Tree {
      RIO_META_HEADER(TNtuple);
   public:
      virtual void* cast(const Rio::IClass& aClass) const {
	 if (&aClass == sClass) return (TNtuple*)this;
	 else return Tree::cast(aClass);
      }
      virtual void stream(Rio::IBuffer& aBuffer) {
	 if (aBuffer.isReading()) {
	    unsigned int R__s, R__c;
	    short R__v = aBuffer.readVersion(&R__s, &R__c);
	    std::cout << "version " << R__v << " start " << R__s << " count " << R__c << std::endl;
	    Rio::Tree::stream(aBuffer);
	    std::cout << "yay!" << std::endl;

	    int m_nvar;
	    aBuffer.read(m_nvar);
	    aBuffer.checkByteCount(R__s , R__c, "TNtuple");
	    std::cout << "nvar = " << m_nvar << std::endl;
	 }
	 else { // isWriting
	    std::cout << "you're out of luck" << std::endl;
	 }
      }
   public:
      TNtuple(Rio::IDirectory& aDirectory) : Rio::Tree(aDirectory) {};
      ~TNtuple() {};
};

RIO_META_SOURCE(TNtuple);

class TNtupleClass : public Rio::Class {
   public:
      TNtupleClass(Rio::IPrinter& aPrinter): Rio::Class(aPrinter, "TNtuple", 0) {}
      virtual ~TNtupleClass() {}

      virtual Rio::IObject* create(const Rio::Arguments& aArgs) {
	 Rio::IDirectory* directory = (Rio::IDirectory*)Rio::Args_findDirectory(aArgs);
	 if (!directory) return 0;
	 Rio::IObject* object = new TNtuple(*directory);
	 directory->appendObject(object);
	 return object;
      }
};

class MyDictionary : public Rio::TreeDictionary {
   public:
      MyDictionary(Rio::IPrinter& aPrinter) : Rio::TreeDictionary(aPrinter) {
	 Rio::IClass* cl = new TNtupleClass(fPrinter);
	 TNtuple::setClass(cl);
	 fClasses.push_back(cl);
      }
      virtual ~MyDictionary() {}
};

int main(int argc, char* argv[]) {

   Rio::Printer printer;
   MyDictionary dictionary(printer);
   Rio::IFile& file = *(new Rio::File("lauras.root", Rio::File::READ, dictionary, printer));

   file.directory().print(printer);

   Rio::IDirectory* pilnuproc = file.directory().findDirectory("DtoPiLNuProc");

   std::vector<Rio::IKey*> keys = pilnuproc->keys();
   for (int i = 0;  i < keys.size();  i++) {
      std::cout << "key: " << keys[i]->name() << " " << keys[i]->title() << " " << keys[i]->className() << std::endl;
   }

   TNtuple* ntuple = Rio::cast<TNtuple>(pilnuproc->find("nt2"));

   if (!ntuple) {
      std::cout << "not found!" << std::endl;
   }
   else {
      std::cout << "found!" << std::endl;
   }



//   file.close();
//   delete& file;
}
