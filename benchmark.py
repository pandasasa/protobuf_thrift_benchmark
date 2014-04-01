#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import sys
import time

import json
import pickle

from lib.python import *
from data import data_generator


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

    for tool in basic_lib.se_tool_list:
        benchmark_dict[tool] = dict()

        file_list = filter(lambda x : not os.path.isdir(x), \
            os.listdir(data_dir))
        file_list = filter(lambda x : x[-5:] == '.json', file_list)

        if len(file_list) == 0:
            sys.stderr.write('Testing Data MISSED in dir ' + data_dir)
            exit(1)

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

                    data_obj = json.loads(json_str)
                    data_file_info_dict[data_key] = data_obj
                else:
                    data_file_info_dict[data_key] = None

    return benchmark_dict

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
                    + '.'.join(file_name.split('.')[:-1]) + '.serialized'
            output_file = open(output_path, 'w')
            output_file.write(data_info['seed_file_str'])
            output_file.close()

            data_info['seed_file_path'] = output_path
            data_info['seed_file_size'] = os.path.getsize(output_path)

            ins.deserialization()

            # Clear the original string and serialized string
            # for reducing the size of pickle file.
            del data_dict[tool][file_name]['input_data']
            del data_dict[tool][file_name]['seed_file_str']


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


def clean_workspace():
    '''
    Clean the workspace before running benchmark.
    '''

    def walk_func(arg, dirname, fnames):
        for name in fnames:
            for rm in basic_lib.rm_postfix_list:
                if name[-len(rm):] == rm:
                    os.remove(os.path.join(dirname, name))

    os.path.walk('./', walk_func, None)


def data_generating(template_path, output_path, config_dict):
    '''
    Generating Test Data.
    '''

    data_generator.del_org('./data/', '.json')
    gen = data_generator.DataGenerator(template_path, output_path, config_dict)
    gen.gen()


def parse_config(file_obj):
    '''
    Parsing the config file.
    '''

    result = {}

    for line in file_obj:
        key_value = line.split('=')
        result[key_value[0]] = int(key_value[1])

    return result


if __name__ == '__main__':
    # Cleaning workspace
    print 'Cleaning Workspace'
    clean_workspace()

    # Testing Data Generating
    print 'Generating Test Data'
    template_path = './lib/template/address_book.json'
    output_path = './data/'

    config_file = open('./gen.config', 'r')
    parsing_result = parse_config(config_file)
    config_file.close()

    data_generating(template_path, output_path, parsing_result)


    # Initializing information dictionary by data in path data_dir
    benchmark_dict = dict()
    print 'Init. Data'
    init_data(basic_lib.input_data_dir, benchmark_dict)

    # Running Benchmark
    print 'Running Benchmark'
    test_go(benchmark_dict)

    # Processing the result and generating statistic results
    print 'Saving Result to Pickle.'
    result_dict_output(basic_lib.result_file_path, benchmark_dict)

