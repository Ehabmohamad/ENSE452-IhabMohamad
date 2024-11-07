#include <stdio.h>
#include <math.h>
#include <time.h>

#define SCALE_FACTOR 100000  // the scale factor for the sin values by 100,000
#define TABLE_SIZE 1440      // the resoltution of 0.5-degree resolution from -359.5 to +359.5
#define DEGREE_TO_INDEX(angle) ((int)((angle + 359.5) * 2))  // Translate angle to index

// the lookup table to store scaled sin values
int sin_lut[TABLE_SIZE];

// Function to populate the LUT
void create_sin_lut() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        double angle = (i / 2.0) - 359.5;  // Converting the index back to angle
        sin_lut[i] = (int)(sin(angle * M_PI / 180.0) * SCALE_FACTOR);  // Scaling and storing 
    }
}

// Function that will get sin value using LUT
int sin_lut_lookup(double angle) {
    int index = DEGREE_TO_INDEX(angle);
    if (index < 0 || index >= TABLE_SIZE) {
        // Handle out-of-bounds access (bounds-check)
        return 0;
    }
    return sin_lut[index];
}
int get_sin_from_table(double angle) {
    // Bounds check of the angle
    if (angle < -359.5 || angle > 359.5) {
        printf("Angle out of bounds.\n");
        return 0;
    }
    // Converting the angle back to an index
    int index = (int)((angle - -359.5) / 0.5);
    return sin_lut[index];
}

// Function to measure speed and accuracy
void measure_performance() {
    clock_t start, end;
    int test_angle;
    double sin_builtin;
    int sin_lut_value;
    double accuracy_error_sum = 0;
    
    // Measure LUT speed
    start = clock();
    for (int i = 0; i < 100000000; i++) {
        test_angle = (i % TABLE_SIZE) / 2.0 - 359.5;  // Wrap-around angles
        sin_lut_lookup(test_angle);
    }
    end = clock();
    double lut_time = ((double)(end - start) / CLOCKS_PER_SEC) / 100000000;
    
    // Measure built-in sin() speed
    start = clock();
    for (int i = 0; i < 100000000; i++) {
        test_angle = (i % TABLE_SIZE) / 2.0 - 359.5;
        sin_builtin = sin(test_angle * M_PI / 180.0);
    }
    end = clock();
    double sin_time = ((double)(end - start) / CLOCKS_PER_SEC) / 100000000;
    
    // Calculate speedup factor
    double speedup = sin_time / lut_time;

    // Calculate accuracy in parts per million (ppm)
    for (int i = 0; i < TABLE_SIZE; i++) {
        test_angle = i / 2.0 - 359.5;
        sin_builtin = sin(test_angle * M_PI / 180.0);
        sin_lut_value = sin_lut_lookup(test_angle);
        accuracy_error_sum += fabs(sin_builtin - sin_lut_value / (double)SCALE_FACTOR);
    }
    double accuracy_ppm = (accuracy_error_sum / TABLE_SIZE) * 1e6;

    // Display results
    printf("Lookup Table Speed: %f ns\n", lut_time * 1e9);
    printf("Built-in sin() Speed: %f ns\n", sin_time * 1e9);
    printf("Speedup Factor: %.2fx\n", speedup);
    printf("Accuracy (ppm): %.2f\n", accuracy_ppm);
}

int main() {
    create_sin_lut();  // Create LUT
    
    // User input for sine calculation
    double user_angle;
    printf("Enter an angle between %.1f and %.1f degrees: ", -359.5, 359.5);
    scanf("%lf", &user_angle);
    int sin_value = get_sin_from_table(user_angle);
    printf("The sine of %.1f degrees (scaled by %d) is: %d\n", user_angle, SCALE_FACTOR, sin_value);

    
    measure_performance();  // Measure performance and accuracy
    return 0;
}