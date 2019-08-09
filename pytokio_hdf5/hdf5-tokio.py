#%matplotlib inline

import matplotlib
matplotlib.rcParams.update({'font.size': 14})
import matplotlib.pyplot as plt
import numpy as np
import datetime
import tokio.tools
#import PyQt4
import os
import datetime
import tokiotest
import tokio.config
import tokio.tools.hdf5
import tokio.connectors.hdf5
from test_connectors_hdf5 import DATASETS_1D, DATASETS_2D




def plot_hdf5():
    start_time = datetime.datetime(2016, 7, 6, 12, 37, 49)
    end_time = datetime.datetime(2016, 7, 6, 12, 54, 45)
    file_system = 'cscratch'

    result_reads = tokio.tools.hdf5.get_dataframe_from_time_range(
                fsname=file_system,
                dataset_name='datatargets/readrates',
                datetime_start=start_time,
                datetime_end=end_time)
    result_reads.head()


    result_writes = tokio.tools.hdf5.get_dataframe_from_time_range(
                fsname=file_system,
                dataset_name='datatargets/writerates',
                datetime_start=start_time,
                datetime_end=end_time)
    result_writes.head()

    plot_df = ((result_writes+result_reads) / 2.0**30.0)
    plot_title = "Overall OST I/O Rates"

    fig, ax = matplotlib.pyplot.subplots()
    fig.set_size_inches(10, 8)
    fig.suptitle(plot_title)

    ### Convert bytes/sec to GiB/sec
    plot_df.plot.area(ax=ax)

    ax.grid(True)
    ax.legend_.remove()
    ax.set_ylabel("GiB/sec")
    ax.set_xlabel("Time")

    fig, ax = plt.subplots(figsize=(15, 10))
    fig.suptitle(plot_title)
    heatmap = ax.matshow(plot_df.transpose(),
                         cmap='OrRd',
                         aspect=(len(plot_df.transpose().index)/len(plot_df.transpose().columns)))
    ax.xaxis.tick_bottom()
    ax.invert_yaxis()
    cbar = plt.colorbar(heatmap)
    cbar.ax.set_ylabel("I/O Rate (GiB/sec)")

    ### set the OST name labels (y axis)
    new_labels = ax.get_yticks().tolist()
    for i, label in enumerate(new_labels):
        column_index = int(new_labels[i])
        if column_index < 0 or column_index >= len(plot_df.columns):
            pass
        else:
            new_labels[i] = plot_df.columns[column_index]
    ax.set_yticklabels(new_labels)

    ### set the timestamp labels (x axis)
    new_labels = ax.get_xticks().tolist()
    for i, label in enumerate(new_labels):
        row_index = int(new_labels[i])
        if row_index < 0 or row_index >= len(plot_df.index):
            pass
        else:
            new_labels[i] = plot_df.index[row_index].to_pydatetime()
    ax.set_xticklabels(new_labels)

    for label in ax.get_xticklabels():
        label.set_rotation(90)
    #fig = plt.figure()
    fig.savefig('hdfplot.png')
    plt.show()

if __name__ == "__main__":
    plot_hdf5()
