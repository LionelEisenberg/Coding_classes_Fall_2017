 #!/bin/bash
COUNTER=0;
while [  $COUNTER -lt 360 ]; do
  ./Bin/Linux/Assignment1 --in leisenb5_HTML/TEST_IMAGES/input/stripe.2.bmp --out leisenb5_HTML/TEST_IMAGES/gifOutput/rotate$COUNTER.bmp --rotateNearest  $COUNTER
  let COUNTER=COUNTER+5
done