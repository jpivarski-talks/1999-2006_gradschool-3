#ifndef Rio_ObjArrayT_h
#define Rio_ObjArrayT_h

#include <Rio/Interfaces/IBuffer.h>
#include <Rio/Core/Out.h>

#include <list>

namespace Rio {

template <class T,class IT> //FIXME : rm class IT argument.
class ObjArrayT : public std::list<T*> {
public:
  ObjArrayT(IPrinter& aPrinter):fPrinter(aPrinter){}
  ObjArrayT(IPrinter& aPrinter,int aSize,T* aValue):std::list<T*>(aSize,aValue),fPrinter(aPrinter){}
  virtual ~ObjArrayT(){}
  T*& operator[](int aIndex){
    // g++ 2.95.2 : the below does not compile :
    //std::list<T>::iterator it;
    IT it;
    int index = 0;
    for(it=begin();it!=end();++it,++index) {
      if(index==aIndex) return *it;
    }
    Out out(fPrinter);
    out << "Rio::ObjArrayT::operator[] : bad index " 
	<< aIndex << " (size " << size() << ")" << endl;
    return *begin();
  }
  void read(IBuffer& aBuffer,const std::vector<std::pair<int,void*> >& aArgs,bool aNoNull = true){
    if (!aBuffer.isReading()) return;
    unsigned int s, c;
    aBuffer.readVersion(&s, &c);
    Object dummy_object;
    dummy_object.stream(aBuffer);
    std::string name;
    aBuffer.read(name);
    int nobjects;
    aBuffer.read(nobjects);
    int lowerBound;
    aBuffer.read(lowerBound);
    clear();
    for (int i = 0; i < nobjects; i++) {
      IObject* obj = aBuffer.readObject(aArgs);
      if(obj) {
	T* t = Rio::cast<T>(obj);
	if(!t) {
	  Out out(fPrinter);
	  out << "Rio::ObjArrayT::read : bad object class." << endl;
	} else {
	  push_back(t);
	}
      } else {
	if(!aNoNull) push_back(0);
      }
    }
    aBuffer.checkByteCount(s, c,"TObjArray");
  }
  void write(IBuffer& aBuffer){
    if (aBuffer.isReading()) return;
#ifndef Rio_ObjectArray_Class_Version
#define Rio_ObjectArray_Class_Version 3
#endif
    unsigned int c = aBuffer.writeVersion(Rio_ObjectArray_Class_Version,true);
    Object dummy_object;
    dummy_object.stream(aBuffer);
    aBuffer.write(std::string(""));
    int nobjects = size();
    aBuffer.write(nobjects);
    //debug int lowerBound = 211;
    int lowerBound = 0;
    aBuffer.write(lowerBound);
    //std::list<T>::iterator it;
    IT it;
    for(it=begin();it!=end();++it) {
      aBuffer.writeObject(*it);
    }
    aBuffer.setByteCount(c,true);
  }
private:
  IPrinter& fPrinter;
};

}

#endif
