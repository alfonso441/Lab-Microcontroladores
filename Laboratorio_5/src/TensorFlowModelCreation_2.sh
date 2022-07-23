#!/bin/sh

# Crea un archivo model.h con el modelo creado de TensorFlowLite
echo "const unsigned char modelTF[] = {" >  gesture_model.h
cat gesture_model.tflite | xxd -i      >> gesture_model.h
echo "};"                              >> gesture_model.h
