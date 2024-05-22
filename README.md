# OpenMP Linear Equation Solver

## Overview

This project involves creating an OpenMP program to solve a system of linear equations using iterative methods. The program calculates the values of unknown variables within a specified error margin.

## Prerequisites

  - OpenMP library
  - C compiler (e.g., gcc)

## Files

  - `equations.c`: The source code file.

## How to Compile

To compile the program, use the following command:

`gcc -std=c99 -fopenmp -Wall -o solve equations.c -lm`

## How to Run

To run the program, use the following command with the input file and the desired number of threads:

`./solve inputfile.txt <num_threads>`

Example:

`./solve inputfile.txt 4`

This command will solve the system of equations described in `inputfile.txt` using 4 threads.

## Input File Format

The input file should be formatted as follows:

  - **Line 1:** Number of variables (n)
  - **Line 2:** Absolute relative error (<= 1)
  - **Line 3:** Initial values for each unknown variable
  - **Line 4 onwards:** Coefficients for each equation followed by the constant term

Example:


3

0.01

2 3 4

5 1 3 6

3 7 2 8

3 6 9 6


This represents the following system of equations:

5X_1 + X_2 + 3X_3 = 6

3X_1 + 7X_2 + 2X_3 = 8

3X_1 + 6X_2 + 9X_3 = 6

## Output

The program outputs the following:
  - **Number of iterations:** Printed on the screen.
  - **Solution file:** A file named `num.sol` (where num is the number of unknowns) containing the values of the unknowns.

Example output file `3.sol`:

2
3
4

## Algorithm

The program follows these steps:

  1. Rewrite each equation so that the left-hand side is one of the unknowns.
  2. Use initial values provided in the input file.
  3. Iteratively calculate new values for the unknowns until the absolute relative error for each unknown is within the specified threshold.
  4. Update the values of the unknowns for each iteration using the old values within the same iteration.
  5. Stop when all errors are less than or equal to the given threshold.

## Performance and Scalability

To evaluate the performance and scalability of the code, I measure the time for the parallel part using `omp_get_wtime`.
