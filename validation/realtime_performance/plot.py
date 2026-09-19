import pandas as pd
import matplotlib.pyplot as plt


# Load performance results
performance_10 = pd.read_csv("performance_10.csv")
performance_1 = pd.read_csv("performance_1.csv")
performance_0_1 = pd.read_csv("performance_0_1.csv")


# Create figure
plt.figure(figsize=(10, 6))

plt.plot(
    performance_10["step"],
    performance_10["compute_time_ms"],
    marker="o",
    markersize=3,
    linewidth=1.2,
    label="dt = 10 s"
)

plt.plot(
    performance_1["step"],
    performance_1["compute_time_ms"],
    marker="o",
    markersize=3,
    linewidth=1.2,
    label="dt = 1 s"
)

plt.plot(
    performance_0_1["step"],
    performance_0_1["compute_time_ms"],
    marker="o",
    markersize=3,
    linewidth=1.2,
    label="dt = 0.1 s"
)


# Most demanding realtime deadline
plt.axhline(
    y=100.0,
    linestyle="--",
    linewidth=1.3,
    label="100 ms deadline (dt = 0.1 s)"
)


# Plot formatting
plt.xlabel("Simulation step")
plt.ylabel("FEM computation time [ms]")
plt.title("Realtime FEM performance validation")

plt.grid(alpha=0.25)
plt.legend()
plt.tight_layout()


# Save and display
plt.savefig(
    "realtime_performance.png",
    dpi=200,
    bbox_inches="tight"
)

plt.show()