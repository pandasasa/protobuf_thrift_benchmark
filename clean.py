#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

se_tool_list = ['protobuf', 'thrift', 'json']
lang_list = ['python', 'java', 'cpp']

seed_file_path = './output/'

rm_postfix_list = ['.pyc', '.pyo', '.pyd']

def clean_file(path, postfix):
    for f in os.listdir(path):
        if os.path.isdir(os.path.join(path, f)) \
                or f[-len(postfix):] != postfix:
            continue
        else:
            os.remove(os.path.join(path, f))

def walk_func(arg, dirname, fnames):
    for name in fnames:
        for rm in rm_postfix_list:
            if name[-len(rm):] == rm:
                os.remove(os.path.join(dirname, name))

if __name__ == '__main__':
    for lang in lang_list:
        for tool in se_tool_list:
            # Delete serialized files
            del_path = seed_file_path + lang + '/' + tool + '/'
            clean_file(del_path, '.serialized')

    os.path.walk('./', walk_func, None)

