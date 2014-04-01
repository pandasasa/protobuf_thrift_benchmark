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

    _temp_path = None
    _temp_dict = None
    _data_box = None
    _output_path = None

    _x_init = 1
    _x_loop = 1000
    _y_loop = 1
    _z_loop = 1
    _x_step = 1
    _y_step = 1
    _z_step = 1

    def __init__(self, temp_path, output_path, config_dict):
        '''
        Init template information
        '''

        self._temp_path = temp_path

        temp_file = open(self._temp_path, 'r')
        self._temp_dict = json.load(temp_file)

        self._output_path = output_path

        if len(config_dict.keys()) != 0:
            self._x_init = config_dict['x_init']
            self._x_loop = config_dict['x_loop']
            self._y_loop = config_dict['y_loop']
            self._z_loop = config_dict['z_loop']
            self._x_step = config_dict['x_step']
            self._y_step = config_dict['y_step']
            self._z_step = config_dict['z_step']

        self._data_box = [[[]]]
        # _data_box will store all the generated data
        # and the data in box will output to the data file.
        # Structure: [x[y[z]]]

    def x_gen(self):
        '''
        Expend the length of single value in template.
        x means single value length.
        '''

        def str_gen(num):
            for i in xrange(num):
                yield 'g'

        def ext_dict(dic, length):
            for key in dic.keys():
                if type(dic[key]) is unicode:
                    dic[key] = ''.join(str_gen(self._x_init + length)) \
                            if dic[key] == '' else dic[key]
                elif type(dic[key]) is list:
                    for item in dic[key]:
                        ext_dict(item, length)
                elif type(dic[key]) is dict:
                    ext_dict(dic[key], length)
                else:
                    pass

        for length in xrange(1, self._x_loop + 1, self._x_step):
            obj_dict = copy.deepcopy(self._temp_dict)
            ext_dict(obj_dict, length)
            self._data_box[0][0].append(obj_dict)


    def y_gen(self):
        '''
        Expend the repeat value in template.
        y means repeat value.
        '''

        def ext_list(dic, repeat):
            for key in dic.keys():
                if type(dic[key]) is list:
                    ext_list(dic[key][0], repeat)
                    for i in range(repeat):
                        dic[key].append(copy.copy(dic[key][0]))
                elif type(dic[key]) is dict:
                    ext_list(dic[key], repeat)
                else:
                    pass

        for repeat in xrange(1, self._y_loop, self._y_step):
            obj_list = copy.deepcopy(self._data_box[0][0])
            for item in obj_list:
                ext_list(item, repeat)
            self._data_box[0].append(obj_list)

    def z_gen(self):
        '''
        Expend the number of keys in first level of template.
        z means the number of keys.
        '''

        for repeat in xrange(1, self._z_loop, self._z_step):
            obj_list = copy.deepcopy(self._data_box[0])
            for y in xrange(self._y_loop):
                for x in xrange(self._x_loop):
                    key_list = obj_list[y][x].keys()
                    for key in key_list:
                        for i in xrange(repeat):
                            obj_list[y][x][key + str(repeat)] = \
                                    copy.copy(obj_list[y][x][key])

            self._data_box.append(obj_list)

    def gen(self):
        '''
        Call the *_gen methods and generate data box.
        And output data box to file.
        '''
        
        print 'Generating single value.'
        self.x_gen()

        print 'Generating repeat value.'
        self.y_gen()

        print 'Generating new keys.'
        self.z_gen()

        print 'Output to file.'
        for z in xrange(self._z_loop):
            for y in xrange(self._y_loop):
                for x in xrange(self._x_loop):
                    file_name = '.'.join([str(x), str(y), str(z)]) + '.json'
                    data_file = open(self._output_path + file_name, 'w')
                    json.dump(self._data_box[z][y][x], data_file)
                    data_file.close()

def del_org(dir, postfix):
    for f in os.listdir(dir):
        if os.path.isdir(os.path.join(os.getcwd(), f)) \
                or f[-len(postfix):] != postfix:
            continue
        else:
            os.remove(os.path.join(dir, f))

if __name__ == '__main__':
    template_path = '../lib/template/address_book.json'
    output_path = './'
    config_dict = {}

    del_org('./', '.json')
    gen = DataGenerator(template_path, output_path, config_dict)

    gen.gen()

