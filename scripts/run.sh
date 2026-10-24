#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/hoppings
else
    ./build/debug/hoppings
fi