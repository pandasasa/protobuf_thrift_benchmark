#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os


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


if __name__ == '__main__':
    # The filename postfix of the files to be remove.
    rm_postfix_list = ['.pyc', '.pyo', '.pyd', '.serialized', '.o', '.exe']

    clean_workspace(rm_postfix_list)
    print 'DONE!'
    os.system("ls -l --color=auto");

