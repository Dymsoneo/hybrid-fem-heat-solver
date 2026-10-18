# Acquisition performance validation

## Objective

Evaluate timing regularity and frame continuity of the physical temperature-acquisition chain used by the hybrid FEM heat solver. This is an **acquisition-only** test, independent of the FEM computation. The test does **not** measure end-to-end FEM latency, absolute sensor-to-PC latency, thermocouple accuracy, or thermal response time.

## Hardware and communication

- Arduino UNO R3; MAX6675 thermocouple interface; type-K thermocouple.
- MAX6675: SCK → D6, CS → D5, SO → D4; serial USB/COM at 9600 baud.
- Each frame: `sample_id,arduino_time_ms,temperature_c` terminated by newline.
- Arduino timestamp from `millis()` taken immediately before the MAX6675 read; this is **not** an independently verified instant of thermocouple conversion.
- PC timestamp from `std::chrono::steady_clock` when the complete line is received. The two timestamps use independent, unsynchronized clocks.
- The host program opens the COM port, waits for UNO startup/reset, discards startup-buffered bytes and collects 200 valid frames per test. Accordingly, first recorded IDs are not necessarily zero.

## Procedure

The configured Arduino sampling period was varied between 1000 ms (1 Hz), 500 ms (2 Hz), and 250 ms (4 Hz). For each run, 200 successive valid frames were recorded. Analyze consecutive timestamp differences separately for Arduino and PC. Count missing IDs *within the recorded sequence*; calculate the 95th percentile of `abs(PC_interval - configured_interval)` and relative elapsed-time drift:

`drift_i = (pc_time_i - pc_time_first) - (arduino_time_i - arduino_time_first)`.

Do not subtract absolute PC and Arduino timestamps directly: their origins differ.

## Results

| Metric | 1 Hz | 2 Hz | 4 Hz |
|---|---:|---:|---:|
| Frames | 200 | 200 | 200 |
| First–last ID | 0–199 | 37–236 | 30–229 |
| Missing IDs inside captured sequence | 0 | 0 | 0 |
| Mean Arduino interval [ms] | 1000.000 | 500.000 | 250.000 |
| Arduino interval min–max [ms] | 1000–1000 | 499–501 | 249–251 |
| Mean PC interval [ms] | 1001.802 | 500.888 | 250.445 |
| PC interval min–max [ms] | 998.0-1005.0 | 498.0–504.7 | 248.9–254.2 |
| PC interval SD [ms] | 2.017 | 1.875 | 1.448 |
| P95 absolute PC interval deviation [ms] | 4.0 | 3.9 | 4.0 |
| Effective PC arrival rate [Hz] | 0.9982 | 1.9965 | 3.9929 |
| Relative elapsed-time drift [ms] | 358.662 | 176.772 | 88.479 |



### Interpretation

All captured sequences have continuous sample IDs. Arduino scheduling is regular within the 1 ms resolution of `millis()`. PC arrivals show small short-term interval deviations, including periodic deviations of a few milliseconds, but no captured frame loss. The mean PC interval exceeds the Arduino-reported interval by approximately 0.178% in each test. This suggests a systematic difference between the two time bases and/or timing of acquisition/receipt; it **does not establish** an absolute communication delay. Longer independent-clock experiments or synchronization instrumentation would be required to isolate causes.


### Recommendation

Use **2 Hz (500 ms)** as a provisional default for subsequent real-time integration: stable observed transport and comfortable margin relative to MAX6675 conversion time. 1 Hz is also suitable for slow furnace transients; 4 Hz remains a useful stress test. The final selection should be confirmed against furnace thermal dynamics and the actual FEM update policy.

