#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pickle
import pprint
import re

class Analyser(object):
    '''
    Used to analyse the outputed pickle dictionary.
    '''

    _pickle_dict = None
    _output_path = None

    _key_list = ['se_time', 'dese_time', 'seed_file_size']


    def __init__(self, pickle_file_path, output_path):
        pickle_file = open(pickle_file_path, 'r')
        self._pickle_dict = pickle.load(pickle_file)
        pickle_file.close()

        self._output_path = output_path


    def default_write(self):
        '''
        Default writing behavior.
        '''
        
        for key in self._key_list:
            self.write_data(key)


    def write_data(self, key):
        '''
        Write result data to file for future analysing.
        '''

        fact = 1 if key == 'seed_file_size' else 10 ** 6

        for tool in self._pickle_dict:
            tool_dict = self._pickle_dict[tool]

            x_set = set()
            y_set = set()

            for file_name in tool_dict:
                x_set.add(int(file_name.split('.')[0]))
                y_set.add(int(file_name.split('.')[1]))

            x_list = list(x_set)
            y_list = list(y_set)
            x_list.sort()
            x_list.sort()

            out_path = self._output_path + '/' + tool + '.' + key + '.result'
            write_file = open(out_path, 'w')

            for x in x_list:
                for y in y_list:
                    file_name = '%d.%d.json' % (x, y)
                    file_dict = tool_dict[file_name]

                    value = file_dict[key] * fact
                    write_file.write(str(value) + '\t')

                write_file.write('\n')

            write_file.close()


    def print_dict(self):
        pprint.pprint(self._pickle_dict)


if __name__ == '__main__':
    pickle_file_path = './original_dict.pickle'
    output_path = './'

    analyser = Analyser(pickle_file_path, output_path)
    analyser.default_write()

    analyser.print_dict()

