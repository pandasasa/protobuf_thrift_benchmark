#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import time

import json
import pickle

from lib.python import *


def init_data(data_dir, benchmark_dict):
    '''
    Initialization Data
    Load testing data from data_dir, filling them into global dictionary.
    
    Arguments:
    data_dir: Direction of testing data.
    benchmark_dict: Dictionary to be initialized.

    Return value:
    The initialized dictionary itself.
    '''

    file_list = filter(lambda x : not os.path.isdir(x), \
        os.listdir(data_dir))
    file_list = filter(lambda x : x[-5:] == '.json', file_list)

    if len(file_list) == 0:
        sys.stderr.write('Testing Data MISSED in dir ' + data_dir)
        exit(1)

    for tool in basic_lib.se_tool_list:
        benchmark_dict[tool] = dict()
        for file_name in file_list:
            benchmark_dict[tool][file_name] = dict()

            data_file_info_dict = benchmark_dict[tool][file_name]
            for data_key in basic_lib.se_data_key_list:
                if data_key == 'input_file_path':
                    data_file_info_dict[data_key] = data_dir + file_name
                elif data_key == 'input_data':
                    data = open(data_file_info_dict['input_file_path'], 'r')
                    json_str = data.read()
                    data.close()

                    data_file_info_dict[data_key] = json.loads(json_str)
                else:
                    data_file_info_dict[data_key] = None


def test_go(data_dict):
    '''
    Running Test
    Begin to run the test.

    Arguments:
    data_dict: The initialized dictionary.

    Return value:
    None
    '''

    for tool in data_dict.keys():
        print 'Tool: ' + tool
        for file_name in data_dict[tool].keys():
            data_info = data_dict[tool][file_name]

            ins = address_book.AddressBook(tool, data_info)
            ins.serialization()

            output_path = './output/python/' + tool + '/' \
                    + file_name[:-5] + '.serialized'
            output_file = open(output_path, 'w')
            output_file.write(data_info['seed_file_str'])
            output_file.close()

            data_info['seed_file_path'] = output_path
            data_info['seed_file_size'] = len(data_info['seed_file_str'])

            ins.deserialization()

            # Clear the original string and serialized string
            # for reducing the size of pickle file.
            del data_info['input_data']
            del data_info['seed_file_str']


def result_dict_output(result_file_path, result_dict):
    '''
    Output Result Dictionary
    The filled result dicationary will be dumped to a file using pickle for
    the statistic.

    Arguments:
    result_file_path: The outputed file's path.
    result_dict: The result dictionary.

    Return value:
    None
    '''

    result_file = open(result_file_path, 'w')
    pickle.dump(result_dict, result_file)
    result_file.close()


if __name__ == '__main__':
    # Initializing information dictionary by data in path data_dir
    print 'Init. Data'
    benchmark_dict = dict()
    init_data(basic_lib.input_data_dir, benchmark_dict)

    # Running Benchmark
    print 'Running Benchmark'
    test_go(benchmark_dict)

    # Processing the result and generating statistic results
    print 'Saving Result to Pickle.'
    result_dict_output(basic_lib.result_file_path, benchmark_dict)
