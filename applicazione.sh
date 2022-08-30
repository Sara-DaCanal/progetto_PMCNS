#! /bin/bash

cd Simulatore_ProntoSoccorso;
rm -d -r statistiche;
make progetto; 
cd ../Python;
python3 grafici.py
