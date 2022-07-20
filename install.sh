#!/bin/bash

echo "Hi, I'm just a simple bash script which set up some things."
echo "Will you let me do it? [y/n]"
read answer
echo 
if [ "$answer" = "y" ]; then
    sudo apt-get install libncurses5-dev libncursesw5-dev
fi
echo
if [ ! -d "./.bin/" ]; then 
    echo "Directory .bin does not exist, so I create it."
    mkdir .bin
fi

echo "Everything's alright now, have fun :)"