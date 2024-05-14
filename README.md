
# pi-cpu

`pi-cpu` is a command-line utility for Raspberry Pi that provides real-time information about the device's board model, CPU frequency, and temperature. It offers both Celsius and Fahrenheit outputs for temperature monitoring.

## Features

- **Board Model**: Displays the model of the Raspberry Pi.
- **CPU Frequency**: Shows current, minimum, and maximum CPU frequencies.
- **Temperature Monitoring**: Monitors and displays CPU temperature in Celsius or Fahrenheit.

## Installation

### Prerequisites

Make sure you have `gcc` installed on your Raspberry Pi. Install it with:

```
sudo apt-get install gcc
```

### Building pi-cpu

Clone the repository and compile the utility using the provided Makefile:

```
git clone https://github.com/Coralesoft/pi-cpu.git
cd pi-cpu
make
```

### Installing pi-cpu

Install `pi-cpu` to your system with:

```
make install
```

This command copies the executable to `/usr/local/bin`, allowing it to be run from anywhere on the system.

## Usage

Run `pi-cpu` with the following options:

```
pi-cpu [options]
```

### Options

- `-v` : Verbose mode, shows detailed information.
- `-V` : Displays version information.
- `-f` : Outputs the temperature in Fahrenheit.
- `-t` : Displays the current CPU temperature.
- `-m` : Shows the current CPU MHz.
- `-h` : Displays help information.

### Examples

- Show all information in verbose mode:
  ```
  ./pi-cpu -v
  ```
- Display the CPU temperature in Fahrenheit:
  ```
  ./pi-cpu -f
  ```
- Show the current CPU frequency:
  ```
  ./pi-cpu -m
  ```

## Contributing

Contributions are welcome! Fork the repository, make your changes, and submit a pull request. For bugs and feature requests, please open an issue.

## License

`pi-cpu` is released under the MIT License. See the LICENSE file for more details.
