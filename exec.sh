cc -O2 -g -Wall -Iinclude -c src/*.c
cc -O2 -g -Wall *.o -o aefetch -lm
rm -f *.o
./aefetch
mv -f aefetch src/aefetch

