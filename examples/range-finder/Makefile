CC=avr-gcc

CFLAGS=-Os -DF_CPU=16000000UL -mmcu=atmega328p -I ../../src -L ../../output -l utility -g
INPUT=./src/*
OUTPUT=./bin/main
PORT=/dev/ttyACM1

FLASHER=avrdude
AVRDUDE_CONF=/usr/share/arduino/hardware/archlinux-arduino/avr/bootloaders/gemma/avrdude.conf
FLASH_ARGS=-F -v -V -c arduino -p atmega328p -b 115200 -P $(PORT) -C $(AVRDUDE_CONF)

all: build flash

build:
	$(CC) $(INPUT) $(CFLAGS) -o $(OUTPUT)

flash:
	$(FLASHER) $(FLASH_ARGS) -U flash:w:$(OUTPUT)
