#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <omp.h>

/**
 * Main function to solve a system of linear equations using OpenMP.
 * 
 * @param argc Number of command line arguments
 * @param argv Command line arguments
 * @return Exit status of the program
 */
int main(int argc, char* argv[]) {

    char outputFile[256];
    int numVars, numThreads, iterations = 0;
    double error, errorThresh;

    // Check for correct usage
    if (argc != 3) {
        printf("Usage: ./solve inputfile.txt num_threads\n");
        return 1;
    }

    char *filename = argv[1];
    numThreads = atoi(argv[2]);
    double e_time, s_time;

    // Set the number of threads for OpenMP
    omp_set_num_threads(numThreads);

    // Open the input file
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open input file.\n");
        return 1;
    }

    // Read the number of variables
    fscanf(file, "%d", &numVars);

    // Read the error threshold
    fscanf(file, "%lf", &errorThresh);

    // Allocate memory for the old and new values of the variables
    double *oldValues = malloc(numVars * sizeof(double));
    double *newValues = malloc(numVars * sizeof(double));

    // Allocate memory for the constants of the equations
    double *constants = malloc(numVars * sizeof(double));

    // Allocate memory for the coefficients matrix
    double **equation = malloc(numVars * sizeof(double*));
    for (int i = 0; i < numVars; i++) {
        equation[i] = malloc(numVars * sizeof(double));
    }

    // Read the initial values for the variables
    for (int i = 0; i < numVars; i++) {
        fscanf(file, "%lf", &oldValues[i]);
    }

    // Read the coefficients matrix and constants vector
    for (int i = 0; i < numVars; i++) {
        for (int j = 0; j < numVars; j++) {
            fscanf(file, "%lf", &equation[i][j]);
        }
        fscanf(file, "%lf", &constants[i]);
    }

    fclose(file);

    // Flag to determine if the error threshold is met
    bool stop = false;

    // Variable for calculating the numerator
    double numerator;

    // Start the timing for performance measurement
    s_time = omp_get_wtime();

    // Iterative method to solve the linear equations
    while (!stop) {
        stop = true;

        // Parallelized loop to update the values of the variables
        #pragma omp parallel for num_threads(numThreads) private(numerator, error)
        for (int i = 0; i < numVars; i++) {
            numerator = constants[i];

            for (int j = 0; j < numVars; j++) {
                if (i != j) {
                    numerator -= (equation[i][j] * oldValues[j]);
                }
            }

            newValues[i] = numerator / equation[i][i];

            if (stop != false) {
                error = ((newValues[i] - oldValues[i]) / newValues[i]);
                if (error < 0) error = fabs(error);

                if (error > errorThresh && stop == true) stop = false;
            }
        }

        // Update oldValues for the next iteration
        #pragma omp parallel for num_threads(numThreads)
        for (int i = 0; i < numVars; i++) {
            oldValues[i] = newValues[i];
        }

        iterations++;
    }

    // End the timing for performance measurement
    e_time = omp_get_wtime();

    // Print the total number of iterations and time taken
    printf("Total number of iterations: %d\n", iterations);
    printf("Time taken: %f\n", e_time - s_time);

    // Write the results to the output file
    sprintf(outputFile, "%d.sol", numVars);
    file = fopen(outputFile, "w");

    for (int i = 0; i < numVars; i++) {
        fprintf(file, "%.5f\n", newValues[i]);
    }

    fclose(file);

    // Free the allocated memory
    free(oldValues);
    free(newValues);
    free(constants);
    for (int i = 0; i < numVars; i++) {
        free(equation[i]);
    }
    free(equation);

    return 0;
}