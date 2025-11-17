#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./FFT/FFT.h"

#define SAMPLE_RATE 48000
#define DURATION 0.1  // 100ms test signals
#define NUM_SAMPLES (int)(SAMPLE_RATE * DURATION)

// Create test signals
float* create_sine_wave(float frequency, float amplitude, float phase) {
    float* signal = malloc(NUM_SAMPLES * sizeof(float));
    for(int i = 0; i < NUM_SAMPLES; i++) {
        double t = (double)i / SAMPLE_RATE;
        signal[i] = amplitude * sin(2.0 * M_PI * frequency * t + phase);
    }
    return signal;
}

float* create_multi_frequency_signal() {
    float* signal = malloc(NUM_SAMPLES * sizeof(float));
    for(int i = 0; i < NUM_SAMPLES; i++) {
        double t = (double)i / SAMPLE_RATE;
        // 1kHz at 0.8 amplitude, 2kHz at 0.5 amplitude
        signal[i] = 0.8 * sin(2.0 * M_PI * 1000.0 * t) +
                   0.5 * sin(2.0 * M_PI * 2000.0 * t);
    }
    return signal;
}

float* create_phase_shifted_signal_90() {
    float* signal = malloc(NUM_SAMPLES * sizeof(float));
    for(int i = 0; i < NUM_SAMPLES; i++) {
        double t = (double)i / SAMPLE_RATE;
        // 1kHz with 45 degree phase shift (π/4 radians)
        signal[i] = sin(2.0 * M_PI * 16000.0 * t + M_PI/2)+sin(2.0*M_PI*17000.0*t);
    }
    return signal;
}
float* create_phase_shifted_signal_90_2() {
    float* signal = malloc(NUM_SAMPLES * sizeof(float));
    for(int i = 0; i < NUM_SAMPLES; i++) {
        double t = (double)i / SAMPLE_RATE;
        // 1kHz with 45 degree phase shift (π/4 radians)
        signal[i] = sin(2.0 * M_PI * 16000.0 * t + M_PI/4)+sin(2.0*M_PI*17000.0*t + M_PI/2 + M_PI/4);
    }
    return signal;
}

float* create_phase_shifted_signal_0() {
    float* signal = malloc(NUM_SAMPLES * sizeof(float));
    for(int i = 0; i < NUM_SAMPLES; i++) {
        double t = (double)i / SAMPLE_RATE;
        // 1kHz with 45 degree phase shift (π/4 radians)
        signal[i] = sin(2.0 * M_PI * 16000.0 * t)+sin(2.0*M_PI*17000.0*t);
    }
    return signal;
}
float* create_phase_shifted_signal_both() {
    float* signal = malloc(NUM_SAMPLES * sizeof(float));
    for(int i = 0; i < NUM_SAMPLES; i++) {
        double t = (double)i / SAMPLE_RATE;
        // 1kHz with 45 degree phase shift (π/4 radians)
        signal[i] = sin(2.0 * M_PI * 16000.0 * t + M_PI/4)+sin(2.0*M_PI*17000.0*t + M_PI/4);
    }
    return signal;
}
float* create_phase_shifted_signal_both_2() {
    float* signal = malloc(NUM_SAMPLES * sizeof(float));
    for(int i = 0; i < NUM_SAMPLES; i++) {
        double t = (double)i / SAMPLE_RATE;
        // 1kHz with 45 degree phase shift (π/4 radians)
        signal[i] = sin(2.0 * M_PI * 16000.0 * t + M_PI/2)+sin(2.0*M_PI*17000.0*t + M_PI/2);
    }
    return signal;
}



// Test utilities
void print_results(double* results, float* frequencies, int num_freqs, const char* label) {
    printf("%s:\n", label);
    for(int i = 0; i < num_freqs; i++) {
        printf("  %.0f Hz: %f\n", frequencies[i], results[i]);
    }
    printf("\n");
}

int find_frequency_index(float* frequencies, int num_freqs, float target_freq) {
    for(int i = 0; i < num_freqs; i++) {
        if(fabsf(frequencies[i] - target_freq) < 1.0f) {
            return i;
        }
    }
    return -1;
}

// Test 1: Single frequency magnitude detection
void test_single_frequency_magnitude() {
    printf("=== Test 1: Single Frequency Magnitude ===\n");

    float frequency_map[] = {1000.0f, 2000.0f};
    int frequencies = 2;
    int oversampling = 4;
    int window_size = 1024;

    // Create 1kHz test signal
    float* test_signal = create_sine_wave(1000.0f, 1.0f, 0.0f);

    FFT fft = create_FFT(frequency_map, frequencies, oversampling, SAMPLE_RATE, SAMPLE_RATE);

    double* magnitude_results = calculate_FFT(fft, test_signal, NUM_SAMPLES);

    print_results(magnitude_results, frequency_map, frequencies, "Magnitude Results");

    // Verify results
    int idx_1k = find_frequency_index(frequency_map, frequencies, 1000.0f);
    int idx_2k = find_frequency_index(frequency_map, frequencies, 2000.0f);

    if(idx_1k >= 0 && idx_2k >= 0) {
        printf("Verification:\n");
        printf("  1kHz magnitude: %f (expected: high)\n", magnitude_results[idx_1k]);
        printf("  2kHz magnitude: %f (expected: low)\n", magnitude_results[idx_2k]);

        if(magnitude_results[idx_1k] > magnitude_results[idx_2k] * 10) {
            printf("  ✓ PASS: 1kHz correctly detected as dominant frequency\n");
        } else {
            printf("  ✗ FAIL: 1kHz not properly distinguished from 2kHz\n");
        }
    }

    free_FFT(fft);
    free(test_signal);
    printf("\n");
}

// Test 2: Multi-frequency magnitude detection
void test_multi_frequency_magnitude() {
    printf("=== Test 2: Multi-Frequency Magnitude ===\n");

    float frequency_map[] = {1000.0f, 2000.0f, 3000.0f};
    int frequencies = 3;
    int oversampling = 4;
    int window_size = 1024;

    // Create signal with 1kHz and 2kHz components
    float* test_signal = create_multi_frequency_signal();

    FFT fft = create_FFT(frequency_map, frequencies, oversampling, SAMPLE_RATE, SAMPLE_RATE);

    double* magnitude_results = calculate_FFT(fft, test_signal, NUM_SAMPLES);

    print_results(magnitude_results, frequency_map, frequencies, "Multi-Frequency Magnitude");

    // Verify results
    int idx_1k = find_frequency_index(frequency_map, frequencies, 1000.0f);
    int idx_2k = find_frequency_index(frequency_map, frequencies, 2000.0f);
    int idx_3k = find_frequency_index(frequency_map, frequencies, 3000.0f);

    if(idx_1k >= 0 && idx_2k >= 0 && idx_3k >= 0) {
        printf("Verification:\n");
        printf("  1kHz (0.8 amplitude): %f\n", magnitude_results[idx_1k]);
        printf("  2kHz (0.5 amplitude): %f\n", magnitude_results[idx_2k]);
        printf("  3kHz (0.0 amplitude): %f\n", magnitude_results[idx_3k]);

        if(magnitude_results[idx_1k] > magnitude_results[idx_2k] &&
           magnitude_results[idx_2k] > magnitude_results[idx_3k]) {
            printf("  ✓ PASS: Frequency magnitudes correctly ordered\n");
        } else {
            printf("  ✗ FAIL: Frequency magnitudes not in expected order\n");
        }
    }

    free_FFT(fft);
    free(test_signal);
    printf("\n");
}



// Test 3: Phase detection
void test_phase_detection() {
    printf("=== Test 3: Phase Detection ===\n");

    float frequency_map[] = {16000.0f,17000.0f};
    int frequencies = 2;
    int oversampling = 4;
    int window_size = 48;//sampling phase at 1khz

    // Create 1kHz signal with 45° phase shift
    float* test_signal = create_phase_shifted_signal_90();
    float* test_signal3 = create_phase_shifted_signal_90_2();
    float* test_signal1 = create_phase_shifted_signal_0();
    float* test_signal2 = create_phase_shifted_signal_both();
    float* test_signal4 = create_phase_shifted_signal_both_2();

    FFT fft = create_FFT(frequency_map, frequencies, oversampling, SAMPLE_RATE, SAMPLE_RATE);

    double* phase_results = calculate_FFT_phase(fft, test_signal, window_size);

    double phase_diff = fabs(phase_results[0] - (phase_results[1]))*57.295779513;
    printf("phase difference(90 degress): %f error: %f\n",phase_diff,90 - phase_diff);

    phase_results = calculate_FFT_phase(fft, test_signal3+4800, window_size);
    phase_diff = fabs(phase_results[0] - phase_results[1])*57.295779513;
    printf("phase difference(90 degrees): %f error: %f\n",phase_diff,90 - phase_diff);



    phase_results = calculate_FFT_phase(fft, test_signal1, window_size);
    phase_diff = fabs(phase_results[0] - phase_results[1])*57.295779513;
    printf("phase difference(0): %f error: %f\n",phase_diff,phase_diff);

    phase_results = calculate_FFT_phase(fft, test_signal2, window_size);
    phase_diff = fabs(phase_results[0] - phase_results[1])*57.295779513;
    printf("phase difference(0): %f error: %f\n",phase_diff,phase_diff);

    phase_results = calculate_FFT_phase(fft, test_signal4, window_size);
    phase_diff = fabs(phase_results[0] - phase_results[1])*57.295779513;
    printf("phase difference(0): %f error: %f\n",phase_diff,phase_diff);



    free_FFT(fft);
    free(test_signal);
    free(test_signal1);
    free(test_signal2);
    free(test_signal3);
    free(test_signal4);
    printf("\n");
}

// Test 4: Compare magnitude and phase on same signal
void test_magnitude_vs_phase() {
    printf("=== Test 4: Magnitude vs Phase Comparison ===\n");

    float frequency_map[] = {1000.0f, 2000.0f};
    int frequencies = 2;
    int oversampling = 4;
    int window_size = 1024;

    float* test_signal = create_multi_frequency_signal();

    FFT fft = create_FFT(frequency_map, frequencies, oversampling, SAMPLE_RATE, SAMPLE_RATE);

    // Test magnitude
    double* magnitude_results = calculate_FFT(fft, test_signal, NUM_SAMPLES);
    print_results(magnitude_results, frequency_map, frequencies, "Magnitude");

    // Reset FFT for phase test (since your implementation modifies state)
    free_FFT(fft);
    fft = create_FFT(frequency_map, frequencies, oversampling, SAMPLE_RATE, SAMPLE_RATE);

    // Test phase
    double* phase_results = calculate_FFT_phase(fft, test_signal, NUM_SAMPLES);

    // Convert phases to degrees
    printf("Phase Results (degrees):\n");
    for(int i = 0; i < frequencies; i++) {
        double phase_degrees = phase_results[i] * 180.0 / M_PI;
        printf("  %.0f Hz: %f°\n", frequency_map[i], phase_degrees);
    }

    printf("\nVerification:\n");
    printf("  Both frequencies should have different phases (not both 0°)\n");

    if(fabs(phase_results[0]) > 0.1 || fabs(phase_results[1]) > 0.1) {
        printf("  ✓ PASS: Non-zero phases detected\n");
    } else {
        printf("  ✗ FAIL: Phases appear to be zero\n");
    }

    free_FFT(fft);
    free(test_signal);
    printf("\n");
}

// Test 5: Frequency response across multiple frequencies
void test_frequency_response() {
    printf("=== Test 5: Frequency Response ===\n");

    // Test many frequencies to see response curve
    float frequency_map[] = {500.0f, 1000.0f, 1500.0f, 2000.0f, 2500.0f, 3000.0f};
    int frequencies = 6;
    int oversampling = 4;
    int window_size = 2048; // Larger window for better frequency resolution

    // Create signal with only 1kHz component
    float* test_signal = create_sine_wave(1000.0f, 1.0f, 0.0f);

    FFT fft = create_FFT(frequency_map, frequencies, oversampling, SAMPLE_RATE, SAMPLE_RATE);

    double* magnitude_results = calculate_FFT(fft, test_signal, NUM_SAMPLES);

    printf("Frequency Response for 1kHz input:\n");
    for(int i = 0; i < frequencies; i++) {
        printf("  %.0f Hz: %f\n", frequency_map[i], magnitude_results[i]);
    }

    // Find the peak
    int peak_index = 0;
    double peak_value = magnitude_results[0];
    for(int i = 1; i < frequencies; i++) {
        if(magnitude_results[i] > peak_value) {
            peak_value = magnitude_results[i];
            peak_index = i;
        }
    }

    printf("\nPeak at %.0f Hz: %f\n", frequency_map[peak_index], peak_value);

    if(fabs(frequency_map[peak_index] - 1000.0f) < 1.0f) {
        printf("  ✓ PASS: Correct peak frequency detected\n");
    } else {
        printf("  ✗ FAIL: Peak at wrong frequency\n");
    }

    free_FFT(fft);
    free(test_signal);
    printf("\n");
}

int main() {
    printf("FFT Implementation Test Suite\n");
    printf("Sampling Rate: %d Hz, Test Duration: %d samples\n\n", SAMPLE_RATE, NUM_SAMPLES);

    test_single_frequency_magnitude();
    test_multi_frequency_magnitude();
    test_phase_detection();
    test_magnitude_vs_phase();
    test_frequency_response();

    printf("=== Test Suite Complete ===\n");
    return 0;
}
