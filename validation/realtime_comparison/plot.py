import pandas as pd
import matplotlib.pyplot as plt

accurate = pd.read_csv("accurate_summary.csv")
realtime = pd.read_csv("realtime_summary.csv")

accurate = accurate.rename(columns={"selected_node_temperature": "center_accurate",
"t_max": "tmax_accurate",
"t_min": "tmin_accurate",})

realtime = realtime.rename(columns={"selected_node_temperature": "center_realtime",
"t_max": "tmax_realtime", 
"t_min": "tmin_realtime",})

comparison = pd.merge(accurate[["time", "center_accurate", "tmax_accurate", "tmin_accurate"]],
realtime[["time", "center_realtime", "tmax_realtime", "tmin_realtime"]],
on="time", how="inner")

comparison["center_error"] = comparison["center_realtime"] - comparison["center_accurate"]
comparison["center_abs_error"] = comparison["center_error"].abs()

comparison["tmax_error"] = comparison["tmax_realtime"] - comparison["tmax_accurate"]
comparison["tmax_abs_error"] = comparison["tmax_error"].abs()

comparison.to_csv("comparison.csv", index=False)

#Plot 1: center temperature

plt.figure(figsize=(8, 5))
plt.plot(comparison["time"], comparison["center_accurate"], label="Accurate Mode", linewidth=2)
plt.plot(comparison["time"], comparison["center_realtime"], label="Realtime Mode", linewidth=2, linestyle="--")

plt.xlabel("Time [s]")
plt.ylabel("Center temperature [°C]")
plt.title("Center Temperature: Accurate vs Realtime")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("center_temperature_comparison.png", dpi=300)
plt.show()

# Plot 2: center error

plt.figure(figsize=(8, 5))
plt.plot(comparison["time"], comparison["center_error"], linewidth=2)

plt.xlabel("Time [s]")
plt.ylabel("Realtime - Accurate [°C]")
plt.title("Center Temperature difference")
plt.grid(True)
plt.tight_layout()
plt.savefig("center_temperature_difference.png", dpi=300)
plt.show()

# Plot 3: maximum temperature

plt.figure(figsize=(8, 5))
plt.plot(comparison["time"], comparison["tmax_accurate"], label="Accurate Mode", linewidth=2)
plt.plot(comparison["time"], comparison["tmax_realtime"], label="Realtime Mode", linewidth=2, linestyle="--")
plt.xlabel("Time [s]")
plt.ylabel("Maximum temperature [°C]")
plt.title("Maximum Temperature: Accurate vs Realtime")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("maximum_temperature_comparison.png", dpi=300)
plt.show()

print("Realtime comparison summary:")
print(f"Max center absolute error: {comparison['center_abs_error'].max():.6f} °C")
print(f"Mean center absolute error: {comparison['center_abs_error'].mean():.6f} °C")
print(f"Final center error: {comparison['center_error'].iloc[-1]:.6f} °C")
print(f"Max Tmax absolute error: {comparison['tmax_abs_error'].max():.6f} °C")
print(f"Mean Tmax absolute error: {comparison['tmax_abs_error'].mean():.6f} °C")