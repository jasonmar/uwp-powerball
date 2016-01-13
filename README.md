## Description

This app generates random numbers for use in powerball.

## Motivation

A toy app for Windows 10 Mobile written in C++.

## How it works

The app generates random numbers until all available numbers have been generated at least once, using a bitset to track which numbers have been generated. Once the final number is generated the app prints the last 5 numbers from a ring buffer, then repeats the process with a new RNG/seed for the powerball number. The idea is to pick the "unluckiest" numbers.
