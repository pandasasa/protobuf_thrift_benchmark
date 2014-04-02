#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

from data import data_generator


def clean_workspace(rm_postfix_list):
    '''
    Clean the workspace before running benchmark.
    '''

    def walk_func(arg, dirname, fnames):
        for name in fnames:
            for rm in rm_postfix_list:
                if name[-len(rm):] == rm:
                    os.remove(os.path.join(dirname, name))

    os.path.walk('./', walk_func, None)


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
    rm_postfix_list = ['.pyc', '.pyo', '.pyd', '.serialized']

    # Cleaning the workspace
    print 'Cleaning Workspace'
    clean_workspace(rm_postfix_list)

    # Testing Data Generating
    template_path = './lib/template/address_book_json.json'
    output_path = './data/'

    print 'Loading Config'
    config_file = open('./gen.config', 'r')
    parsing_result = data_generator.parse_config(config_file)
    config_file.close()

    print 'Testing Data Generating'
    data_generating(template_path, output_path, parsing_result)