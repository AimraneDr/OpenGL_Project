#!/bin/bash

# Engine
make -f "engine.linux.mk" clean
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit 1
fi

# Game
make -f "game.linux.mk" clean
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit 1
fi

echo "All assemblies deleted successfully."