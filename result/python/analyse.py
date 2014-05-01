#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pickle
import pprint

class Analyser(object):
    '''
    Used to analyse the outputed pickle dictionary.
    '''

    _pickle_dict = None
    _output_path = None
    _x = None
    _y = None
    _x_step = None
    _y_step = None

    _key_list = ['se_time', 'dese_time', 'seed_file_size']


    def __init__(self, pickle_file_path, config_file_path, output_path):
        pickle_file = open(pickle_file_path, 'r')
        self._pickle_dict = pickle.load(pickle_file)
        pickle_file.close()

        config_file = open(config_file_path, 'r')
        config_dict = self._parse_config(config_file)

        self._x = config_dict['x_loop']
        self._y = config_dict['y_loop']
        self._x_step = config_dict['x_step']
        self._y_step = config_dict['x_step']

        config_file.close()

        self._output_path = output_path


    def _parse_config(self, file_obj):
        '''
        Parsing the config file.
        '''

        comment_sign = '#'
        result = dict()

        for line in file_obj:
            # Ignore blank line and comments
            if len(re.findall(r'\S', line)) == 0 \
                or len(re.findall(r'^\s*%s.*$' % comment_sign, line)):
                continue
            if comment_sign in line:
                line = line[:line.index(comment_sign)]

            key_value = line.split('=')
            result[key_value[0]] = int(key_value[1])

        return result


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
            out_path = self._output_path + '/' + tool + '.' + key + '.result'

            write_file = open(out_path, 'w')

            for x in xrange(self._x):
                for y in xrange(self._y):
                    file_name = str(x) + '.' + str(y) + '.json'
                    file_dict = tool_dict[file_name]

                    value = file_dict[key] * fact
                    write_file.write(str(value) + '\t')

                write_file.write('\n')

            write_file.close()


    def print_dict(self):
        pprint.pprint(self._pickle_dict)


if __name__ == '__main__':
    pickle_file_path = './original_dict.pickle'
    config_file_path = '../../gen.config'
    output_path = './'

    analyser = Analyser(pickle_file_path, config_file_path, output_path)
    analyser.default_write()

    analyser.print_dict()

