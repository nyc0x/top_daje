#!/bin/bash

echo "Hi, this is a simple bash script which installs a library and creates a folder."
echo "Will you let me do it? Type 'y' if 'Yes', anything else if 'No'"
read answer
echo 
if [ "$answer" = "y" ]; then
    sudo apt-get install libncurses5-dev libncursesw5-dev
else
    exit
fi
echo
if [ ! -d "bin/" ]
then 
    echo "Directory bin does not exist, so let's create it."
    mkdir bin
fi
echo "\nEverything's alright now, have fun :)"