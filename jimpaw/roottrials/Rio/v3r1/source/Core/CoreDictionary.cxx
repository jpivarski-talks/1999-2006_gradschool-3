// this :
#include <Rio/Core/CoreDictionary.h>

#include <Rio/Core/File.h>
#include <Rio/Core/Class.h>
#include <Rio/Core/StreamerInfo.h>
#include <Rio/Core/StreamerElement.h>
#include <Rio/Core/Directory.h>
#include <Rio/Core/List.h>
#include <Rio/Core/ObjArray.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Core/Out.h>

namespace Rio {

class DirectoryClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments& aArgs) {
    IDirectory* directory = (IDirectory*)Args_findDirectory(aArgs);
    if(!directory) {
      Out out(fPrinter);
      out << "Rio::DirectoryClass::create : no directory given." << endl;
      return 0;
    }
    File* file = (File*)directory->file().cast(File::is());
    return new Directory(*file,(Directory*)directory->cast(Directory::is()));
  }
public:
  DirectoryClass(IPrinter& aPrinter):Class(aPrinter,"TDirectory",1){}
  virtual ~DirectoryClass(){}
};

class ListClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments&) {return new List();}
public:
  ListClass(IPrinter& aPrinter):Class(aPrinter,"TList",4){}
  virtual ~ListClass(){}
};

class ObjArrayClass : public Class {
public: //Rio::IClass
  virtual IObject* create(const Arguments& aArgs) {
    ObjArray* obj = new ObjArray();
    obj->setReadArguments(aArgs);
    return obj;
  }
public:
  ObjArrayClass(IPrinter& aPrinter):Class(aPrinter,"TObjArray",3){}
  virtual ~ObjArrayClass(){}
};

#define RIO_CLASS_DECL(aName,aClassName,aVersion) \
class aName : public Class {\
public:\
  virtual IObject* create(const Arguments&) {return 0;}\
public:\
  aName(IPrinter& aPrinter):Class(aPrinter,aClassName,aVersion){}\
  virtual ~aName(){}\
}

RIO_CLASS_DECL(FileClass,"File",0);
RIO_CLASS_DECL(ObjectClass,"TObject",1);
RIO_CLASS_DECL(NamedClass,"TNamed",1);
RIO_CLASS_DECL(ArrayIntegerClass,"TArrayI",1);
RIO_CLASS_DECL(ArrayDoubleClass,"TArrayD",1);
RIO_CLASS_DECL(StreamerInfoClass,"TStreamerInfo",2);
RIO_CLASS_DECL(StreamerElementClass,"TStreamerElement",2);
RIO_CLASS_DECL(StreamerBaseClass,"TStreamerBase",3);
RIO_CLASS_DECL(StreamerStringClass,"TStreamerString",2);
RIO_CLASS_DECL(StreamerBasicTypeClass,"TStreamerBasicType",2);
RIO_CLASS_DECL(StreamerBasicPointerClass,"TStreamerBasicPointer",2);
RIO_CLASS_DECL(StreamerObjectClass,"TStreamerObject",2);
RIO_CLASS_DECL(StreamerObjectPointerClass,"TStreamerObjectPointer",2);
RIO_CLASS_DECL(StreamerObjectAnyClass,"TStreamerObjectAny",2);

}

//////////////////////////////////////////////////////////////////////////////
Rio::CoreDictionary::CoreDictionary(
 IPrinter& aPrinter
) 
:fPrinter(aPrinter)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  ///////////////////////////////////////////////////////////////////////
  // Classes :
  ///////////////////////////////////////////////////////////////////////
  IClass* clNamed = new NamedClass(fPrinter);
  Named::setClass(clNamed);
  fClasses.push_back(clNamed);
  IClass* clSI = new StreamerInfoClass(fPrinter);
  StreamerInfo::setClass(clSI);
  fClasses.push_back(clSI);
  IClass* clSE = new StreamerElementClass(fPrinter);
  StreamerElement::setClass(clSE);
  fClasses.push_back(clSE);
  IClass* clSB = new StreamerBaseClass(fPrinter);
  StreamerBase::setClass(clSB);
  fClasses.push_back(clSB);
  IClass* clSS = new StreamerStringClass(fPrinter);
  StreamerString::setClass(clSS);
  fClasses.push_back(clSS);
  IClass* clSBT = new StreamerBasicTypeClass(fPrinter);
  StreamerBasicType::setClass(clSBT);
  fClasses.push_back(clSBT);
  IClass* clSBP = new StreamerBasicPointerClass(fPrinter);
  StreamerBasicPointer::setClass(clSBP);
  fClasses.push_back(clSBP);
  IClass* clSO = new StreamerObjectClass(fPrinter);
  StreamerObject::setClass(clSO);
  fClasses.push_back(clSO);
  IClass* clSOP = new StreamerObjectPointerClass(fPrinter);
  StreamerObjectPointer::setClass(clSOP);
  fClasses.push_back(clSOP);
  IClass* clSOA = new StreamerObjectAnyClass(fPrinter);
  StreamerObjectAny::setClass(clSOA);
  fClasses.push_back(clSOA);

  // Has no streamer info. 
  IClass* clObject = new ObjectClass(fPrinter);
  Object::setClass(clObject);
  fClasses.push_back(clObject);

  // Has no streamer info. 
  IClass* clDir = new DirectoryClass(fPrinter);
  Directory::setClass(clDir);
  fClasses.push_back(clDir);

  // Has no streamer info. 
  IClass* clOA = new ObjArrayClass(fPrinter);
  ObjArray::setClass(clOA);
  fClasses.push_back(clOA);

  // Has no streamer info. 
  IClass* clList = new ListClass(fPrinter);
  List::setClass(clList);
  fClasses.push_back(clList);

  // Has no streamer info. 
  IClass* clFile = new FileClass(fPrinter);
  File::setClass(clFile);
  fClasses.push_back(clFile);

  // Has no streamer info. 
  fClasses.push_back(new ArrayIntegerClass(fPrinter));

  // Has no streamer info. 
  fClasses.push_back(new ArrayDoubleClass(fPrinter));

  ///////////////////////////////////////////////////////////////////////
  // StreamerInfos :
  ///////////////////////////////////////////////////////////////////////
  StreamerInfo* streamerInfo = new StreamerInfo(*clNamed,"",-68599943);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase("TObject","Basic ROOT object",0,Object::is().version()));
  streamerInfo->add(new StreamerString("fName","object identifier",12));
  streamerInfo->add(new StreamerString("fTitle","object title",20));

  streamerInfo = new StreamerInfo(*clSI,"",357609799);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TNamed","The basis for a named object (name, title)",0,Named::is().version()));
  streamerInfo->add(new StreamerBasicType
("fCheckSum","checksum of original class",28,StreamerInfo::UNSIGNED_INTEGER,"UInt_t"));
  streamerInfo->add(new StreamerBasicType
("fClassVersion","Class version identifier",32,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerObjectPointer
("fElements","Array of TStreamerElements",76,"TObjArray*"));

  streamerInfo = new StreamerInfo(*clSE,"",-1925148838);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TNamed","The basis for a named object (name, title)",0,Named::is().version()));
  streamerInfo->add(new StreamerBasicType
("fType","element type",28,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fSize","sizeof element",32,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fArrayLength","cumulative size of all array dims",36,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerBasicType
("fArrayDim","number of array dimensions",40,StreamerInfo::INTEGER,"Int_t"));
 {StreamerElement* streamerElement = new StreamerBasicType
("fMaxIndex","Maximum array index for array dimension \"dim\"",44,StreamerInfo::INTEGER,"Int_t");
  streamerInfo->add(streamerElement);
  streamerElement->setArrayDimension(1);
  streamerElement->setMaxIndex(0,5);}
  streamerInfo->add(new StreamerString
("fTypeName","Data type name of data member",72));

  streamerInfo = new StreamerInfo(*clSB,"",-1623888782);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is().version()));
  streamerInfo->add(new StreamerBasicType
("fBaseVersion","version number of the base class",88,StreamerInfo::INTEGER,"Int_t"));

  streamerInfo = new StreamerInfo(*clSBT,"",-1293091330);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase("TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is().version()));

  streamerInfo = new StreamerInfo(*clSBP,"",1587298059);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase
("TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is().version()));
  streamerInfo->add(new StreamerBasicType
("fCountVersion","version number of the class with the counter",88,StreamerInfo::INTEGER,"Int_t"));
  streamerInfo->add(new StreamerString
("fCountName","name of data member holding the array count",92));
  streamerInfo->add(new StreamerString
("fCountClass","name of the class with the counter",100));

  streamerInfo = new StreamerInfo(*clSS,"",-1769387431);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase("TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is().version()));

  streamerInfo = new StreamerInfo(*clSO,"",-2117510581);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase("TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is().version()));

  streamerInfo = new StreamerInfo(*clSOP,"",720556968);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase("TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is().version()));

  streamerInfo = new StreamerInfo(*clSOA,"",-1186087169);
  fStreamerInfos.push_back(streamerInfo);
  // Elements :
  streamerInfo->add(new StreamerBase("TStreamerElement","base class for one element (data member) to be Streamed",0,StreamerElement::is().version()));

}
//////////////////////////////////////////////////////////////////////////////
Rio::CoreDictionary::~CoreDictionary(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
 {std::list<IClass*>::iterator it;
  for(it=fClasses.begin();it!=fClasses.end();it=fClasses.erase(it)) 
    delete (*it);}

 {std::list<IObject*>::iterator it;
  for(it=fStreamerInfos.begin();
      it!=fStreamerInfos.end();
      it=fStreamerInfos.erase(it)) 
    delete (*it);}
}
//////////////////////////////////////////////////////////////////////////////
Rio::IClass* Rio::CoreDictionary::findClass(
 const std::string& aName
) 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<IClass*>::const_iterator it;
  for(it=fClasses.begin();it!=fClasses.end();++it) {
    if((*it)->name()==aName) return *it;
  }
  Out out(fPrinter);
  out << "Rio::CoreDictionary::findClass : \"" 
      << aName << "\" not found." << endl;
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
const std::list<Rio::IObject*>& Rio::CoreDictionary::streamerInfos(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return fStreamerInfos;
} 
//////////////////////////////////////////////////////////////////////////////
void Rio::CoreDictionary::print(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  std::list<Rio::IObject*>::const_iterator it;
  for(it=fStreamerInfos.begin();it!=fStreamerInfos.end();++it)
    (*it)->print(fPrinter);
  
  std::list<Rio::IClass*>::const_iterator itc;
  Out out(fPrinter);
  out << "Dictionary has " << fClasses.size() << " entries :" << endl;
  for(itc=fClasses.begin();itc!=fClasses.end();++itc) {
    out << " - class \"" << (*itc)->name() << "\", version " 
	<< (*itc)->version() << endl;
  }

}
//////////////////////////////////////////////////////////////////////////////
void Rio::CoreDictionary::addClass(
 IClass* aClass				
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fClasses.push_back(aClass);
}

/* ROOT tag v3-00-06 :
debug : TStreamerInfo : +++++ TNamed -68599943
debug : TStreamerInfo : +++++ TStreamerInfo 357609799
debug : TStreamerInfo : +++++ TStreamerBase -1623888782
debug : TStreamerInfo : +++++ TStreamerElement -1925148838
debug : TStreamerInfo : +++++ TStreamerBasicType -1293091330
debug : TStreamerInfo : +++++ TStreamerObject -2117510581
debug : TStreamerInfo : +++++ TStreamerObjectAny -1186087169
debug : TStreamerInfo : +++++ TStreamerString -1769387431
debug : TStreamerInfo : +++++ TStreamerBasicPointer 1587298059
debug : TStreamerInfo : +++++ TStreamerObjectPointer 720556968
*/
