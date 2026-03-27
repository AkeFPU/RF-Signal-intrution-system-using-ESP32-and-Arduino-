import numpy as np
import pandas as pd
from sklearn.ensemble import IsolationForest
import joblib

print("🧹 Erasing old data and creating new strict baseline...")

num_samples = 2000  
num_channels = 128  

# Create pure background noise (-105)
normal_data = np.full((num_samples, num_channels), -105)

# Add NORMAL Wi-Fi traffic (1 to 5 random channels spiking to -40 per sweep)
# The AI will learn this is completely safe and normal.
for i in range(num_samples):
    num_spikes = np.random.randint(0, 6) # Max 5 spikes
    random_channels = np.random.choice(num_channels, size=num_spikes, replace=False)
    normal_data[i, random_channels] = -40

df = pd.DataFrame(normal_data, columns=[f"Ch_{i}" for i in range(num_channels)])

print("🧠 Training AI to ignore normal Wi-Fi...")
# contamination=0.005 means it will only trigger on the top 0.5% most extreme anomalies
model = IsolationForest(n_estimators=100, contamination=0.005, random_state=42)
model.fit(df)

joblib.dump(model, "isolation_forest_model.pkl")
print("✅ New AI Brain Saved! (isolation_forest_model.pkl)")