#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os

import json
import random
import copy

class DataGenerator:
    '''
    Test Data Generator.
    '''

    _temp_dict = None
    _output_path = None
    _ignore_key_list = None

    _x_loop = 10
    _y_loop = 1
    _x_step = 1
    _y_step = 1

    def __init__(self, temp_path, output_path, config_dict):
        '''
        Init template information
        '''

        temp_file = open(temp_path, 'r')
        self._temp_dict = json.load(temp_file)
        temp_file.close()

        self._output_path = output_path
        self._ignore_key_list = ['type']


        if len(config_dict.keys()) != 0:
            self._x_loop = config_dict['x_loop']
            self._y_loop = config_dict['y_loop']
            self._x_step = config_dict['x_step']
            self._y_step = config_dict['y_step']

        if self._x_step > self._x_loop \
            or self._y_step > self._x_loop:
            print 'ERROR! STEP IS LARGER THAN LOOP NUM.'
            exit(1)


    def _x_ext(self, dic):
        '''
        Extend the x axis.
        '''

        for key in dic:
            if key in self._ignore_key_list:
                continue

            value_type = type(dic[key])
                
            if value_type is unicode:
                dic[key] += 'g'
            elif value_type is list:
                for item in dic[key]:
                    self._x_ext(item)
            elif value_type is dict:
                self._x_ext(dic[key])
            else:
                pass


    def _y_ext(self, dic):
        '''
        Extend the y axis.
        '''

        for key in dic:
            if key in self._ignore_key_list:
                continue

            value_type = type(dic[key])

            if value_type is list:
                item = None
                for item in dic[key]:
                    self._y_ext(item)
                if item is None:
                    dic[key].append(dict())
                else:
                    dic[key].append(item)
            elif value_type is dict:
                self._x_ext(dic[key])
            else:
                pass


    def fast_gen(self):
        '''
        New generator algorithms, save huge amount of memory.
        '''

        x_dict = copy.deepcopy(self._temp_dict)

        for x in xrange(0, self._x_loop, self._x_step):
            # Extend the x axis
            self._x_ext(x_dict)
            y_dict = copy.deepcopy(x_dict)
            
            for y in xrange(0, self._y_loop, self._y_step):
                # Extend the y axis
                self._y_ext(y_dict)
                
                file_name = '%d.%d.json' % (x, y)
                data_file = open(self._output_path + '/' + file_name, 'w')
                json.dump(y_dict, data_file)
                data_file.close()

            del y_dict
        del x_dict


    def gen(self):
        '''
        The gen method has been changed in new git branch, it change the 
        data generating algorithms to avoid overhead memory requirement
        in large data set situation.

        The gen method is no more recommanded, its implementation has also
        been replaced by the new fast_gen method.

        The obsolete description as below:
        Call the *_gen methods and generate data box.
        And output data box to file.
        '''
        self.fast_gen()


def del_org(dir, postfix):
    for f in os.listdir(dir):
        if os.path.isdir(os.path.join(os.getcwd(), f)) \
                or f[-len(postfix):] != postfix:
            continue
        else:
            os.remove(os.path.join(dir, f))


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
    template_path = '../lib/template/address_book_json.json'
    output_path = './'

    config_file = open('../gen.config', 'r')
    config_dict = parse_config(config_file)

    del_org('./', '.json')
    gen = DataGenerator(template_path, output_path, config_dict)
    gen.gen()
