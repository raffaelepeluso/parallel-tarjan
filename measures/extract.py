import os
import numpy as np
import scipy as sp
import matplotlib.pyplot as plt
import pandas as pd
from scipy import stats
import seaborn as sns
from prettytable import PrettyTable
from prettytable import MARKDOWN
from prettytable import MSWORD_FRIENDLY
import re

config = {
    "sequential": "NP-00",
    "filenames": "SIZE-[0-9]+-NP-[0-9]+-OPT*",
    "cols": {
        'read_time': {
            'plot': False,
            'computeSpeedup': False
        },
        'tarjan_time': {
            'plot': False,
            'computeSpeedup': False
        },
        'communication_time': {
            'plot': False,
            'computeSpeedup': False,
            "skipForFile": 'SIZE-[0-9]+-NP-00-*'
        },
        'aggregation_time': {
            'plot': False,
            'computeSpeedup': False,
            "skipForFile": 'SIZE-[0-9]+-NP-00-*'
        },
        'write_time': {
            'plot': False,
            'computeSpeedup': False
        },
        'global_time': {
            'plot': False,
            'computeSpeedup': True
        }
    },

    "table": {
        "header": ['Version', 'Processes', 'ReadFromFile', 'Tarjan', 'communication', 'Aggregation', 'WriteToFile', 'Elapsed', 'Speedup', 'Efficiency']
    },

    "plot": {
        "x_from_table": "Processes",
        "y_from_table": "Speedup",
    },

    "calcComplExpr": ""
}


def _extract(path_to_folder, plot_columns):
    prev = os.getcwd()
    os.chdir(path_to_folder)
    filenames = [f for f in os.listdir('.') if os.path.isfile(f)]
    if not os.path.exists("plot"):
        os.mkdir("plot")

    filenames = [f for f in os.listdir('.') if f.endswith(
        ".csv") and re.match(config["filenames"], f)]
    print(filenames)
    filenames = sorted(filenames)
    means = {}

    for filename in filenames:
        file_mean = {}
        print('Processing: ' + filename)
        ds = pd.read_csv(filename)
        for col in plot_columns.keys():
            print('Processing: ' + filename + ", Col: " + col)
            if 'skipForFile' in plot_columns[col] and re.match(plot_columns[col]['skipForFile'], filename):
                # print('SKIPPED: ' filename + ", Col: " + col)
                print('SKIPPED: ' + filename + ", col: " + col)
                file_mean[col] = 0
                continue
            x_data = ds[col]
            mean, std = stats.norm.fit(x_data)  # media gaussiana
            np_mean = np.mean(x_data)  # normale media, usata per poce misure

            x_data = ds[(ds[col] < (mean + std)) &
                        (ds[col] > (mean - std))][col]
            mean, std = stats.norm.fit(x_data)
            file_mean[col] = mean if np_mean == mean else np_mean

            if plot_columns[col]['plot']:
                sns.histplot(x_data, kde=True)
                plt.savefig("plot/" + str(col) + "_" +
                            filename.split('.')[0] + ".jpg")
                plt.close()

        means[filename] = file_mean
    os.chdir(prev)
    return means


def _compute_speedup(t, tp, nt, psize):
    speedup = t/tp
    efficiency = t/(tp*float(nt))
    return speedup, efficiency


def _make_table(header, rows, print_table=False, save=True, name=""):
    if save and not name:
        raise Exception("No filename to save file")
    x = PrettyTable()
    x.field_names = header
    x.add_rows(rows)
    if save:
        _save_table(x, name)
    if print_table:
        print(x)
    return x


def _save_table(table, filename):
    with open(filename, "w") as table_file:
        # table.set_style(MARKDOWN)
        table.set_style(MSWORD_FRIENDLY)
        data = table.get_string()
        table_file.write(data)


def _plot_from_table(header, rows, save=True, name="", show_plot=False):
    x = [0]
    y = [0]

    try:
        x_processes = config["plot"]["x_from_table"]
        y_speedup = config["plot"]["y_from_table"]
        speedup_data = config["table"]["header"].index(y_speedup)
        processes_data = config["table"]["header"].index(x_processes)
    except Exception as e:
        print("Config table or plot error")

    for row in rows[1:]:
        x.append(row[processes_data])
        y.append(row[speedup_data])

    x_pr = np.array(x)
    fig, axis = plt.subplots(figsize=(12, 8))
    axis.plot(x_pr, y, 'ro-', label='Experimental')
    axis.plot(x_pr, x_pr, color='blue', label='Ideal')

    plt.style.use('seaborn-v0_8-darkgrid')

    plt.autoscale(enable=True, axis='x', tight=True)
    plt.autoscale(enable=True, axis='y', tight=True)

    plt.legend()
    plt.xlabel(x_processes)
    plt.ylabel(y_speedup)
    if show_plot:
        plt.show()
    if save:
        plt.savefig(name)
    plt.close()


def extract():
    root = os.path.dirname(os.path.realpath(__file__))
    cols = config["cols"]
    process = [0, 1, 2, 3, 4, 8]
    print("Listing folder for problem size")
    folders = [f for f in os.listdir(root) if (
        os.path.isdir(os.path.join(root, f)))]
    print(f"Found folders : {folders}")
    for size_dir in folders:
        print(f"Problem size: {size_dir}")
        path_size = os.path.join(root, size_dir)
        opt_folders = [f for f in os.listdir(path_size) if (
            os.path.isdir(os.path.join(path_size, f)))]
        for opt_folder in opt_folders:
            print(f"Optimization : {opt_folder}")
            files_path = os.path.join(path_size, opt_folder)
            means = _extract(files_path, cols)
            header = {'values': config["table"]["header"]}
            cells = {'values': []}
            nt = -1
            for filename_key in means:
                cell = []
                splitted_filename = filename_key.split("-")
                if config["sequential"] in filename_key:
                    seq = means[filename_key]['global_time']
                    nt = 1
                    cell.append('Serial')
                    cell.append(nt)
                else:
                    nt = int(splitted_filename[3])
                    cell.append('Parallel')
                    cell.append(nt)

                for col in cols:
                    cell.append(means[filename_key][col])
                    if cols[col]['computeSpeedup']:
                        psize = splitted_filename[1]
                        speedup, efficiency = _compute_speedup(
                            seq, means[filename_key][col], nt, psize)
                        cell.append(speedup)
                        cell.append(efficiency)
                cells['values'].append(cell)

            table_filename = files_path + "/finalTable-SIZE" + \
                splitted_filename[1] + splitted_filename[-1]
            table = _make_table(
                header['values'], cells['values'], name=table_filename)
            plot_filename = root + "/speedup-SIZE" + \
                splitted_filename[1] + \
                splitted_filename[-1].split('.')[0] + ".jpg"
            _plot_from_table(header['values'],
                             cells['values'], name=plot_filename)


if __name__ == "__main__":
    extract()
