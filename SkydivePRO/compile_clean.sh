#!/bin/bash

# Compileaza codul sursa specifcând explicit standardul C++11
g++ -std=c++11 -Wall -Wextra -g -I./include ./src/*.cpp -o SkydivePRO

echo "Compilarea a fost finalizată. Rulează cu ./SkydivePRO"
