#!/bin/bash

# Commands in the same process group
echo "Example with commands in the same process group:"
sleep 3 | echo "a"

# Commands in different process groups
echo "Example with commands in different process groups:"
(sleep 3; echo "a") &
