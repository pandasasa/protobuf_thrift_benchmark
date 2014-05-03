#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import time

import json
import pickle

from lib.python import *
from result.python import analyse


def test_go(data_dir, benchmark_dict):
    '''
    Initializing and running data
    Load testing data from data_dir, filling them into global dictionary.
    Then run the benchmark.
    
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

        print '    %s' % tool
        for file_name in file_list:
            benchmark_dict[tool][file_name] = dict()

            data_info = benchmark_dict[tool][file_name]

            data_file = open(data_dir + file_name, 'r')
            data_info['input_data'] = json.load(data_file)
            data_file.close()

            ins = address_book.AddressBook(tool, data_info)
            ins.serialization()

            output_path = './output/python/%s/%s.serialized' \
                    % (tool, file_name[:-5])
            output_file = open(output_path, 'w')
            output_file.write(data_info['seed_file_str'])
            output_file.close()

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
    print '  Running Benchmark'
    benchmark_dict = dict()
    test_go(basic_lib.input_data_dir, benchmark_dict)

    # Processing the result and generating statistic results
    print '  Saving Result to Pickle.'
    result_dict_output(basic_lib.result_file_path, benchmark_dict)

    # Outputing result to txt files
    print '  Outputing result to txt files.'

    pickle_file_path = './result/python/original_dict.pickle'
    output_path = './result/python/'

    ana = analyse.Analyser(pickle_file_path, output_path)
    ana.default_write()

