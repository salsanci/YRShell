
#include <YRShell.h>


YRShell yrShell;

void setup()
{
  BSerial.begin( 115200);
  yrShell.init();
  BSerial.init( yrShell.getInq(), yrShell.getOutq());
}

void loop()
{
  Sliceable::sliceAll( );
}
