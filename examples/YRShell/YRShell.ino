
#include <YRShell.h>


class MyShell : public YRShell {
protected:
	virtual const char* shellClass( void) { return "MyShell"; }
    virtual const char* mainFileName( ) { return "examples/YRShell/YRShell.ino"; }
public:
	MyShell() {}
	virtual ~MyShell() {}
};

MyShell yrShell;

void setup()
{
  BSerial.begin( 115200);
  BSerial.init( yrShell.getInq(), yrShell.getOutq());
}

void loop()
{
  Sliceable::sliceAll( );
}
