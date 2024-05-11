#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

void print_usage() {
    printf("Usage: pi-cpu -[vtmhV] \n"
           "-v verbose mode\n"
           "-V show version info\n"
           "-f show in fahrenheit\n"
           "-t show current CPU temp\n"
           "-m show current CPU MHz\n"
           "-h show help\n");
}

void print_version() {
    printf("Version: pi-cpu 1.2\n"
           "Released: 11 May 2024\n"
           "Copyright © 2020-2024 C. Brown\n\n");
}

void read_cpu_info(char *model, int *mincpu, int *maxcpu) {
    FILE *fp;

    // Read CPU model
    fp = fopen("/proc/device-tree/model", "r");
    if (fp) {
        fgets(model, 150, fp);
        fclose(fp);
    } else {
        snprintf(model, 150, "Unknown model");
    }

    // Read minimum CPU frequency
    fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq", "r");
    if (fp) {
        fscanf(fp, "%d", mincpu);
        fclose(fp);
    } else {
        *mincpu = 0;
    }

    // Read maximum CPU frequency
    fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "r");
    if (fp) {
        fscanf(fp, "%d", maxcpu);
        fclose(fp);
    } else {
        *maxcpu = 0;
    }
}

int read_current_cpu_freq() {
    FILE *fp;
    int freq = 0;

    fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq", "r");
    if (fp) {
        fscanf(fp, "%d", &freq);
        fclose(fp);
    }

    return freq;
}

int read_cpu_temp() {
    FILE *fp;
    int temp = 0;

    fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
    if (fp) {
        fscanf(fp, "%d", &temp); // In millidegrees Celsius
        fclose(fp);
    }

    return temp;
}

int main(int argc, char *argv[]) {
    int temp = 0, cpuMhz = 0, mincpu = 0, maxcpu = 0;
    double fahrenheit = 0.0;
    char model[150];
    int option = 0, showall = 0, showtemp = 0, showcurrentMHz = 0, showhelp = 0, showVersion = 0, showfahrenheit = 0;

    while ((option = getopt(argc, argv, "vfVtmh")) != -1) {
        switch (option) {
            case 'v':
                showall = 1;
                break;
            case 'f':
                showfahrenheit = 1;
                showtemp = 1;
                break;
            case 'V':
                showVersion = 1;
                break;
            case 't':
                showtemp = 1;
                break;
            case 'm':
                showcurrentMHz = 1;
                break;
            case 'h':
                showhelp = 1;
                print_usage();
                return 0;
            case '?':
            default:
                print_usage();
                return EXIT_FAILURE;
        }
    }

    // Display help if no valid options were given
    if (argc == 1 || (showhelp == 0 && showVersion == 0 && showtemp == 0 && showcurrentMHz == 0 && showall == 0)) {
        print_usage();
        return EXIT_FAILURE;
    }

    // Version info
    if (showVersion) {
        print_version();
    }

    // CPU information
    if (showall || showtemp || showcurrentMHz) {
        read_cpu_info(model, &mincpu, &maxcpu);

        if (showall) {
            printf("Model: %s\n", model);
            printf("CPU Min MHz: %.2f MHz\n", mincpu / 1000.0);
            printf("CPU Max MHz: %.2f MHz\n", maxcpu / 1000.0);
        }

        if (showcurrentMHz || showall) {
            cpuMhz = read_current_cpu_freq();
            printf("Current CPU MHz: %.2f MHz\n", cpuMhz / 1000.0);
        }

        if (showtemp || showall) {
            temp = read_cpu_temp();
            fahrenheit = ((temp / 1000.0) * 1.8) + 32.0; // Celsius to Fahrenheit conversion

            if (showfahrenheit) {
                printf("Current CPU Temp: %.2f°F\n", fahrenheit);
            } else {
                printf("Current CPU Temp: %.2f°C\n", temp / 1000.0);
            }
        }
    }

    return 0;
}
