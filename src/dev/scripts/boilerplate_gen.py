#!/usr/bin/python3
import sys

def gen(name):
    print(f"Generating boilerplate for {name}...")

if __name__ == "__main__":
    if len(sys.argv) > 1:
        gen(sys.argv[1])
