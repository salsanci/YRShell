
#include <YRShell.h>


class MyShell : public YRSmallShell {
protected:
	virtual const char* shellClass( void) { return "MyShell"; }
    virtual const char* mainFileName( ) { return "examples/YRSmallShell/YRSnmallShell.ino"; }
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
