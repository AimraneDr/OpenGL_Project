#!/bin/bash

# Enginelear
make -f "engine.linux.mk" all
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit 1
else
    echo "Engine built successfully."
fi

# Game
make -f "game.linux.mk" all
if [ $? -ne 0 ]; then
    echo "Error: $?"
    exit 1
else
    echo "Game built successfully."
fi

echo "All assemblies built successfully."
