CC=avr-gcc
CFLAGS=-c -Os -DF_CPU=16000000UL -mmcu=atmega328p
INPUT=./src/utility.c
OUTPUT=./output/utility.o

AR=avr-ar
AFLAGS=-rc
LIB_OUTPUT=./output/libutility.a

all: build lib

build:
	$(CC) $(INPUT) $(CFLAGS) -o $(OUTPUT)

lib:
	$(AR) $(AFLAGS) $(LIB_OUTPUT) $(OUTPUT)
