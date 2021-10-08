#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

void print_usage()
{
    printf("Usage: pi-cpu -[vtmh] \n-v verbose mode\n-V show version info\n-t show current CPU temp\n-m show current cpu MHz\n-h show help\n");
}

int main(int argc, char *argv[])
{
    FILE *fp;

    int temp = 0;
    int cpuMhz =0;
    int mincpu=0;
    int maxcpu=0;
    double fahrenheit=0;
    char model[150];
    int err = 0;
    extern char *optarg;
    extern int optind, opterr, optopt;

    int option = 0;
    int showall = -1, showtemp = -1, showcurrentMHz = -1, showhelp =-1, showVersion =-1;
    int showfahrenheit = -1;

    while ((option = getopt(argc, argv,"fvtmhV?:")) != -1)
    {
        switch (option)
        {
        case 'v' :
            showall = 0;
            break;
        case 'f' :
            showfahrenheit = 0;
            showtemp = 0;
            break;
        case 'V' :
            showVersion = 0;
            break;
        case 't' :
            showtemp = 0;
            break;
        case 'm' :
            showcurrentMHz = 0;
            break;
        case 'h' :
            showhelp = 0;
            print_usage();
            break;
        case '?':
            printf("Unknown option: %c\n", optopt);
            break;
        case ':':
            printf("Unknown option: %c\n", optopt);
            break;
        default:
            print_usage();
            exit(EXIT_FAILURE);
        }
    }



    for( int n = 1; n < argc; n++ )            /* Scan through args. */
    {
        if ((int)argv[n][0]!='-' )
        {
            printf("Unknown option: %c\n",argv[n][0]);
            printf("did you forget the switch? '-'\n\n");
            print_usage();
            err=1;
        }
    }
    if (err==0)
    {
        if (argc==1)
        {
            showtemp = 0;
            showcurrentMHz = 0;

        }


        fp = fopen("/proc/device-tree/model", "r");
        if (fp)
        {
            fgets ( model, 150, fp );
            fclose(fp);
        }
        fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_min_freq", "r");
        if (fp)
        {
            fscanf(fp, "%d", &mincpu);
            fclose(fp);
        }
        fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq", "r");
        if (fp)
        {
            fscanf(fp, "%d", &maxcpu);
            fclose(fp);
        }
        if (showtemp==0||showall==0)
        {
            fp = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
            if (fp)
            {
                fscanf(fp, "%d", &temp); //in Celcius
                fclose(fp);
                fahrenheit=(((temp/1000)*1.8)+32); //convert to Fahrenheit
            }
        }
        if (showcurrentMHz==0||showall==0)
        {
            fp = fopen("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq","r");
            if (fp)
            {
                fscanf(fp,"%d", &cpuMhz);
                fclose(fp);
            }
        }

        if (showhelp!=0)
        {
            if (showVersion==0)
            {
                printf("Version: pi-cpu 1.1\n");
                printf("Released: 03 October 2021\nCopyright © 2020-2021 C. Brown\n\n");

            }
            if (showall == 0)
            {
                printf("Model: %s\n", model);
                printf("CPU Min MHz: %.2f MHz\n", mincpu / 1000.0);
                printf("CPU Max MHz: %.2f MHz\n", maxcpu / 1000.0);
            }
            if(showcurrentMHz==0||showall==0)
            {
                printf("Current CPU MHz: %.2f MHz\n", cpuMhz / 1000.0);
            }
            if(showtemp==0||showall==0)
            {
                if (showfahrenheit==0)
                {
                    printf("Current CPU Temp: %.2f°F\n",fahrenheit);
                }
                else
                {
                    printf("Current CPU Temp: %.2f°C\n", temp / 1000.0);
                }



            }
        }
    }

    return 0;
}
