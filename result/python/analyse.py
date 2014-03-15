#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pickle
import pprint

class Analyser(object):
    '''
    Used to analyse the outputed pickle dictionary.
    '''

    _pickle_dict = None

    def __init__(self, pickle_file_path):
        pickle_file = open(pickle_file_path, 'r')
        self._pickle_dict = pickle.load(pickle_file)

    def analyse(self):
        pass

    def get_raw(self):
        pass

    def print_dict(self):
        pprint.pprint(self._pickle_dict)

if __name__ == '__main__':
    pickle_file_path = './original_dict.pickle'

    analyzer = Analyser(pickle_file_path)

    analyzer.print_dict()

