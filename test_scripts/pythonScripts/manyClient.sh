#!/bin/bash

for (( i = 0; i < $1; i++ ));
do
    python chunk.py >/dev/null &
done