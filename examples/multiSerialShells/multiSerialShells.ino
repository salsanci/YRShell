
#include <YRShell.h>

YRShell shell1;
YRShell shell2;
YRShell shell3;


IntervalTimer it;

void setup()
{
  BSerial.begin(115200);
  BSerial2.begin(230400);
  BSerial3.begin(230400);
  pinMode(LED_BUILTIN, OUTPUT);
  it.setInterval( 0);
  BSerial.init( shell1.getInq(), shell1.getOutq());
  BSerial2.init( shell2.getInq(), shell2.getOutq());
  BSerial3.init( shell3.getInq(), shell3.getOutq());
}

void loop()
{
  static bool state = false;

  if( it.hasIntervalElapsed( ) ) {
    if( state) {
      digitalWrite(LED_BUILTIN, HIGH);
      it.setInterval( 500);
    } else {
      digitalWrite(LED_BUILTIN, LOW);
      it.setInterval( 1500);
    }
    state = !state;
  }
  Sliceable::sliceAll();
}
