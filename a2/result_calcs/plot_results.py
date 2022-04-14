import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from os.path import join

experiment = 'E2'

if experiment == 'E1':
    detinpath = join("results_csv", experiment+"Det.csv")
    expinpath = join("results_csv", experiment+"Exp.csv")
else:
    csma_inpath = join("results_csv", experiment+"Csma.csv")
    aloha_inpath = join("results_csv", experiment+"Aloha.csv")

outpath = "out_results"

def e1func(inpath):
    buffResult_dict = {}
    uniquePkts_dict = {}
    df = pd.read_csv(inpath)
    grouped_df = df[df['attrname'] == "interArrivalTime"].groupby('attrvalue')
    for key, item in grouped_df:
        indices = grouped_df.get_group(key).index
        runs = []
        for ix in indices:
            runs.append(df['run'][ix])
        filtered_df =df[df['run'].isin(runs)]
        bufferLossList = filtered_df[filtered_df['name'] == "bufferLossSig:count"][filtered_df['type'] == 'scalar'].value[1::2]
        uniquePktsList = filtered_df[filtered_df['name'] == "rxUniquePacketsSig:count"][filtered_df['type'] == 'scalar'].value[::2]
        buffResult_dict[int(key)] = np.mean(bufferLossList)
        uniquePkts_dict[int(key)] = np.mean(uniquePktsList)
    interarrival_times = sorted(list(buffResult_dict.keys()))
    bufferLosses = [buffResult_dict[ix] + 1 for ix in interarrival_times]
    uniquePkts = [uniquePkts_dict[ix] for ix in interarrival_times]

    return interarrival_times, bufferLosses, uniquePkts


def e2func(inpath):
    uniquePkts_dict = {}
    df = pd.read_csv(inpath)
    grouped_df = df[df['attrname'] == "radius"].groupby('attrvalue')
    for key, item in grouped_df:
        indices = grouped_df.get_group(key).index
        runs = []
        for ix in indices:
            runs.append(df['run'][ix])
        filtered_df = df[df['run'].isin(runs)]
        uniquePktsList = filtered_df[filtered_df['name'] == "rxUniquePacketsSig:count"][filtered_df['type'] == 'scalar'].value[0::11]
        uniquePkts_dict[int(key)] = np.mean(uniquePktsList)
    radii = sorted(list(uniquePkts_dict.keys()))
    uniquePkts = [uniquePkts_dict[ix] for ix in radii]
    return radii, uniquePkts


def e3e4func(inpath):
    uniquePkts_dict = {}
    df = pd.read_csv(inpath)
    grouped_df = df[df['attrname'] == "numTransmitters"].groupby('attrvalue')
    for key, item in grouped_df:
        indices = grouped_df.get_group(key).index
        runs = []
        for ix in indices:
            runs.append(df['run'][ix])
        filtered_df = df[df['run'].isin(runs)]
        uniquePktsList = filtered_df[filtered_df['name'] == "rxUniquePacketsSig:count"][filtered_df['type'] == 'scalar'].value[0::int(key)+1]
        uniquePkts_dict[int(key)] = np.mean(uniquePktsList)
    radii = sorted(list(uniquePkts_dict.keys()))
    uniquePkts = [uniquePkts_dict[ix] for ix in radii]
    return radii, uniquePkts


if __name__ == '__main__':

    if experiment == 'E1':
        interarrival_timesDet, bufferLossesDet, uniquePktsDet = e1func(detinpath)
        interarrival_timesExp, bufferLossesExp, uniquePktsExp = e1func(expinpath)
        plt.plot(interarrival_timesDet, bufferLossesDet, label='Deterministic', linewidth=4)
        plt.plot(interarrival_timesExp, bufferLossesExp, label='Exponential')
        ax = plt.gca()
        ax.set_yscale('log')
        plt.title("Packets discarded at full buffer")
        plt.xlabel("Interarrival times [ms]")
        plt.ylabel("Numbers of packets lost")
        plt.legend()
        plt.savefig(join(outpath, "E1BufferLoss"))
        # plt.show()
        plt.close()

        plt.plot(interarrival_timesDet, uniquePktsDet, label='Deterministic', linewidth=4)
        plt.plot(interarrival_timesExp, uniquePktsExp, label='Exponential')
        ax = plt.gca()
        ax.set_yscale('log')
        plt.title("Unique packets received by the application")
        plt.xlabel("Interarrival times [ms]")
        plt.ylabel("Numbers of unique packets")
        plt.legend()
        plt.savefig(join(outpath, "E1UniquePkts"))
        # plt.show()
    if experiment == 'E2':
        radiiCsma, uniquePktsCsma = e2func(csma_inpath)
        radiiAloha, uniquePktsAloha = e2func(aloha_inpath)
        plt.plot(radiiCsma, uniquePktsCsma, label='Csma', linewidth=4)
        plt.plot(radiiAloha, uniquePktsAloha, label='Aloha')
        ax = plt.gca()
        ax.set_yscale('log')
        plt.title("Unique packets received by the application")
        plt.xlabel("Radius of transmitter circle (distance from receiver)")
        plt.ylabel("Numbers of unique packets")
        plt.legend()
        plt.savefig(join(outpath, "E2UniquePkts"))
        # plt.show()
    if experiment == 'E3' or experiment == 'E4':
        radiusDict = {'E3':'5m', 'E4':'20m'}
        transNoCsma, uniquePktsCsma = e3e4func(csma_inpath)
        transNoAloha, uniquePktsAloha = e3e4func(aloha_inpath)
        plt.plot(transNoCsma, uniquePktsCsma, label='Csma', linewidth=4)
        plt.plot(transNoAloha, uniquePktsAloha, label='Aloha')
        ax = plt.gca()
        ax.set_yscale('log')
        plt.title("Unique packets received by the application for a radius of " + radiusDict[experiment])
        plt.xlabel("Number of transmitters")
        plt.ylabel("Numbers of unique packets")
        plt.legend()
        plt.savefig(join(outpath, experiment + "UniquePkts"))
        # plt.show()