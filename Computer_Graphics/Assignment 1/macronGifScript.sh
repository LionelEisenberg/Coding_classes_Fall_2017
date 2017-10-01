#!/bin/bash
COUNTER=1
./Bin/Linux/Assignment1 --in leisenb5_HTML/TEST_IMAGES/input/Macron.jpg --out leisenb5_HTML/TEST_IMAGES/gifOutput/macron$COUNTER.jpg --fun
while [  $COUNTER -lt 10 ]; do
  let COUNTER2=COUNTER+1;
  ./Bin/Linux/Assignment1 --in leisenb5_HTML/TEST_IMAGES/gifOutput/macron$COUNTER.jpg --out leisenb5_HTML/TEST_IMAGES/gifOutput/macron$COUNTER2.jpg --fun;
  let COUNTER=COUNTER+1;
done