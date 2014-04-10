#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import copy
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

# The list of languages
lang_list = ['python', 'cpp']

# The serialized file output path.
seed_file_path = './output/'


class Scenario(object):
    '''
    This class is only used to be as a parent class.
    '''

    _tool = None
    _repeat_time = 100

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

        final_time = -1
        se_time = 0

        for i in xrange(self._repeat_time):
            if self._tool == 'protobuf':
                se_time = self.se_protobuf()
            elif self._tool == 'thrift':
                se_time = self.se_thrift()
            else:
                se_time = self.se_json()

            if final_time == -1 or final_time > se_time:
                final_time = se_time
            else:
                continue

        self._data_dic['se_time'] = final_time


    def deserialization(self):
        '''
        Start Deserialization

        Arguments:
        None

        Return value:
        None
        '''

        final_time = -1
        de_time = 0

        for i in xrange(self._repeat_time):
            if self._tool == 'protobuf':
                de_time = self.de_protobuf()
            elif self._tool == 'thrift':
                de_time = self.de_thrift()
            else:
                de_time = self.de_json()

            if final_time == -1 or final_time > de_time:
                final_time = de_time
            else:
                continue

        self._data_dic['dese_time'] = final_time


    def se_protobuf(self):
        pass
    def se_thrift(self):
        pass
    def de_protobuf(self):
        pass
    def de_thrift(self):
        pass


    def se_json(self):
        '''
        Serialization using JSON.
        '''

        se_dict = dict()

        start_time = time.clock()

        se_dict['person'] = list()
        
        for person in self._data_dic['input_data']['person']:
            person_dict = dict()
            person_dict['name'] = person['name']
            person_dict['id'] = person['id']

            if person['email'] != '':
                person_dict['email'] = person['email']

            person_dict['phone'] = list()

            for phone in person['phone']:
                phone_dict = dict()
                phone_dict['number'] = phone['number']

                if phone['type'] == '':
                    phone_dict['type'] = 'HOME'
                else:
                    phone_dict['type'] = phone['type']

                person_dict['phone'].append(phone_dict)

            se_dict['person'].append(person_dict)

        json_obj = json.dumps(se_dict)

        end_time = time.clock()

        self._data_dic['seed_file_str'] = json_obj

        return end_time - start_time


    def de_json(self):
        '''
        Deserialization using JSON.
        '''

        start_time = time.clock()
        json.loads(self._data_dic['seed_file_str'])
        end_time = time.clock()

        return end_time - start_time

