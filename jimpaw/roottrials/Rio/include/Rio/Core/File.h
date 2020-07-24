#ifndef Rio_File_h
#define Rio_File_h

#include <string>
#include <list>

#include <Rio/Interfaces/IFile.h>

#include <Rio/Core/Seek.h>
#include <Rio/Core/Meta.h>

namespace Rio {

class IDictionary;
class IPrinter;
class Directory;
class FreeSegment;

class File : public virtual IFile {
  RIO_META_HEADER(File)
public: //Rio::IFile
  virtual const std::string& name() const;
  virtual IDictionary& dictionary() const;
  virtual IPrinter& printer() const;
  virtual int compressionLevel() const;
  virtual int verboseLevel() const;
  virtual void setVerboseLevel(int);
  virtual IDirectory& directory() const;
  virtual void* cast(const IClass&) const;
  virtual int write();
  virtual void close();
  virtual bool isOpen() const;
  virtual bool isWritable() const;
  virtual void setCompressionLevel(int = 1);
public:
  enum Mode {
    CREATE,
    RECREATE,
    UPDATE,
    READ
  };
  File(const std::string&,Mode,IDictionary&,IPrinter&);
  virtual ~File();
  // Get/set :
  Seek end() const;
  void setEnd(Seek);
  std::list<FreeSegment*>& freeSegments();
  // Else :
  enum From {
    BEGIN,
    CURRENT,
    END
  };
  int bestBuffer() const;
  int getRecordHeader(char*,Seek,int,int&,int&,int&);
  virtual bool setPosition(Seek = 0,From = BEGIN);
  virtual bool writeBuffer(const char*,int);
  virtual bool readBuffer(char*,int);
  virtual FreeSegment* bestFreeSegment(int);
  virtual void makeFreeSegment(Seek,Seek);
  virtual bool synchronize();
  virtual void sumBuffer(int);
  virtual bool writeHeader();
  virtual void writeFreeSegments();
private: //Files cannot be copied.
  File(const File&);
  void operator=(const File &);
  bool initializeInCreateMode(); 
  bool initializeInUpdateMode(); 
  int version();
  bool readHeader();
  int errorNumber();
  void resetErrorNumber();
  void readFreeSegments();
  void recover();
  void readStreamerInfo();
  void writeStreamerInfo();
private:
  std::string fName;
  std::string fTitle;
  Mode fMode;
  IDictionary& fDictionary;
  IPrinter& fPrinter;
  Directory* fRootDirectory;
  int fVerboseLevel;
  //
  bool fWritable;
  int fFileDescriptor;
  double fBytesWrite;       //Number of bytes written to this file
  double fBytesRead;        //Number of bytes read from this file
  int fWritten;             //Number of objects written so far
  double fSumBuffer; //Sum of buffer sizes of objects written so far
  double fSum2Buffer;//Sum of squares of buffer sizes of objects written so far
  std::list<FreeSegment*> fFreeSegments; //Free segments linked list table
  // Record (stored in file) :
  // "root"
  int fVersion;          //File format version
  Seek fBEGIN;           //First used byte in file
  Seek fEND;             //Last used byte in file
  Seek fSeekFree;        //Location on disk of free segments structure
  int fNbytesFree;       //Number of bytes for free segments structure
  //int nfree
  int fNbytesName;       //Number of bytes in TNamed at creation time
  char fUnits;           //Number of bytes for file pointers
  int fCompress;         //(=1 file is compressed, 0 otherwise)
  Seek fSeekInfo;        //Location on disk of StreamerInfo record
  int fNbytesInfo;       //Number of bytes for StreamerInfo record
};

}

#endif
