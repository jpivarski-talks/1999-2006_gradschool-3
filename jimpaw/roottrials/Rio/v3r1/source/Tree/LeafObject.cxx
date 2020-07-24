// this :
#include <Rio/Tree/LeafObject.h>

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Interfaces/IClass.h>
#include <Rio/Interfaces/IDictionary.h>
#include <Rio/Interfaces/IDirectory.h>
#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Out.h>
#include <Rio/Core/Arguments.h>
#include <Rio/Tree/Branch.h>
#include <Rio/Tree/Tree.h>

RIO_META_SOURCE(Rio::LeafObject)
//////////////////////////////////////////////////////////////////////////////
Rio::LeafObject::LeafObject(
 Branch& aBranch
)
:BaseLeaf(aBranch)
,fClass(0)
,fObjAddress(0)
,fVirtual(true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
Rio::LeafObject::LeafObject(
 Branch& aBranch
,const std::string& aName
,IClass& aClass
)
:BaseLeaf(aBranch,aName)
,fClass(&aClass)
,fObjAddress(0)
,fVirtual(true)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  setTitle(aClass.name());
}
//////////////////////////////////////////////////////////////////////////////
Rio::LeafObject::~LeafObject(
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
}
//////////////////////////////////////////////////////////////////////////////
double Rio::LeafObject::value(
 int
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::LeafObject::valuePointer(
) const 
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{ 
  return 0;
}
//////////////////////////////////////////////////////////////////////////////
Rio::IObject* Rio::LeafObject::object(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  return *fObjAddress;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::LeafObject::fillBasket(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Pack leaf elements in Basket output buffer
  if (!fObjAddress) return;
  IObject* obj  = object(); 
  if (obj) {
    if (fVirtual) {
      unsigned char n = obj->isA().name().size();
      aBuffer.write(n);
      aBuffer.writeFastArray(obj->isA().name().c_str(),n+1);
    }
    obj->stream(aBuffer);
  } else {
    Out out(branch().printer());
    out << "Rio::LeafObject::fillBasket : why do we pass here ? " << endl;
    if (fClass) {
      /*FIXME      if (fClass->Property() & kIsAbstract) {
	obj = new Object;
	} else FIXME */ {
	obj = fClass->create(Arguments());
      }
	//FIXME obj->SetBit(kInvalidObject);
	//FIXME obj->SetUniqueID(123456789);
      obj->stream(aBuffer);
      delete obj;
    } else {
      Out out(branch().printer());
      out << "Rio::LeafObject::fillBasket "
	  << "Attempt to write a NULL object in leaf:\"" 
	  << name() << "\"." << endl;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////
void Rio::LeafObject::readBasket(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  // Read leaf elements from Basket input buffer
  if (fVirtual) {
    unsigned char n;
    aBuffer.read(n);
    char classname[128];
    aBuffer.readFastArray(classname,n+1);
    IDictionary& dico = branch().tree().directory().file().dictionary();
    fClass = dico.findClass(title());
  }
  if (fClass) {
    IObject* object = 0;
    if (!fObjAddress) {
      //Out out(branch().printer());
      //out << "Rio::LeafObject::readBasket : " 
      //  << "debug : create object for class " << fClass->name() << endl;
      //FIXME : who delete the below ?
      typedef IObject* Pointer;
      fObjAddress = new Pointer;
      //FIXME : who delete the returned object ?
      Arguments args;
      Args_insertDirectory(args,&(branch().tree().directory()));
      *fObjAddress = (IObject*)fClass->create(args);
    }
    object = *fObjAddress;
    if (branch().isAutoDelete()) {
      Out out(branch().printer());
      out << "Rio::LeafObject::readBasket : debug : autodelete "  << endl;
      delete object;
      Arguments args;
      Args_insertDirectory(args,&(branch().tree().directory()));
      object = (IObject*)fClass->create(args);
    }
    if (!object) {
      Out out(branch().printer());
      out << "Rio::LeafObject::readBasket : " 
	  << "unable to get an object for class " << fClass->name() << endl;
      return;
    }
    object->stream(aBuffer);
    // in case we had written a null pointer a Zombie object was created
    // we must delete it
    /*FIXME
    if (object->TestBit(kInvalidObject)) {
      if (object->GetUniqueID() == 123456789) {
	delete object;
	object = 0;
      }
    }
    */
    *fObjAddress = object;
  } else {
    Out out(branch().printer());
    out << "Rio::LeafObject::readBasket : debug : no class !"  << endl;
    branch().setAddress(0);
  }
}
/*
//////////////////////////////////////////////////////////////////////////////
TMethodCall* Rio::LeafObject::GetMethodCall(const char *name)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
// Returns pointer to method corresponding to name
//    name is a string with the general form  "method(list of params)"
//   If list of params is omitted, () is assumed;
//

   char *namecpy = new char[strlen(name)+1];
   strcpy(namecpy,name);
   char *params = strchr(namecpy,'(');
   if (params) { *params = 0; params++; }
   else params = ")";

   if (!fClass) fClass      = gROOT->GetClass(GetTitle());
   TMethodCall *m = new TMethodCall(fClass, namecpy, params);
   delete [] namecpy;
   if (m->GetMethod()) return m;
   Error("GetMethodCall","Unknown method:%s",name);
   delete m;
   return 0;
}
//////////////////////////////////////////////////////////////////////////////
const std::string& Rio::LeafObject::typeName(
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
   return fTitle;
}
*/
//////////////////////////////////////////////////////////////////////////////
void Rio::LeafObject::printValue(
 IPrinter& aPrinter
,int
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  Out out(aPrinter);
  //out << (unsigned long)valuePointer() << endl;
  IObject* obj = object();
  if(obj)
    out << obj << " " << obj->isA().name() << " " << obj->name() << endl;
  else 
    out << obj << endl;
}
//////////////////////////////////////////////////////////////////////////////
void Rio::LeafObject::setAddress(
 void* aAddress
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  fObjAddress = (IObject**)aAddress;
}
//////////////////////////////////////////////////////////////////////////////
void* Rio::LeafObject::cast(
 const IClass& aClass
) const
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if(&aClass==sClass) return (LeafObject*)this;
  else return BaseLeaf::cast(aClass);
}
//////////////////////////////////////////////////////////////////////////////
void Rio::LeafObject::stream(
 IBuffer& aBuffer
)
//////////////////////////////////////////////////////////////////////////////
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!//
{
  if (aBuffer.isReading()) {
    unsigned int s,c;
    /*short v = */ aBuffer.readVersion(&s,&c);
    /*
    if (R__v > 3 || R__v == 2) {
      TLeafObject::Class()->ReadBuffer(b, this, R__v, R__s, R__c);
      fObjAddress = 0;
      fClass  = gROOT->GetClass(fTitle.Data());
      if (!fClass) Warning("Streamer","Cannot find class:%s",fTitle.Data());
      return;
    }
    */
    //====process old versions before automatic schema evolution
    BaseLeaf::stream(aBuffer);
    fObjAddress = 0;
    IDictionary& dico = branch().tree().directory().file().dictionary();
    fClass = dico.findClass(fTitle);
    aBuffer.read(fVirtual);
    aBuffer.checkByteCount(s,c,sClass->name());
    //====end of old versions
  } else {
    //FIXME TLeafObject::Class()->WriteBuffer(b,this);
    unsigned int c = aBuffer.writeVersion(sClass->version(),true);
    BaseLeaf::stream(aBuffer);
    aBuffer.write(fVirtual);
    aBuffer.setByteCount(c,true);
  }
}
