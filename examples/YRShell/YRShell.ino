
#include <YRShell.h>


YRShell yrShell;

void setup()
{
  BSerial.begin( 115200);
  BSerial.init( yrShell.getInq(), yrShell.getOutq());
}

void loop()
{
  Sliceable::sliceAll( );
}
