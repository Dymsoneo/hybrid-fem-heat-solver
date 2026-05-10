import pandas as pd
import matplotlib.pyplot as plt

constant = pd.read_csv("constant_htc_summary.csv")
variable = pd.read_csv("variable_htc_summary.csv")
contact = pd.read_csv("variable_htc_contact_summary.csv")

comparison = pd.DataFrame()
comparison["time"] = constant["time"]

comparison["center_constant"] = constant["selected_node_temperature"]
comparison["center_variable"] = variable["selected_node_temperature"]
comparison["center_contact"] = contact["selected_node_temperature"]

comparison["tmax_constant"] = constant["t_max"]
comparison["tmax_variable"] = variable["t_max"]
comparison["tmax_contact"] = contact["t_max"]

comparison["variable_minus_constant"] = (
    comparison["center_variable"] - comparison["center_constant"]
)

comparison["contact_minus_variable"] = (
    comparison["center_contact"] - comparison["center_variable"]
)

comparison["contact_minus_constant"] = (
    comparison["center_contact"] - comparison["center_constant"]
)

comparison.to_csv("comparison.csv", index=False)

# Plot 1: Center temperature
plt.figure(figsize=(8, 5))
plt.plot(comparison["time"], comparison["center_constant"], label="Constant HTC = 25", linewidth=2)
plt.plot(comparison["time"], comparison["center_variable"], label="Variable HTC", linewidth=2)
plt.plot(comparison["time"], comparison["center_contact"], label="Variable HTC + bottom contact", linestyle="--", linewidth=2)

plt.xlabel("Time [s]")
plt.ylabel("Center temperature [°C]")
plt.title("Center temperature for different HTC models")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("center_temperature_htc_comparison.png", dpi=300)
plt.show()

# Plot 2: Maximum temperature
plt.figure(figsize=(8, 5))
plt.plot(comparison["time"], comparison["tmax_constant"], label="Constant HTC = 25", linewidth=2)
plt.plot(comparison["time"], comparison["tmax_variable"], label="Variable HTC", linewidth=2)
plt.plot(comparison["time"], comparison["tmax_contact"], label="Variable HTC + bottom contact", linestyle="--", linewidth=2)

plt.xlabel("Time [s]")
plt.ylabel("Maximum temperature [°C]")
plt.title("Maximum temperature for different HTC models")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("maximum_temperature_htc_comparison.png", dpi=300)
plt.show()

# Plot 3: Differences
plt.figure(figsize=(8, 5))
plt.plot(comparison["time"], comparison["variable_minus_constant"], label="Variable - Constant", linewidth=2)
plt.plot(comparison["time"], comparison["contact_minus_variable"], label="Contact - Variable", linewidth=2)
plt.plot(comparison["time"], comparison["contact_minus_constant"], label="Contact - Constant", linestyle="--", linewidth=2)

plt.xlabel("Time [s]")
plt.ylabel("Temperature difference [°C]")
plt.title("Influence of variable HTC and bottom contact")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("htc_temperature_differences.png", dpi=300)
plt.show()

print("HTC model check summary:")
print(f"Final center temp, constant HTC 25: {comparison['center_constant'].iloc[-1]:.6f} °C")
print(f"Final center temp, variable HTC: {comparison['center_variable'].iloc[-1]:.6f} °C")
print(f"Final center temp, variable HTC + contact: {comparison['center_contact'].iloc[-1]:.6f} °C")

print()
print(f"Variable - constant final difference: {comparison['variable_minus_constant'].iloc[-1]:.6f} °C")
print(f"Contact - variable final difference: {comparison['contact_minus_variable'].iloc[-1]:.6f} °C")
print(f"Contact - constant final difference: {comparison['contact_minus_constant'].iloc[-1]:.6f} °C")

print()
print(f"Max variable - constant difference: {comparison['variable_minus_constant'].max():.6f} °C")
print(f"Max contact - variable difference: {comparison['contact_minus_variable'].max():.6f} °C")