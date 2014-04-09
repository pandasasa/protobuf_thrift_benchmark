#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import compileall

from data import data_generator
import clean


def data_generating(template_path, output_path, config_dict):
    '''
    Generating Test Data.
    '''

    print '\tCleaning Old Testing Data'
    data_generator.del_org('./data/', '.json')
    gen = data_generator.DataGenerator(template_path, output_path, config_dict)
    print '\tGenerating'
    gen.gen()


if __name__ == '__main__':
    # The filename postfix of the files to be remove.
    rm_postfix_list = ['.pyc', '.pyo', '.pyd', '.serialized', '.o', '.exe']

    # Cleaning the workspace
    print 'Cleaning Workspace'
    clean.clean_workspace(rm_postfix_list)

    # Testing Data Generating
    template_path = './lib/template/address_book_json.json'
    output_path = './data/'

    print 'Loading gen.config'
    config_file = open('./gen.config', 'r')
    parsing_result = data_generator.parse_config(config_file)
    config_file.close()

    print 'Testing Data Generating'
    data_generating(template_path, output_path, parsing_result)

    # Compiling Python file
    print 'Compiling Python Benchmark'
    compileall.compile_dir('./', quiet=True)

    # Compiling C++ file
    print 'Compiling C++ Benchmark'
    os.system('make')

    # Compiling Java file
    print 'Compiling Java Benchmark(NOT FINISHED YET)'

    # Running Python Benchmark
    print 'Running Python Benchmark'
    os.system('python ./benchmark.py')

    # Running C++ Benchmark
    print 'Running C++ Benchmark'
    os.system('./benchmark.exe')

    # Running Java Benchmark
    print 'Running Java Benchmark(NOT FINISHED YET)'

