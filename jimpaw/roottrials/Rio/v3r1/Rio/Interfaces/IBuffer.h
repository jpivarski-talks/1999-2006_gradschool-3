#ifndef Rio_IBuffer_h
#define Rio_IBuffer_h

#include <string>
#include <vector>

namespace Rio {

class IObject;

/**
 * @class IBuffer
 *
 *  IBuffer is the interface to data buffer 
 * used by object streamer methods.
 *
 * @author Guy Barrand
 * @date 2002
 */

class IBuffer {
public:
  virtual ~IBuffer() {}

  /** Methods to read data from the buffer. */
  virtual void read(char&) = 0;
  virtual void read(bool&) = 0;
  virtual void read(short&) = 0;
  virtual void read(int&) = 0;
  virtual void read(long&) = 0;
  virtual void read(float&) = 0;
  virtual void read(double&) = 0;
  virtual void read(unsigned char&) = 0;
  virtual void read(unsigned short&) = 0;
  virtual void read(unsigned int&) = 0;
  virtual void read(unsigned long&) = 0;
  virtual void read(std::string&) = 0;
  virtual int readArray(int*&) = 0;
  virtual int readArray(double*&) = 0;
  virtual int readArray(char*&) = 0;
  virtual void readFastArray(char*,int) = 0;
  virtual void readFastArray(int*,int) = 0;
  virtual void readFastArray(float*,int) = 0;

  /** Methods to write data in the buffer. */
  virtual void write(char) = 0;
  virtual void write(bool) = 0;
  virtual void write(short) = 0;
  virtual void write(int) = 0;
  virtual void write(long) = 0;
  virtual void write(float) = 0;
  virtual void write(double) = 0;
  virtual void write(unsigned char) = 0;
  virtual void write(unsigned short) = 0;
  virtual void write(unsigned int) = 0;
  virtual void write(unsigned long) = 0;
  //Warning : for constant strings, someone must do a :
  //     write(std::string("<string>"))
  // else a :
  //     write("<string>")
  // will be casted on write(bool) !!!
  virtual void write(const std::string&) = 0;
  virtual void writeArray(const int*,int) = 0;
  virtual void writeArray(const double*,int) = 0;
  virtual void writeFastArray(const char*,int) = 0;
  virtual void writeFastArray(const float*,int) = 0;
  virtual void writeFastArray(const int*,int) = 0;

  /** @return true if buffer is in read mode, false if not.*/
  virtual bool isReading() const = 0;

  /** Below method is used in a streamer to write the 
   * version and the byte count of the streamed data. It is used in 
   * the following way :
   * @verbatim
   *   void MyObject::stream(IBuffer& aBuffer) {
   *     if (!aBuffer.isReading()) {
   *       // If the "version" of the MyObject class is "2" :
   *       unsigned int c = aBuffer.writeVersion(2,true);
   *       .... 
   *       write things with the write methods.
   *       ....
   *       aBuffer.setByteCount(c,true);
   *   }}
   * @endverbatim
   *
   * @param aVersion : the version. 
   * @param aUseByteCount : true if using byte count.
   * @return Position where to store possible byte count. */
  virtual unsigned int writeVersion(short aVersion,
				    bool aUseByteCount = false) = 0;

  /** See writeVersion comment. */
  virtual void setByteCount(unsigned int aPosition,
			    bool aUseByteCount = false) = 0;

  /** The below method is used in a streamer to read the 
   * version and the byte count of the streamed data. It is used in 
   * the following way :
   * @verbatim
   *   void MyObject::stream(IBuffer& aBuffer) {
   *     if (aBuffer.isReading()) {
   *       unsigned int s, c;
   *       short version = aBuffer.readVersion(&s, &c);
   *       .... 
   *       read things according the version with the read methods.
   *       ....
   *       // If the stored name of the class is "MyObject"
   *       // (could be retreived with a MyObject::is().name()) :
   *       aBuffer.checkByteCount(s, c,"MyObject");
   *   }}
   * @endverbatim
   */
  virtual short readVersion(unsigned int* aStartPosition = 0, 
			    unsigned int* aByteCount = 0) = 0;

  /** See readVersion comment. */
  virtual int checkByteCount(unsigned int aStartPosition,
			     unsigned int aByteCount,
			     const std::string& aClassName) = 0;

  /** To read an IObject from the buffer. Internally the class name
   * is read from the buffer. From it, the dictionary is used 
   * to retrieve the IClass describing the class. The IClass::create
   * method is used to create an object. Some arguments may be passed
   * to the constructor. In Rio we are not limited to a default constructor
   * with no arguments.
   * 
   * @param aArgs : A list of pair(code,pointer) to pass arguments to 
   *                the constructors.
   * @return A pointer to the created object. If the IClass::create method
   *         put the object in a IDirectory, the caller do not have to delete
   *         the returned object. If not, the caller has the responsibility 
   *         of managing the object.
   */
  virtual IObject* readObject(const std::vector< std::pair<int,void*> >& aArgs) = 0;

  /** Write an object. */
  virtual void writeObject(IObject*) = 0;

  /** Length of written data. */
  virtual int length() const = 0;

  /** Internal usage only. 
   * It is not advised to manipulate directly the buffer. */
  virtual char* buffer() const = 0;

  /** Internal usage only. */
  virtual void setBufferOffset(int = 0) = 0;
};

}

#endif
