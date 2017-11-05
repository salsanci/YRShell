
#include <YRShell.h>

class MyShell : public YRShell {
protected:
  virtual const char* shellClass( void) { return "MyShell"; }
    virtual const char* mainFileName( ) { return "examples/multipleSerialShells/multipleSerialShells.ino"; }
public:
  MyShell() {}
  virtual ~MyShell() {}
};

MyShell shell1;
MyShell shell2;



void setup()
{
  BSerial.begin(115200);
  BSerial1.begin(230400);
  BSerial.init( shell1.getInq(), shell1.getOutq());
  BSerial1.init( shell2.getInq(), shell2.getOutq());
}

void loop()
{
  Sliceable::sliceAll();
}
