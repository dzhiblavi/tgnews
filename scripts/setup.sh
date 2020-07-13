#!/bin/bash

unzip submission.zip

for file in $(cat deb_packages.txt); do
    sudo apt install $file
done

sudo ./pymodules.sh
