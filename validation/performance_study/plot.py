import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("performance_results.csv")

df_linear = df[df["mode"] == "linear"]
df_nonlinear = df[df["mode"] == "nonlinear"]

# Plot 1: Total time vs elements

plt.figure()
plt.plot(df_linear["elements"], df_linear["total_time_s"], marker='o', label = "Linear")
plt.plot(df_nonlinear["elements"], df_nonlinear["total_time_s"], marker='o', linestyle='--', label = "Nonlinear")

plt.xlabel("Number of elements")
plt.ylabel("Total simulation time [s]")
plt.title("Performance scaling with mesh size")
plt.legend()
plt.grid()

plt.savefig("performance_total_time.png")
plt.show()

# Plot 2: Avg step time

plt.figure()
plt.plot(df_linear["elements"], df_linear["avg_step_time_s"], marker='o', label = "Linear")
plt.plot(df_nonlinear["elements"], df_nonlinear["avg_step_time_s"], marker='o', linestyle='--', label = "Nonlinear")

plt.xlabel("Number of elements")
plt.ylabel("Average step time [s]")
plt.title("Average time per step")
plt.legend()
plt.grid()

plt.savefig("performance_step_time.png")
plt.show()

# PLot 3: Nonlinear overhead

plt.figure()

ratio = df_nonlinear["total_time_s"].values / df_linear["total_time_s"].values

plt.plot(df_linear["elements"], ratio, marker='o')

plt.xlabel("Number of elements")
plt.ylabel("Nonlinear / Linear time ratio")
plt.title("Cost of Picard iteration")
plt.grid()

plt.savefig("performance_picard_cost.png")
plt.show()