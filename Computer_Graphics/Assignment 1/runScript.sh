./Bin/Linux/Assignment1 --in Test\ Images/input/yoda.bmp --out tests/noise1.bmp --noisify 0
./Bin/Linux/Assignment1 --in Test\ Images/input/yoda.bmp --out tests/noise2.bmp --noisify .5
./Bin/Linux/Assignment1 --in Test\ Images/input/yoda.bmp --out tests/noise3.bmp --noisify 1
./Bin/Linux/Assignment1 --in Test\ Images/input/yoda.bmp --out tests/bright1.bmp --brighten .5
./Bin/Linux/Assignment1 --in Test\ Images/input/yoda.bmp --out tests/bright2.bmp --brighten 2
./Bin/Linux/Assignment1 --contrast 0.5 --in shrek.bmp --out tests/co1.bmp
./Bin/Linux/Assignment1 --contrast 1 --in shrek.bmp --out tests/co2.bmp
./Bin/Linux/Assignment1 --contrast 2 --in shrek.bmp --out tests/co3.bmp
./Bin/Linux/Assignment1 --saturate 0.5 --in shrek.bmp --out tests/sa1.bmp
./Bin/Linux/Assignment1 --saturate 1 --in shrek.bmp --out tests/sa2.bmp
./Bin/Linux/Assignment1 --saturate 2 --in shrek.bmp --out tests/sa3.bmp
./Bin/Linux/Assignment1 --gray --in yoda.bmp --out tests/gray.bmp
./Bin/Linux/Assignment1 --quantize 1 --in ramp.bmp --out tests/qu1.bmp
./Bin/Linux/Assignment1 --quantize 2 --in ramp.bmp --out tests/qu2.bmp
./Bin/Linux/Assignment1 --quantize 3 --in ramp.bmp --out tests/qu3.bmp
./Bin/Linux/Assignment1 --blur3x3 --in sully.bmp --out tests/blur.bmp
