#!/bin/sh

#Este es un script simple para subir el archivo binario al micro

st-flash --reset write build/firmware.bin 0x8000000
