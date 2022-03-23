import re
from turtle import color
import pandas as pd
  
with open("vit_e0p0x2-7.log", "r") as fp:
  text = fp.read()

start_groups = re.findall(r"\[(\d+)\].*start (\d+)", text)
end_groups = re.findall(r"\[(\d+)\].*end (\d+)", text)

start_groups = dict(start_groups)
end_groups = dict(end_groups)

paired_groups = {k: (v, end_groups[k]) for k,v in start_groups.items()}

ids = list(paired_groups.keys())
paired_timestamps = list(paired_groups.values())
# print(paired_timestamps)
start_timestamps, end_timestamps = tuple(zip(*paired_timestamps))

# print(start_timestamps)

df = pd.DataFrame({"ids": ids, "start_timestamps": start_timestamps, "end_timestamps": end_timestamps})
df.to_csv("model-time.csv", index=False)

with open("build/vit-e2p0x2-7.log", "r") as fp:
  lines = fp.readlines()

powers = []
timestamps = []

for line in lines:
  groups = re.findall(r"\((\d+)\).*Power\(mW\) (\d+) ", line)[0]
  # print(groups)
  timestamp, power = groups

  powers.append(int(power))
  timestamps.append(int(timestamp))

df = pd.DataFrame({"powers":powers, "timestamps":timestamps})
print(df.timestamps.min(), df.timestamps.max())
df = df[df.timestamps >= int(start_timestamps[0]) - 1000000000]
print(df.timestamps <  int(start_timestamps[0]))
df = df.loc[df.powers.shift() != df.powers]
df.to_csv("vit-e2p0x2-7.csv", index=False)

print("first start,end", int(start_timestamps[0]), int(end_timestamps[0]))

import seaborn as sns
import matplotlib.pyplot as plt

plt.figure(figsize=(25,25))
sns.scatterplot(x="timestamps", y="powers", data=df)
# for i in range(len(start_timestamps)):
#   plt.axvline(x=int(start_timestamps[i]), color="r", linewidth=.5, linestyle="--")
#   plt.axvline(x=int(end_timestamps[i]), color="y", linewidth=.5, linestyle="--")


from scipy.optimize import curve_fit

import numpy as np

# def func(x, a, b, c, loc, power):
#   return - a * np.power(power, - b * (x - loc)) + c

df_fit = df.head(100)

def func(x, a, t, loc):
  return a / (1+np.exp(-t*(x-loc))) + 22

# df_fit = df[(df.timestamps >= int(start_timestamps[0])) & (df.timestamps <= int(end_timestamps[-500]))]
# df_fit = df[df.timestamps > int(end_timestamps[-1])]

# df_fit = df_fit.loc[df_fit.powers.shift() != df_fit.powers]

df_fit['tnew'] = (df.timestamps - df.timestamps[0]) / 1000 / 1000 / 1000
print(df_fit)
df_fit.info()
popt, pcov = curve_fit(func, df_fit.tnew.to_numpy().flatten(), df_fit.powers.to_numpy().flatten(), bounds=(0,np.inf))
print(popt, pcov)
# popt = [104-22, 1, 1, 10000, 22]

params = np.polyfit(df_fit.tnew.to_numpy().flatten(), df_fit.powers.to_numpy().flatten(), 1)
print(params)
polyfunc = np.poly1d(params)

# df_fit['pred'] = func(df_fit.tnew.to_numpy(), *popt)
df_fit['pred'] = polyfunc(df_fit.tnew.to_numpy())
print(df_fit['pred'])
sns.scatterplot(x="timestamps", y="pred", data=df_fit)

plt.savefig("test.png")
plt.close()



