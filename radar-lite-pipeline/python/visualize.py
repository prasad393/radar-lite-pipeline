# Simple visualization for RD map and detections
import sys, csv
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) < 3:
    print("Usage: python visualize.py <rd_map.csv> <detections.csv>")
    sys.exit(1)

rd_path, det_path = sys.argv[1], sys.argv[2]

rd = np.loadtxt(rd_path, delimiter=",")
plt.figure()
plt.imshow(rd, aspect='auto', origin='lower')
plt.title("Range-Doppler (avg power)")
plt.xlabel("Range bin")
plt.ylabel("Doppler bin")
plt.colorbar()
plt.tight_layout()

# plot detections
det_r, det_d = [], []
with open(det_path) as f:
    rdr = csv.DictReader(f)
    for row in rdr:
        det_r.append(int(row["range_bin"]))
        det_d.append(int(row["doppler_bin"]))

plt.scatter(det_r, det_d, s=10, marker='x')
plt.title("Detections overlay")
plt.show()
