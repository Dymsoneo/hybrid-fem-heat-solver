import pandas as pd
import matplotlib.pyplot as plt

# === SolidWorks ===
sw = pd.read_csv("solidworks_center_temp.csv")

time_sw = pd.to_numeric(sw["X"], errors="coerce")
T_sw = pd.to_numeric(sw["Y"], errors="coerce")

# === FEM ===
fem = pd.read_csv("fem_results.csv")


time_fem = pd.to_numeric(fem["time"], errors="coerce")
T_fem = pd.to_numeric(fem["selected_node_temperature"], errors="coerce")


sw_mask = time_sw.notna() & T_sw.notna()
fem_mask = time_fem.notna() & T_fem.notna()

time_sw = time_sw[sw_mask]
T_sw = T_sw[sw_mask]

time_fem = time_fem[fem_mask]
T_fem = T_fem[fem_mask]


plt.figure(figsize=(8, 5))
plt.plot(time_fem, T_fem, label="FEM (own solver)", linewidth=2)
plt.plot(time_sw, T_sw, label="SolidWorks", linestyle="--", linewidth=2)

plt.xlabel("Time [s]")
plt.ylabel("Temperature [°C]")
plt.title("Temperature at cylinder center")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.savefig("comparison_plot.png", dpi=300)
plt.show()

comparison = pd.merge(
    fem[["time", "selected_node_temperature"]],
    sw[["X", "Y"]],
    left_on="time",
    right_on="X",
    how="inner"
)

comparison["abs_error"] = (comparison["selected_node_temperature"] - comparison["Y"]).abs()

print("Max error:", comparison["abs_error"].max())
print("Mean error:", comparison["abs_error"].mean())

comparison.to_csv("comparison.csv", index=False)

plt.figure(figsize=(8, 5))
plt.plot(comparison["time"], comparison["abs_error"], linewidth=2)

plt.xlabel("Time [s]")
plt.ylabel("Absolute error [°C]")
plt.title("Error vs time")
plt.grid(True)

plt.tight_layout()
plt.savefig("error_plot.png", dpi=300)
plt.show()