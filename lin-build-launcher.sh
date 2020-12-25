#!/bin/bash
echo "----------------------------------"
echo "BGArmor Executable Builder - Linux"
echo "----------------------------------"
echo

echo "Building launcher executable at root directory..."

gcc -no-pie -c ./source/Launcher.c -o Launcher.o
gcc -no-pie -o Launcher Launcher.o
rm -f Launcher.o

echo "Done!"
read -p "Press any key to continue..."
