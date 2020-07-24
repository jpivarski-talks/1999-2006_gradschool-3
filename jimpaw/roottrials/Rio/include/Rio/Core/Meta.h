#ifndef Rio_Meta_h
#define Rio_Meta_h

namespace Rio {class IClass;}

#define RIO_META_HEADER(a_Class) \
public:\
  static const Rio::IClass& is();\
  static void setClass(Rio::IClass*);\
public:\
  virtual const Rio::IClass& isA() const;\
private:\
  static Rio::IClass* sClass;


#define RIO_META_SOURCE(a_Class) \
Rio::IClass* a_Class::sClass = 0;\
void a_Class::setClass(Rio::IClass* aClass) { if(!sClass) sClass = aClass;}\
const Rio::IClass& a_Class::isA() const {return *sClass;}\
const Rio::IClass& a_Class::is() {return *sClass;}

#endif
