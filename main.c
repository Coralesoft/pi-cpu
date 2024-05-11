/*
# pi-cpu - CPU Information Utility for Raspberry Pi
# This program:
#    - Reads the model and frequency information of the CPU.
#    - Displays the current CPU temperature.
#    - Provides output in Celsius or Fahrenheit as per user selection.
#    - Supports verbose mode for detailed output.
#
# Usage:
#    - Run with option flags to display specific information or use help (-h) for usage instructions.
#
# To modify or enhance functionality, adapt the source and recompile.
#
# Last revised: 11/05/2024
# version 2024.5.1
#-----------------------------------------------------------------------
# Version         Date          Notes:
# 2021.10.1       10/10/2021    Initial Release - basic functionality.
# 2024.5.1        11/05/2024    Added error handling and Fahrenheit option and refactored code.
#
# Copyright (C) 2012 -2024 by C. Brown
# Email: dev@coralesoft.nz
# Released under the MIT License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>

// Define file paths as constants for CPU model and frequency data
#define CPU_MODEL_FILE "/proc/device-tree/model"
#define SCALING_MIN_FREQ_FILE "/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq"
#define SCALING_MAX_FREQ_FILE "/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq"
#define SCALING_CUR_FREQ_FILE "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq"
#define CPU_TEMP_FILE "/sys/class/thermal/thermal_zone0/temp"

// Function to print usage information
void print_usage() {
    printf("Usage: pi-cpu -[vtmhV] \n"
           "-v verbose mode\n"
           "-V show version info\n"
           "-f show in fahrenheit\n"
           "-t show current CPU temp\n"
           "-m show current CPU MHz\n"
           "-h show help\n");
}

// Function to print version information
void print_version() {
    printf("Version: pi-cpu 1.2\n"
           "Released: 11 May 2024\n"
           "Copyright © 2020-2024 C. Brown\n\n");
}

// Utility function to read an integer value from a given file path
int read_file_as_int(const char *filepath) {
    FILE *fp;
    int value = 0;

    // Attempt to open the file at the specified path
    fp = fopen(filepath, "r");
    if (!fp) {
        // If unable to open the file, print an error message
        fprintf(stderr, "Error: Unable to open file %s. Check if the file exists and has read permissions.\n", filepath);
        return -1;
    }

    // Read an integer value from the file
    if (fscanf(fp, "%d", &value) != 1) {
        // If unable to read the integer value, print an error message
        fprintf(stderr, "Error: Unable to read an integer value from file %s.\n", filepath);
        fclose(fp);
        return -1;
    }

    // Close the file after reading
    fclose(fp);
    return value;
}

// Function to read CPU information (model, min/max frequency)
void read_cpu_info(char *model, int *mincpu, int *maxcpu) {
    FILE *fp;

    // Read CPU model information from the designated file
    fp = fopen(CPU_MODEL_FILE, "r");
    if (fp) {
        fgets(model, 150, fp);
        fclose(fp);
    } else {
        snprintf(model, 150, "Unknown model");
        fprintf(stderr, "Warning: Unable to read CPU model from %s.\n", CPU_MODEL_FILE);
    }

    // Read minimum CPU frequency using the utility function
    *mincpu = read_file_as_int(SCALING_MIN_FREQ_FILE);
    if (*mincpu == -1) {
        *mincpu = 0;
    }

    // Read maximum CPU frequency using the utility function
    *maxcpu = read_file_as_int(SCALING_MAX_FREQ_FILE);
    if (*maxcpu == -1) {
        *maxcpu = 0;
    }
}

// Function to read the current CPU frequency
int read_current_cpu_freq() {
    return read_file_as_int(SCALING_CUR_FREQ_FILE);
}

// Function to read the current CPU temperature
int read_cpu_temp() {
    return read_file_as_int(CPU_TEMP_FILE);
}

// Main function that parses input options and displays CPU information
int main(int argc, char *argv[]) {
    // Declare variables to hold temperature, CPU frequencies, and option flags
    int temp = 0, cpuMhz = 0, mincpu = 0, maxcpu = 0;
    double fahrenheit = 0.0;
    char model[150];
    int option = 0, showall = 0, showtemp = 0, showcurrentMHz = 0, showhelp = 0, showVersion = 0, showfahrenheit = 0;

    // Parse command-line options
    while ((option = getopt(argc, argv, "vfVtmh")) != -1) {
        switch (option) {
            case 'v':  // Verbose mode
                showall = 1;
                break;
            case 'f':  // Display temperature in Fahrenheit
                showfahrenheit = 1;
                showtemp = 1;
                break;
            case 'V':  // Show version information
                showVersion = 1;
                break;
            case 't':  // Show current CPU temperature
                showtemp = 1;
                break;
            case 'm':  // Show current CPU MHz
                showcurrentMHz = 1;
                break;
            case 'h':  // Display usage help
                showhelp = 1;
                print_usage();
                return 0;
            case '?':  // Handle unknown options
            default:
                print_usage();
                return EXIT_FAILURE;
        }
    }

    // Display usage help if no valid options were given
    if (argc == 1 || (showhelp == 0 && showVersion == 0 && showtemp == 0 && showcurrentMHz == 0 && showall == 0)) {
        print_usage();
        return EXIT_FAILURE;
    }

    // Display version information if requested
    if (showVersion) {
        print_version();
    }

    // Read and display CPU information if any relevant options were provided
    if (showall || showtemp || showcurrentMHz) {
        read_cpu_info(model, &mincpu, &maxcpu);

        // Print model and min/max frequency if verbose mode is enabled
        if (showall) {
            printf("Model: %s\n", model);
            printf("CPU Min MHz: %.2f MHz\n", mincpu / 1000.0);
            printf("CPU Max MHz: %.2f MHz\n", maxcpu / 1000.0);
        }

        // Print current CPU frequency if requested or in verbose mode
        if (showcurrentMHz || showall) {
            cpuMhz = read_current_cpu_freq();
            if (cpuMhz != -1) {
                printf("Current CPU MHz: %.2f MHz\n", cpuMhz / 1000.0);
            } else {
                printf("Current CPU MHz: Unknown\n");
            }
        }

        // Print current CPU temperature if requested or in verbose mode
        if (showtemp || showall) {
            temp = read_cpu_temp();
            if (temp != -1) {
                fahrenheit = ((temp / 1000.0) * 1.8) + 32.0;  // Convert Celsius to Fahrenheit
                if (showfahrenheit) {
                    printf("Current CPU Temp: %.2f°F\n", fahrenheit);
                } else {
                    printf("Current CPU Temp: %.2f°C\n", temp / 1000.0);
                }
            } else {
                printf("Current CPU Temp: Unknown\n");
            }
        }
    }

    return 0;
}
