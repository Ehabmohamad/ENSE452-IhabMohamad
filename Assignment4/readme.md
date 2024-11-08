# Sine Calculator Using Lookup Table (LUT)

##### This project implements a high-performance sine calculator using a precomputed lookup table (LUT) to speed up sine calculations for embedded systems with limited processing power. The LUT stores scaled sine values at a 0.5-degree resolution from -359.5 to +359.5 degrees.


## Target
### Intel(R) Core(TM) i5-L16G7 CPU @ 1.40GHz   1.38 GHz

## Benchmark Information
###    Date: 2018-10-03
###    Compiler: g++ 5.4.0
###    Compile Switches: `None` 

## Benchmark Results

| function        | speed     |
|-----------------|-----------|
| Lookup Table    | 10.15     |
| Standard `sin()`| 92.46     |
| Table Speedup   | 9.11x     |
| Accuracy (ppm)  | 5.45      |

## Usage 
### 1. Compile the code using:

```sh
gcc calculator.c -o calculator -lm
```

### 2. Run the program using:

```sh
./calculator
```

### 3. Enter an angle within the range [-359.5, 359.5] to calculate its sine value using the lookup table.

## Benchmark Information
###    Date: 2018-10-03
###    Compiler: g++ 5.4.0
###    Compile Switches: `-O3`

## Benchmark Results

| function        | speed     |
|-----------------|-----------|
| Lookup Table    | 0.00      |
| Standard `sin()`| 0.00      |
| Table Speedup   | nanx      |
| Accuracy (ppm)  | 5.45      |

## Usage 
### 1. Compile the code using:

```sh
gcc -O3 calculator.c -o calculator -lm
```

### 2. Run the program using:

```sh
./calculator
```

### 3. Enter an angle within the range [-359.5, 359.5] to calculate its sine value using the lookup table.

## Accuracy
##### Accuracy was calculated in parts per million (ppm) by comparing the LUT results against the built-in sin() function. The LUT provides an efficient approximation suitable for applications where minor precision loss is acceptable.
