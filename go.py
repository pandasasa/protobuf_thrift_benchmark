#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import compileall
import clean

from data import data_generator
from result.python import analyse


def data_generating(template_path, output_path, config_dict):
    '''
    Generating Test Data.
    '''

    print '  Cleaning Old Testing Data'
    data_generator.del_org('./data/', '.json')
    gen = data_generator.DataGenerator(template_path, output_path, config_dict)

    print '  Generating'
    gen.gen()

def print_title(title):
    split_line = '============================================================'
    print split_line
    print title


if __name__ == '__main__':
    # The filename postfix of the files to be remove.
    rm_postfix_list = ['.pyc', '.pyo', '.pyd', '.serialized', '.o', '.exe']
    curr_path = './'

    # Cleaning the workspace
    print_title('Cleaning Workspace')
    clean.clean_workspace(curr_path, rm_postfix_list)

    # Testing Data Generating
    template_path = './lib/template/address_book_json.json'
    output_path = './data/'

    print_title('Loading gen.config')
    config_file = open('./gen.config', 'r')
    parsing_result = data_generator.parse_config(config_file)
    config_file.close()

    print_title('Testing Data Generating')
    data_generating(template_path, output_path, parsing_result)

    # Compiling Python file
    print_title('Compiling Python Benchmark')
    compileall.compile_dir('./', quiet=True)

    # Compiling C++ file
    print_title('Compiling C++ Benchmark')
    os.system('make')

    # Running Python Benchmark
    print_title('Running Python Benchmark')
    os.system('python ./benchmark.py')

    # Running C++ Benchmark
    print_title('Running C++ Benchmark')
    os.system('./benchmark.exe')

