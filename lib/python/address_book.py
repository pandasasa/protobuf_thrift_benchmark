#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import time

import basic_lib

import address_book_protobuf_pb2
from BenchmarkThrift import ttypes

from thrift.transport import TTransport

from thrift.protocol import TCompactProtocol
from thrift.protocol import TBinaryProtocol


class AddressBook(basic_lib.Scenario):
    '''
    Address Book Class
    '''

    def __init__(self, tool, data_info_dict):
        '''
        Initializing Object
        '''

        super(AddressBook, self).__init__(tool, data_info_dict)
        self._data_dic = data_info_dict


    def se_protobuf(self):
        '''
        Serialization using protobuf.
        '''

        address_book = address_book_protobuf_pb2.AddressBook()

        start_time = time.clock()

        for person in self._data_dic['input_data']['person']:
            one_person = address_book.person.add()
            
            one_person.name = person['name']
            
            one_person.id = person['id']
            
            if person['email'] != '':
                one_person.email = person['email']
            
            for phone in person['phone']:
                one_phone = one_person.phone.add()
                
                one_phone.number = phone['number']
                
                if phone['type'] == '' or phone['type'] == 'HOME':
                    one_phone.type = address_book_protobuf_pb2.Person.HOME
                elif phone['type'] == 'MOBILE':
                    one_phone.type = address_book_protobuf_pb2.Person.MOBILE
                elif phone['type'] == 'WORK':
                    one_phone.type = address_book_protobuf_pb2.Person.WORK
                else:
                    one_phone.type = address_book_protobuf_pb2.Person.OTHER

        end_time = time.clock()

        self._data_dic['seed_file_str'] = address_book.SerializeToString()

        return end_time - start_time


    def de_protobuf(self):
        '''
        Deserialization using protobuf.
        '''

        address_book = address_book_protobuf_pb2.AddressBook()

        start_time = time.clock()
        address_book.ParseFromString(self._data_dic['seed_file_str'])
        end_time = time.clock()

        return end_time - start_time


    def se_thrift_compact(self):
        '''
        Serialization using thrift compact protocol.
        '''

        address_book = ttypes.AddressBook()
        address_book.person = []

        start_time = time.clock()

        mem_buf = TTransport.TMemoryBuffer()
        bin_proto = TCompactProtocol.TCompactProtocol(mem_buf)

        for person in self._data_dic['input_data']['person']:
            one_person = ttypes.Person()
            
            one_person.name = person['name']
            
            one_person.id = person['id']
            
            if person['email'] != '':
                one_person.email = person['email']
            
            one_person.phone = []
            for phone in person['phone']:
                one_phone = ttypes.PhoneNumber()
                
                one_phone.number = phone['number']
                
                if phone['type'] == '' or phone['type'] == 'HOME':
                    one_phone.type = ttypes.PhoneType.HOME
                elif phone['type'] == 'MOBILE':
                    one_phone.type = ttypes.PhoneType.MOBILE
                elif phone['type'] == 'WORK':
                    one_phone.type = ttypes.PhoneType.WORK
                else:
                    one_phone.type = ttypes.PhoneType.OTHER
                
                one_person.phone.append(one_phone)
            
            address_book.person.append(one_person)
        
        address_book.write(bin_proto)

        end_time = time.clock()

        self._data_dic['seed_file_str'] = mem_buf.getvalue()

        return end_time - start_time


    def de_thrift_compact(self):
        '''
        Deserialization using thrift compact protocol.
        '''

        address_book = ttypes.AddressBook()

        start_time = time.clock()
        
        mem_buf = TTransport.TMemoryBuffer(self._data_dic['seed_file_str'])
        bin_proto = TCompactProtocol.TCompactProtocol(mem_buf)
        
        address_book.read(bin_proto)
        
        end_time = time.clock()

        return end_time - start_time

    def se_thrift_binary(self):
        '''
        Serialization using thrift binary protocol.
        '''

        address_book = ttypes.AddressBook()
        address_book.person = []

        start_time = time.clock()

        mem_buf = TTransport.TMemoryBuffer()
        bin_proto = TBinaryProtocol.TBinaryProtocol(mem_buf)

        for person in self._data_dic['input_data']['person']:
            one_person = ttypes.Person()
            
            one_person.name = person['name']
            
            one_person.id = person['id']
            
            if person['email'] != '':
                one_person.email = person['email']
            
            one_person.phone = []
            for phone in person['phone']:
                one_phone = ttypes.PhoneNumber()
                
                one_phone.number = phone['number']
                
                if phone['type'] == '' or phone['type'] == 'HOME':
                    one_phone.type = ttypes.PhoneType.HOME
                elif phone['type'] == 'MOBILE':
                    one_phone.type = ttypes.PhoneType.MOBILE
                elif phone['type'] == 'WORK':
                    one_phone.type = ttypes.PhoneType.WORK
                else:
                    one_phone.type = ttypes.PhoneType.OTHER
                
                one_person.phone.append(one_phone)
            
            address_book.person.append(one_person)
        
        address_book.write(bin_proto)

        end_time = time.clock()

        self._data_dic['seed_file_str'] = mem_buf.getvalue()

        return end_time - start_time


    def de_thrift_binary(self):
        '''
        Deserialization using thrift binary protocol.
        '''

        address_book = ttypes.AddressBook()

        start_time = time.clock()
        
        mem_buf = TTransport.TMemoryBuffer(self._data_dic['seed_file_str'])
        bin_proto = TBinaryProtocol.TBinaryProtocol(mem_buf)
        
        address_book.read(bin_proto)
        
        end_time = time.clock()

        return end_time - start_time

