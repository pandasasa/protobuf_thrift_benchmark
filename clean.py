#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os


def clean_workspace(clean_path, rm_postfix_list):
    '''
    Clean the workspace before running benchmark.
    '''

    def walk_func(arg, dirname, fnames):
        for name in fnames:
            for rm in rm_postfix_list:
                if name[-len(rm):] == rm:
                    os.remove(os.path.join(dirname, name))

    os.path.walk(clean_path, walk_func, None)


if __name__ == '__main__':
    # The filename postfix of the files to be remove.
    rm_postfix_list = ['.pyc', '.pyo', '.pyd', '.serialized',
            '.o', '.exe', '.result']
    rm_data_postfix_list = ['.json']
    curr_path = './'
    data_path = './data/'

    clean_workspace(curr_path, rm_postfix_list)
    clean_workspace(data_path, rm_data_postfix_list)
    print 'Done'
    print 'Top Directory.'
    os.system("ls -l --color=auto")
    print 'Testing Data Directory'
    os.system("ls -l --color=auto ./data/")

