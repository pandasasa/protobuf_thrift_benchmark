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

    data_generator.del_org('./data/', '.json')
    gen = data_generator.DataGenerator(template_path, output_path, config_dict)
    gen.gen()


if __name__ == '__main__':
    # The filename postfix of the files to be remove.
    rm_postfix_list = ['.pyc', '.pyo', '.pyd', '.serialized']

    # Cleaning the workspace
    print 'Cleaning Workspace'
    clean_workspace(rm_postfix_list)

    # Testing Data Generating
    print 'Generating Testing Data'
    template_path = './lib/template/address_book.json'
    output_path = './data/'

    config_file = open('./gen.config', 'r')
    parsing_result = data_generator.parse_config(config_file)
    config_file.close()

    data_generating(template_path, output_path, parsing_result)