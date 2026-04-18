import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("mesh_convergence_results.csv")

# Reference values = finest mesh

T_center_ref = df["T_center_final"].iloc[-1]
T_max_ref = df["T_max_final"].iloc[-1]

# Errors relative to reference

df["center_abs_error"] = (df["T_center_final"] - T_center_ref).abs()
df["tmax_abs_error"] = (df["T_max_final"] - T_max_ref).abs()

df["center_rel_error_percent"] = (df["center_abs_error"] / abs(T_center_ref) * 100)
df["tmax_rel_error_percent"] = (df["tmax_abs_error"] / abs(T_max_ref) * 100)

# Save enriched table
df.to_csv("mesh_convergence_comparison.csv", index=False)\


# Plot 1: center temperature
plt.figure(figsize=(8, 5))
plt.plot(df["elements"], df["T_center_final"], marker='o', linewidth=2)
plt.xlabel("Number of elements")
plt.ylabel("Final center temperature [°C]")
plt.title("Mesh convergence: center temperature")
plt.grid(True)
#plt.tight_layout()
plt.ticklabel_format(axis="y", style="plain", useOffset=False)
plt.savefig("mesh_convergence_center.png", dpi=300)
plt.show()

# Plot 2: max temperature
plt.figure(figsize=(8, 5))
plt.plot(df["elements"], df["T_max_final"], marker='o', linewidth=2)
plt.xlabel("Number of elements")
plt.ylabel("Final max temperature [°C]")
plt.title("Mesh convergence: max temperature")
plt.grid(True)
#plt.tight_layout()
plt.ticklabel_format(axis="y", style="plain", useOffset=False)
plt.savefig("mesh_convergence_tmax.png", dpi=300)
plt.show()

# Plot 3: center error relative to reference
plt.figure(figsize=(8, 5))
plt.plot(df["elements"], df["center_abs_error"], marker='o', linewidth=2)
plt.xlabel("Number of elements")
plt.ylabel("Absolute error in center temperature [°C]")
plt.title("Mesh convergence error (reference = finest mesh)")
plt.grid(True)
plt.tight_layout()
plt.savefig("mesh_convergence_error.png", dpi=300)
plt.show()

print("Mesh convergence summary:")
print(df[["mesh", 
"elements", 
"T_center_final", 
"T_max_final", 
"center_abs_error", 
"center_rel_error_percent", 
"tmax_abs_error", 
"tmax_rel_error_percent"]])