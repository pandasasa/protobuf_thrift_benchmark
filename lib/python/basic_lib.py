#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time

import json

# This file is used to set some global vaiables such as the dictionary keys.

# The data input path.
input_data_dir = './data/'

# A list of serialization tools name.
se_tool_list = ['protobuf', 'json', 'thrift']

# A list of serialization related keys of dictionary.
se_data_key_list = ['input_file_path', 'input_data', 'se_time', \
                    'seed_file_str', 'seed_file_path', 'seed_file_size', \
                    'dese_time']

# The dictionary's pickle file path.
result_file_path = './result/python/original_dict.pickle'

class Scenario(object):
    '''
    This class is only used to be as a parent class.
    '''

    _tool = None

    def __init__(self, tool, data_info_dict):
        '''
        Initializing Object
        '''

        self._tool = tool
        self._data_dic = data_info_dict


    def serialization(self):
        '''
        Start Serialization
        
        Argumrnts:
        None
        
        Return value:
        None
        '''


        if self._tool == 'protobuf':
            self.se_protobuf()
        elif self._tool == 'thrift':
            self.se_thrift()
        else:
            self.se_json()


    def deserialization(self):
        '''
        Start Deserialization

        Arguments:
        None

        Return value:
        None
        '''

        if self._tool == 'protobuf':
            self.de_protobuf()
        elif self._tool == 'thrift':
            self.de_thrift()
        else:
            self.de_json()


    def se_protobuf(self):
        pass
    def se_thrift(self):
        pass


    def se_json(self):
        '''
        Serialization using JSON.
        '''

        start_time = time.clock()
        json_obj = json.dumps(self._data_dic['input_data'])
        end_time = time.clock()
        self._data_dic['se_time'] = end_time - start_time
        self._data_dic['seed_file_str'] = json_obj


    def de_protobuf(self):
        pass
    def de_thrift(self):
        pass


    def de_json(self):
        '''
        Deserialization using JSON.
        '''

        start_time = time.clock()
        json.loads(self._data_dic['seed_file_str'])
        end_time = time.clock()
        self._data_dic['dese_time'] = end_time - start_time

