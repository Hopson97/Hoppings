#!/bin/bash

if [ "$1" = "release" ]
then
    ./build/release/bin/hoppings 
else
    ./build/debug/bin/hoppings 
fi