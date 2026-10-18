from pathlib import Path
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

ROOT = Path(__file__).resolve().parent
FREQUENCIES = {'1hz': 1000.0, '2hz': 500.0, '4hz': 250.0}
frames = {}
records = []
for label, target in FREQUENCIES.items():
    path = ROOT / f'acquisition_{label}.csv'
    if not path.exists():
        print(f'Skipping missing file: {path.name}')
        continue
    df = pd.read_csv(path)
    if len(df) < 2:
        continue
    for col in ['sample_id', 'arduino_time_ms', 'pc_time_ms', 'temperature_c']:
        if col not in df:
            raise ValueError(f'{path.name}: missing column {col}')
    frames[label] = df
    ar = df.arduino_time_ms.diff().dropna()
    pc = df.pc_time_ms.diff().dropna()
    missing = int((df.sample_id.diff().dropna() - 1).clip(lower=0).sum())
    drift = float((df.pc_time_ms.iloc[-1] - df.pc_time_ms.iloc[0]) - (df.arduino_time_ms.iloc[-1] - df.arduino_time_ms.iloc[0]))
    records.append(dict(test=label, target_period_ms=target, samples=len(df), first_id=int(df.sample_id.iloc[0]), last_id=int(df.sample_id.iloc[-1]), missing_ids=missing, arduino_mean_ms=ar.mean(), arduino_std_ms=ar.std(ddof=1), arduino_min_ms=ar.min(), arduino_max_ms=ar.max(), pc_mean_ms=pc.mean(), pc_std_ms=pc.std(ddof=1), pc_min_ms=pc.min(), pc_max_ms=pc.max(), pc_p95_absolute_jitter_ms=np.quantile(np.abs(pc-target), .95), pc_effective_hz=1000/pc.mean(), drift_ms=drift, relative_drift_percent=100*drift/(df.arduino_time_ms.iloc[-1]-df.arduino_time_ms.iloc[0])))

if not frames:
    raise SystemExit('No acquisition CSV files found.')
pd.DataFrame(records).to_csv(ROOT / 'summary.csv', index=False, float_format='%.6f')

fig, ax = plt.subplots(figsize=(10, 5.2))
for label, df in frames.items():
    target = FREQUENCIES[label]
    x = (df.arduino_time_ms.iloc[1:].to_numpy()-df.arduino_time_ms.iloc[0])/1000
    ax.plot(x, df.pc_time_ms.diff().iloc[1:].to_numpy()-target, label=f'{label} (target {target:g} ms)', linewidth=1.1)
ax.axhline(0, color='black', linewidth=.7)
ax.set(xlabel='Time since first sample, Arduino clock [s]', ylabel='PC inter-arrival interval minus target [ms]', title='Acquisition timing deviations')
ax.grid(alpha=.25)
ax.legend()
fig.tight_layout()
fig.savefig(ROOT/'interval_jitter.png',dpi=180)
plt.close(fig)

fig, ax = plt.subplots(figsize=(10, 5.2))
for label, df in frames.items():
    ar = df.arduino_time_ms.to_numpy()-df.arduino_time_ms.iloc[0]
    pc = df.pc_time_ms.to_numpy()-df.pc_time_ms.iloc[0]
    ax.plot(ar/1000,pc-ar,label=label)
ax.set(xlabel='Time since first sample, Arduino clock [s]',ylabel='Elapsed PC time minus elapsed Arduino time [ms]',title='Relative clock drift (not one-way transmission latency)')
ax.grid(alpha=.25)
ax.legend()
fig.tight_layout()
fig.savefig(ROOT/'relative_clock_drift.png',dpi=180)
plt.close(fig)
print(pd.DataFrame(records).to_string(index=False))
