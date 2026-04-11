import pandas as pd
import matplotlib.pyplot as plt

# Load the data from the CSV file
linear = pd.read_csv('linear_summary.csv')
nonlinear = pd.read_csv('nonlinear_summary.csv')


time_linear = pd.to_numeric(linear['time'], errors='coerce')
time_nonlinear = pd.to_numeric(nonlinear['time'], errors='coerce')

center_linear = pd.to_numeric(linear['selected_node_temperature'], errors='coerce')
center_nonlinear = pd.to_numeric(nonlinear['selected_node_temperature'], errors='coerce')

tmax_linear = pd.to_numeric(linear['t_max'], errors='coerce')
tmax_nonlinear = pd.to_numeric(nonlinear['t_max'], errors='coerce')

# Remove invalid rows (if any)
linear_mask = time_linear.notna() & center_linear.notna() & tmax_linear.notna()
nonlinear_mask = time_nonlinear.notna() & center_nonlinear.notna() & tmax_nonlinear.notna()

linear_clean = pd.DataFrame({
    'time': time_linear[linear_mask],
    'center_linear': center_linear[linear_mask],
    'tmax_linear': tmax_linear[linear_mask]
})

nonlinear_clean = pd.DataFrame({
    'time': time_nonlinear[nonlinear_mask],
    'center_nonlinear': center_nonlinear[nonlinear_mask],
    'tmax_nonlinear': tmax_nonlinear[nonlinear_mask]
})

# Merge on time
comparison = pd.merge(linear_clean, nonlinear_clean, on='time', how='inner')

# Differences
comparison['center_difference'] = comparison['center_nonlinear'] - comparison['center_linear']
comparison['center_abs_difference'] = comparison['center_difference'].abs()

comparison['tmax_difference'] = comparison['tmax_nonlinear'] - comparison['tmax_linear']
comparison['tmax_abs_difference'] = comparison['tmax_difference'].abs()

# Save merged data to CSV
comparison.to_csv('comparison_summary.csv', index=False)

# Plot 1: Center Temperature Comparison
plt.figure(figsize=(8, 5))
plt.plot(comparison['time'], comparison['center_linear'], label='Linear model', linewidth=2)
plt.plot(comparison['time'], comparison['center_nonlinear'], label='Nonlinear model', linestyle='--', linewidth=2)

plt.xlabel('Time [s]')
plt.ylabel('Temperature at center [°C]')
plt.title('Center Temperature: linear vs nonlinear model')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('center_temperature_comparison.png', dpi=300)
plt.show()

# Plot 2: Maximum Temperature Comparison
plt.figure(figsize=(8, 5))
plt.plot(comparison['time'], comparison['tmax_linear'], label='Linear model', linewidth=2)
plt.plot(comparison['time'], comparison['tmax_nonlinear'], label='Nonlinear model', linestyle='--', linewidth=2)

plt.xlabel('Time [s]')
plt.ylabel('Maximum Temperature [°C]')
plt.title('Maximum Temperature: linear vs nonlinear model')
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig('max_temperature_comparison.png', dpi=300)
plt.show()

# Plot 3: Center Temperature Difference
plt.figure(figsize=(8, 5))
plt.plot(comparison['time'], comparison['center_difference'], linewidth=2)

plt.xlabel('Time [s]')
plt.ylabel('T_nonlinear - T_linear [°C]')
plt.title('Difference in Center Temperature')
plt.grid(True)
plt.tight_layout()
plt.savefig('temperature_difference.png', dpi=300)
plt.show()

# Print summary
print("Comparison Summary:")
print(f"Max center temperature difference: {comparison['center_abs_difference'].max():.6f} °C")
print(f"Mean center temperature difference: {comparison['center_abs_difference'].mean():.6f} °C")
print(f"Final center temperature difference: {comparison['center_difference'].iloc[-1]:.6f} °C")

print(f"Max Tmax difference: {comparison['tmax_abs_difference'].max():.6f} °C")
print(f"Mean Tmax difference: {comparison['tmax_abs_difference'].mean():.6f} °C")