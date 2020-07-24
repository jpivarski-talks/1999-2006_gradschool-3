#ifndef Rio_smanip_h
#define Rio_smanip_h

#include <string>
#include <vector>

namespace Rio {

class smanip {
public:
 enum what { leading, trailing, both };
 void strip(std::string&,what = both,char = ' ');
 bool tolong(const std::string&,long&);
 void words(const std::string&,const std::string&,std::vector<std::string>&);
 bool pathNameSuffix(const std::string&,
		     std::string&,std::string&,std::string&);
 bool match(const std::string&,const std::string&);
 bool printf(std::string&,int,const char*,...);
};

}

#endif
