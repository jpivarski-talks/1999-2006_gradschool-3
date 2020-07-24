#ifndef Rio_IFile_h
#define Rio_IFile_h

#include <string>

namespace Rio {

class IDictionary;
class IPrinter;
class IDirectory;
class IClass;

/**
 * @class IFile
 *
 *  IFile is the "user" interface to a Rio file.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IFile {
public:
  virtual ~IFile(){}

  /** @return The name of the file. */
  virtual const std::string& name() const = 0;

  /** @return The dictionary used to read/write objects. */
  virtual IDictionary& dictionary() const = 0;

  /** @return The printer used to print messages. */
  virtual IPrinter& printer() const = 0;

  /** Set the verbose level : 
   *  - 0 = only error messages are dump.
   *  - 1 = error and information messages are dump. */
  virtual void setVerboseLevel(int) = 0;

  /** @return The verbose level. */
  virtual int verboseLevel() const = 0;

  /** Set the compression level.
   * @param aLevel : compression level. */
  virtual void setCompressionLevel(int aLevel = 1) = 0;

  /** @return The compression level. */
  virtual int compressionLevel() const = 0;

  /** @return The root directory of the file. */
  virtual IDirectory& directory() const = 0;

  /** Write data on file. */
  virtual int write() = 0;

  /** Close the file. */
  virtual void close() = 0;

  /** @return True if file is open, false if not. */
  virtual bool isOpen() const = 0;

  /** @return True if file is writable, false if not. */
  virtual bool isWritable() const = 0;

  /** See IObject::cast. */
  virtual void* cast(const IClass& aClass) const = 0;
};

}

#endif
