# Realtime Performance Validation

## Objective

The purpose of this study is to verify whether the nonlinear FEM solver
can perform a complete simulation timestep within the time available
in realtime operation.

The computational realtime condition is defined as:

t_compute < dt

where:

- `t_compute` is the wall-clock time required to perform one nonlinear FEM timestep,
- `dt` is the physical simulation timestep.

The realtime speedup factor is defined as:

S_RT = dt / t_compute

A value greater than 1 indicates that the FEM computation is completed
before the next simulation deadline.

## Test Configuration

The realtime FEM configuration was tested for three simulation timesteps:

- `dt = 10 s`
- `dt = 1 s`
- `dt = 0.1 s`

For each timestep, 50 nonlinear FEM steps were executed.

The measured computation time includes the nonlinear FEM timestep
performed by `TimeIntegrator::stepNonLinearWithHTC()`.

Serial communication waiting time is not included in `t_compute`.
Therefore, this study evaluates the computational capability of the
realtime FEM solver rather than the sampling frequency of the
measurement hardware.

Temperature values were supplied through the Arduino/serial acquisition
pipeline using a synthetic temperature signal.

## Results

| dt [s] | Mean compute time [ms] | Median [ms] | P95 [ms] | Maximum [ms] | Picard iterations | Minimum realtime speedup | Deadline misses |
|-------:|-----------------------:|------------:|---------:|-------------:|------------------:|-------------------------:|----------------:|
| 10.0 | 16.14 | 16.40 | 18.91 | 20.20 | 3 | 495 | 0 / 50 |
| 1.0 | 16.34 | 16.30 | 18.95 | 20.27 | 3 | 49.34 | 0 / 50 |
| 0.1 | 10.72 | 10.95 | 13.12 | 15.71 | 2 | 6.37 | 0 / 50 |

No realtime deadline violations were observed in any of the 150
evaluated FEM timesteps.

The most demanding realtime condition was `dt = 0.1 s`, corresponding
to a computational deadline of 100 ms.

The maximum measured FEM computation time for this case was approximately
15.71 ms, resulting in a minimum realtime speedup factor of approximately
6.37.

## Picard Iteration Behaviour

For `dt = 10 s` and `dt = 1 s`, the nonlinear solution required
3 Picard iterations per timestep.

For `dt = 0.1 s`, the solver required 2 Picard iterations per timestep
during the test.

The reduced number of nonlinear iterations is consistent with the
smaller temperature change occurring during a shorter timestep.

## Conclusion

The tested realtime FEM configuration satisfies the computational
realtime requirement for all investigated timesteps.

Even for the smallest investigated timestep (`dt = 0.1 s`), the maximum
observed FEM computation time remained substantially below the available
100 ms deadline.

The results demonstrate that the current solver configuration provides
sufficient computational performance for realtime execution down to
a simulation timestep of 0.1 s under the tested conditions.

This validation concerns the computational performance of the FEM solver.
End-to-end realtime operation additionally depends on the measurement
sampling rate and serial communication with the acquisition hardware.