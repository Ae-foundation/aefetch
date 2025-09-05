cc -lm -O2 -g -Wall -c src/*.c
cc -lm -O2 -g -Wall src/*.o -o ./aefetch
rm *.o
cp -rf aefetch src/aefetch
./aefetch
rm aefetch

