/*
 * Here we have a simple example showing how to :
 * - declare a user class (MyObject class) suitable for doing IO with Rio.
 * - declare the user class (with the MyObjectClass class) 
 *   to the user dictionary (MyDictionary class).
 * - open a file in write then in read mode using the user dictionary.
 * - write and read some data in the file.
 * - organize the file by using the Rio::Directory.
 * 
 * Stored data are instances of the MyObject class and also
 * simple data types (int, float) declared to a Rio::Tree.
 * 
 * To use this program :
 *    OS> Rio_test.exe -create 
 *      to create the hello.root file.
 *    OS> Rio_test.exe -read
 *      to read the hello.root file.
 */

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IDirectory.h>

#include <Rio/Core/Class.h>
#include <Rio/Core/Printer.h>
#include <Rio/Core/File.h>
#include <Rio/Core/tobuf.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Debug.h>
#include <Rio/Core/Out.h>

#include <Rio/Tree/TreeDictionary.h>
#include <Rio/Tree/Tree.h>
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/BaseLeaf.h>
#include <Rio/Tree/LeafObject.h>

#include <iostream>

/* 
 * A class to declare to the IO system. 
 */
class MyObject : public Rio::Named {
  /* 
   * A cpp macro to help setting the "introspection"
   * for the MyObject class.
   */
  RIO_META_HEADER(MyObject)
public: //Rio::IObject:
  /*
   * A cast method that permits to avoid good part of the
   * dynamic_cast (and then be more efficient).
   */
  virtual void* cast(const Rio::IClass& aClass) const {
    if(&aClass==sClass) return (MyObject*)this;
    else return Named::cast(aClass);
  }
  /*
   * The famous streamer. Here you say what you want to store
   * and retreive of your objects.
   */
  virtual void stream(Rio::IBuffer& aBuffer) {
    if (aBuffer.isReading()) {
      /*short v = */ aBuffer.readVersion();
      Named::stream(aBuffer);
      aBuffer.read(fMyInt);
      if(fVerbose) 
	std::cout << "MyObject::stream : " << this << " name : " << fName
		  << " read : " << fMyInt << std::endl;
    } else {
      if(fVerbose)
	std::cout << "MyObject::stream : " << this
		  << " write : " << fMyInt << std::endl;
      aBuffer.writeVersion(sClass->version());
      Named::stream(aBuffer);
      aBuffer.write(fMyInt);
    }
  }
public:
  MyObject(const std::string& aName = "",bool aVerbose = false)
    :Rio::Named(aName,"My object")
    ,fVerbose(aVerbose),fMyInt(314) {
    if(fVerbose) 
      std::cout << "MyObject::MyObject : called for " 
		<< this << " ." << std::endl;
  }
  virtual ~MyObject(){
    if(fVerbose) 
      std::cout << "MyObject::~MyObject : called for " 
		<< this << " name : " << fName << std::endl;
  }
  int myInt () const { return fMyInt;}
  void setMyInt(int aInt){ fMyInt = aInt;}
private:
  bool fVerbose;
  /* 
   * The data !
   */
  int fMyInt;
};

/*
 * A cpp macro to help setting the "introspection"
 * for the MyObject class.
 */
RIO_META_SOURCE(MyObject)

/*
 * A class to describe the MyObject class. It consists
 * mainly to a factory. One instance of the MyObjectClass
 * must be declared to your dictionary.
 */
class MyObjectClass : public Rio::Class {
public: //Rio::IClass
  virtual Rio::IObject* create(const Rio::Arguments& aArgs) {
    Rio::IDirectory* directory = 
      (Rio::IDirectory*)Rio::Args_findDirectory(aArgs);
    if(!directory) {
      Rio::Out out(fPrinter);
      out << "MyObjectClass::create : no directory given." << Rio::endl;
      return 0;
    }
    Rio::IObject* object = 
      new MyObject("",directory->file().verboseLevel()?true:false);
    directory->appendObject(object);
    return object;
  }
public:
  /*
   * The declared class name is "TFoo" (it could be "MyObject").
   * The declared class version is zero. The class version
   * could be used in the MyObject::streamer for further evolution
   * of the MyObject class (handling of the "schema evolution").
   */
  MyObjectClass(Rio::IPrinter& aPrinter):
    Rio::Class(aPrinter,"TFoo",0){}
  virtual ~MyObjectClass(){}
};

/*
 * Create the dictionary. Your dictionary should, in some way or another
 * (inheritance or composition) give access to an instance of the 
 * Rio::TreeDictionary. The simplest is to inherit.
 */
class MyDictionary : public Rio::TreeDictionary {
public:
  MyDictionary(Rio::IPrinter& aPrinter,bool aVerbose)
    :Rio::TreeDictionary(aPrinter){
    /*
     * Create and declare an instance of the MyObject class
     * describer to the dictionary. Setup (with MyObject::setClass)
     * the introspection for the MyObject class.
     */
    Rio::IClass* cl = new MyObjectClass(fPrinter);
    MyObject::setClass(cl);
    fClasses.push_back(cl);
    if(aVerbose) print();
  }
  virtual ~MyDictionary(){}
};


int main(int aArgc,char* aArgv[]) {

  /*
   * Create a printer (concreete instance of the Rio::IPrinter)
   * that will be declared to the system. It will be used 
   * for error messages, debug informations, etc...
   */
  Rio::Printer printer;

  /* 
   * Create a "cout like" printer.
   */
  Rio::Out out(printer);

 {std::string arg1 = (aArgc>=2) ? aArgv[1] : "";
  std::string arg2 = (aArgc>=3) ? aArgv[2] : "";

  /*
   * Instanciate the dictionary.
   */
  bool verbose = (arg2=="-verbose" ?true:false);
  MyDictionary myDictionary(printer,verbose);

  if(arg1=="-create") {

    /*
     * Create a file. The dictionary and a printer is given here.
     */
    Rio::IFile& file = 
      *(new Rio::File("hello.root",Rio::File::CREATE,myDictionary,printer));
    if(arg2=="-verbose") file.setVerboseLevel(1);

    /*
     * Create some directory and put some data in them.
     */
    Rio::IDirectory* d_histos = file.directory().mkdir("histos","histograms");
    Rio::IDirectory* d_ecal = d_histos->mkdir("ecal");
    if(d_ecal) {
      d_ecal->appendObject(new MyObject("myObject",verbose));
      d_ecal->appendObject(new MyObject("myObject1",verbose));
      d_ecal->appendObject(new MyObject("myObject2",verbose));
      /*
       * MyObject instances will be deleted by the d_ecal directory destructor.
       */ 
    }
    d_histos->mkdir("hcal");
    Rio::IDirectory* d_trees = file.directory().mkdir("trees");

    Rio::Tree* myTree = new Rio::Tree(*d_trees,"myTree","My tree");
    /*
     * Rio::Tree instances will be deleted by the d_trees directory destructor.
     */ 
    
    /*
     * Some data to put in the tree :
     */
    static struct {
      int v1;
      /* WIN32 : inserting a double involves that all fields
       * are mapped to the size of the double ! then the branch
       * computation of each field address is out !
       * (It was true with ROOT too...)
       *double v2;
       */
      float v3;
    } st;
    float v4;

    /*
     * Create branches.
     */
    //tree->createBranch("st",&st,"v1/I:v2/D:v3/F");
    myTree->createBranch("st",&st,"v1/I:v3/F");
    myTree->createBranch("v4",&v4,"v4/F");

    /**
     * We have to delete the below object ourself.
     */
    MyObject* myObject = new MyObject("myObject3",verbose);
    /**
     * Take care, we have to pass the address of a IObject* 
     * (and not of a MyObject*). (Simple no ?).
     */
    Rio::IObject* obj = (Rio::IObject*)myObject;
    myTree->createBranch("myObjects",(Rio::IClass&)MyObject::is(),&obj);

    /*
     * Fill the tree.
     */
    for(int count=0;count<50000;count++) {
      st.v1 = count;
      //st.v2 = 2 * count;
      st.v3 = 3.0F * count;
      v4 = 4.0F * count;

      myObject->setMyInt(5 * count);

      myTree->fill();
    }
    
    // file.directory().remove("histos;*");

    file.directory().print(printer);

    /*
     * Write data in the file.
     */
    file.write();

    /*
     * Close the file. Close does not write !
     */ 
    file.close();

    delete &file;

    delete myObject;

  } else if(arg1=="-recreate") {
    Rio::File file("hello.root",Rio::File::RECREATE,myDictionary,printer);
    file.close();
  } else if(arg1=="-update") {
    Rio::File file("hello.root",Rio::File::UPDATE,myDictionary,printer);
    file.close();
  } else if(arg1=="-read") {

    /*
     * Open a file for reading. Here too, the dictionary and a printer
     * is given. The "create" method of the various IClass instances
     * declared to the dictionary will be used to create back instances
     * of Rio::Directory, Rio::Tree and also of the user MyObject class.
     */
    Rio::IFile& file = *(new Rio::File("hello.root",Rio::File::READ,myDictionary,printer));
    if(arg2=="-verbose") file.setVerboseLevel(1);
    file.directory().print(printer);

    /* 
     * Get back directories and data.
     */
    Rio::IDirectory* d_histos = file.directory().findDirectory("histos");
    if(!d_histos) {
      out << " Directory \"histos\" not found." << Rio::endl;
    } else {
      //d_histos->ls();
      Rio::IDirectory* d_ecal = d_histos->findDirectory("ecal");
      if(!d_ecal) {
	out << " Directory \"ecal\" not found." << Rio::endl;
      } else {
	//d_ecal->ls();
	/*
	 * Returned object is managed by the file, we do not
	 * have to delete it ourself.
	 */
	MyObject* myObject = 
	  Rio::cast<MyObject>(d_ecal->find("myObject;1"));
	if(!myObject) {
	  out << " Object \"myObject\" not found." << Rio::endl;
	} else {
	  if(myObject->myInt()==314) 
	    out << " myObject.fMyInt " << myObject->myInt() 
		<< " : Ouai !!!!." << Rio::endl;
	  else
	    out << " myObject.fMyInt " << myObject->myInt()
		<< " : should be 314 ; beurk !!!!." << Rio::endl;
	}
	MyObject* myObject2 = 
	  Rio::cast<MyObject>(d_ecal->find("myObject2"));
	if(!myObject2) {
	  out << " Object \"myObject2\" not found." << Rio::endl;
	}
      }
    }

    Rio::IDirectory* d_trees = file.directory().findDirectory("trees");
    if(!d_trees) {
      out << " Directory \"trees\" not found." << Rio::endl;
    } else {
      Rio::Tree* tree = Rio::cast<Rio::Tree>(d_trees->find("myTree"));
      if(!tree) {
	out << " Object \"myTree\" not found." << Rio::endl;
      } else {
	int number = (int)tree->entries();
	out << " Object \"myTree\" found (" << number 
	    << " entries)." << Rio::endl;
	
	int index;
	out << "Head :" << Rio::endl;
	for(index=0;index<10;index++) tree->show(index);
	
	out << "Tail :" << Rio::endl;
	for(index=number-10;index<number;index++) tree->show(index);
	
	Rio::BaseLeaf* leaf_v3 = tree->leaf("v3");
	if(!leaf_v3) {
	  out << " Leaf \"v3\" not found." << Rio::endl;
	} else {
	  out << " Leaf \"v3\" found." << Rio::endl;
	  Rio::Branch& branch_v3 = leaf_v3->branch();
	  for(index=0;index<5;index++) {
	    if(branch_v3.entry(index)==0) continue;
	    float val = (float)leaf_v3->value();
	    out << "v3 " << index << " = " << val 
		<< " (" << 3*index << " expected)." << Rio::endl;
	  }
	  for(index=number-5;index<number;index++) {
	    if(branch_v3.entry(index)==0) continue;
	    float val = (float)leaf_v3->value();
	    out << "v3 " << index << " = " << val 
		<< " (" << 3*index << " expected)." << Rio::endl;
	  }
	}
	
	Rio::BaseLeaf* leaf_v4 = tree->leaf("v4");
	if(!leaf_v4) {
	  out << " Leaf \"v4\" not found." << Rio::endl;
	} else {
	  out << " Leaf \"v4\" found." << Rio::endl;
	  Rio::Branch& branch_v4 = leaf_v4->branch();
	  for(index=0;index<10;index++) {
	    if(branch_v4.entry(index)==0) continue;
	    float val = (float)leaf_v4->value();
	    out << "v4 " << index << " = " << val 
		<< " (" << 4*index << " expected)." << Rio::endl;
	  }
	  for(index=number-10;index<number;index++) {
	    if(branch_v4.entry(index)==0) continue;
	    float val = (float)leaf_v4->value();
	    out << "v4 " << index << " = " << val 
		<< " (" << 4*index << " expected)." << Rio::endl;
	  }
	}
	
	Rio::BaseLeaf* leaf_myObjects = tree->leaf("myObjects");
	if(!leaf_myObjects) {
	  out << " Leaf \"myObjects\" not found." << Rio::endl;
	} else {
	  out << " Leaf \"myObjects\" found." << Rio::endl;
	  Rio::Branch& branch_myObjects = leaf_myObjects->branch();
	  for(index=0;index<10;index++) {
	    if(branch_myObjects.entry(index)==0) continue;
	    Rio::IObject* obj = ((Rio::LeafObject*)leaf_myObjects)->object();
	    MyObject* myObj = Rio::cast<MyObject>(obj);
	    out << "obj " << index << " = " << myObj->myInt() 
		<< " (" << 5*index << " expected)." << Rio::endl;
	  }
	  for(index=number-10;index<number;index++) {
	    if(branch_myObjects.entry(index)==0) continue;
	    Rio::IObject* obj = ((Rio::LeafObject*)leaf_myObjects)->object();
	    MyObject* myObj = Rio::cast<MyObject>(obj);
	    out << "obj " << index << " = " << myObj->myInt() 
		<< " (" << 5*index << " expected)." << Rio::endl;
	  }
	}
	
      }
    

      // file.directory().remove("histos");
    }
    file.directory().print(printer);

    /*
     * Close the file.
     */
    file.close();

    delete &file;
  } else {
    out << "Bad argument : [-create, -read] [-verbose] expected." << Rio::endl;
  }

  }

 /*
  * To do a simple instance counting. It should produce zero message.
  */
  Rio::Debug::check(printer);

  return 0;
}
