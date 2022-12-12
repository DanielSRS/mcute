run: all
		sudo ./Rasp -analogic -d."D0".16 -d."D1".5

test: all
		./Rasp -analogic -d."D0".16 -d."D1".5

list: all
		sudo ./Rasp -l

all: Rasp

Rasp: Rasp.o display.o serial.o comunication.o sensor.o utils.o menu.o terminal.o help.o mqtt.o
		gcc main.o display.o serial.o comunication.o sensor.o lcd.o utils.o menu.o terminal.o help.o mqtt.o -o Rasp -l paho-mqtt3c

Rasp.o: main.c
		gcc -c main.c -o main.o

lcd: lcd.s
		gcc -c lcd.s -o lcd.o

mqtt.o: mqtt.c
		gcc -c mqtt.c -o mqtt.o

mock: display.mock.c
		gcc -c display.mock.c -o lcd.o

serial.o: serial.c
		gcc -c serial.c -o serial.o

comunication.o: comunication.c
		gcc -c comunication.c -o comunication.o

sensor.o: sensor.c
		gcc -c sensor.c -o sensor.o

display.o: display.c
		gcc -c display.c -o display.o

utils.o: utils.c
		gcc -c utils.c -o utils.o

menu.o: menu.c
		gcc -c menu.c -o menu.o

terminal.o: terminal.c
		gcc -c terminal.c -o terminal.o

help.o: help.c
		gcc -c help.c -o help.o

clean:
		rm -rf *.o *~ Rasp
