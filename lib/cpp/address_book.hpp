#ifndef ADDRESS_BOOK_HPP
#define ADDRESS_BOOK_HPP

#include <iostream>
#include <ctime>

#include <boost/serialization/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>

#include "basic_lib.hpp"
#include "address_book_protobuf.pb.h"
#include "address_book_thrift_types.h"


namespace Benchmark {
    class AddressBook : public Scenario
    {
    public:
        AddressBook(const std::string &tool, KeyLevel &data_info_dict)
            : Scenario(tool, data_info_dict), tool(tool),
            data_dic(data_info_dict) {}

        long se_protobuf()
        {
            GOOGLE_PROTOBUF_VERIFY_VERSION;
            namespace bp = boost::property_tree;
            namespace BPro = BenchmarkProtobuf;

            timespec start_time, end_time;
            BPro::AddressBook address_book;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

            const boost::any &input_data = this->data_dic["input_data"];
            const bp::ptree &pt = boost::any_cast<bp::ptree>(input_data);

            for (bp::ptree::const_iterator person
                    = pt.get_child("person").begin();
                person != pt.get_child("person").end();
                ++person)
            {
                BPro::Person *one_person = address_book.add_person();

                one_person->set_name(person->second.get<std::string>("name"));
                
                one_person->set_id(person->second.get<int>("id"));
                
                if (person->second.get<std::string>("email") == "")
                    one_person->set_email(
                            person->second.get<std::string>("email"));
                
                for (bp::ptree::const_iterator phone
                        = person->second.get_child("phone").begin();
                    phone != person->second.get_child("phone").end();
                    ++phone)
                {
                    BPro::Person_PhoneNumber *one_phone
                        = one_person->add_phone();

                    one_phone->set_number(
                            phone->second.get<std::string>("number"));
                    
                    if (phone->second.get<std::string>("type") == "HOME"
                        || phone->second.get<std::string>("type") == "")
                        one_phone->set_type(BPro::Person::HOME);
                    else if (phone->second.get<std::string>("type")
                            == "MOBILE")
                        one_phone->set_type(BPro::Person::MOBILE);
                    else if (phone->second.get<std::string>("type") == "WORK")
                        one_phone->set_type(BPro::Person::WORK);
                    else
                        one_phone->set_type(BPro::Person::OTHER);
                }
            }
            
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            std::string seed_str;
            address_book.SerializeToString(&seed_str);
            this->data_dic["seed_file_str"] = seed_str;

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long de_protobuf()
        {
            GOOGLE_PROTOBUF_VERIFY_VERSION;
            timespec start_time, end_time;

            BenchmarkProtobuf::AddressBook address_book;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
            
            boost::any &str = this->data_dic["seed_file_str"];
            
            address_book.ParseFromString(boost::any_cast<std::string>(str));
            
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long se_thrift()
        {
            namespace bp = boost::property_tree;
            namespace BThr = BenchmarkThrift;
            namespace atp = apache::thrift::protocol;
            namespace att = apache::thrift::transport;

            timespec start_time, end_time;
            BThr::AddressBook address_book;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

            boost::shared_ptr<att::TMemoryBuffer>
                mem_buf(new att::TMemoryBuffer);
            atp::TCompactProtocol bin_proto(mem_buf);
            const boost::any &input_data = this->data_dic["input_data"];
            const bp::ptree &pt = boost::any_cast<bp::ptree>(input_data);

            for (bp::ptree::const_iterator person
                    = pt.get_child("person").begin();
                person != pt.get_child("person").end();
                ++person)
            {
                BThr::Person one_person;

                one_person.__set_name(person->second.get<std::string>("name"));
                
                one_person.__set_id(person->second.get<int>("id"));
                
                if (person->second.get<std::string>("email") == "")
                    one_person.__set_email(
                            person->second.get<std::string>("email"));
                
                for (bp::ptree::const_iterator phone
                        = person->second.get_child("phone").begin();
                    phone != person->second.get_child("phone").end();
                    ++phone)
                {
                    BThr::PhoneNumber one_phone;
                    
                    one_phone.__set_number(
                            phone->second.get<std::string>("number"));
                    
                    if (phone->second.get<std::string>("type") == "HOME"
                        || phone->second.get<std::string>("type") == "")
                        one_phone.__set_type(BThr::PhoneType::HOME);
                    else if (phone->second.get<std::string>("type") == "MOBILE")
                        one_phone.__set_type(BThr::PhoneType::MOBILE);
                    else if (phone->second.get<std::string>("type") == "WORK")
                        one_phone.__set_type(BThr::PhoneType::WORK);
                    else
                        one_phone.__set_type(BThr::PhoneType::OTHER);
                    
                    one_person.phone.push_back(one_phone);
                }
                
                address_book.person.push_back(one_person);
            }
            
            atp::TProtocol *bin_proto_ptr = &bin_proto;
            address_book.write(bin_proto_ptr);

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            this->data_dic["seed_file_str"] = mem_buf->getBufferAsString();

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long de_thrift()
        {
            BenchmarkThrift::AddressBook address_book;
            namespace atp = apache::thrift::protocol;
            namespace att = apache::thrift::transport;

            timespec start_time, end_time;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
            
            boost::any &str = this->data_dic["seed_file_str"];
            std::string std_str = boost::any_cast<std::string>(str);
            uint8_t *buf = new uint8_t [std_str.length()];
            memcpy(buf, std_str.c_str(), std_str.length());

            boost::shared_ptr<att::TMemoryBuffer>
                mem_buf(new att::TMemoryBuffer(buf, std_str.length()));
            atp::TCompactProtocol bin_proto(mem_buf);
            
            atp::TProtocol *bin_proto_ptr = &bin_proto;
            address_book.read(bin_proto_ptr);

            delete [] buf;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long se_json()
        {
            namespace bp = boost::property_tree;

            timespec start_time, end_time;
            bp::ptree se_dict;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

            bp::ptree person_list;
            const boost::any &input_data = this->data_dic["input_data"];
            const bp::ptree &pt = boost::any_cast<bp::ptree>(input_data);

            for (bp::ptree::const_iterator person
                    = pt.get_child("person").begin();
                person != pt.get_child("person").end();
                ++person)
            {
                bp::ptree person_dict;
                person_dict.put("name",
                        person->second.get<std::string>("name"));
                person_dict.put("id", person->second.get<int>("id"));

                if (person->second.get<std::string>("email") != "")
                    person_dict.put("email",
                            person->second.get<std::string>("email"));

                bp::ptree phone_list;

                for (bp::ptree::const_iterator phone
                        = person->second.get_child("phone").begin();
                    phone != person->second.get_child("phone").end();
                    ++phone)
                {
                    bp::ptree phone_dict;
                    phone_dict.put("number",
                            phone->second.get<std::string>("number"));
                    
                    if (phone->second.get<std::string>("type") == "")
                        phone_dict.put("type", "HOME");
                    else
                        phone_dict.put("type",
                                phone->second.get<std::string>("type"));

                    phone_list.push_back(make_pair("", phone_dict));
                }

                person_dict.put_child("phone", phone_list);
                person_list.push_back(make_pair("", person_dict));
            }

            se_dict.put_child("person", person_list);

            std::stringstream json_ss;
            bp::json_parser::write_json(json_ss, se_dict);
            std::string json_obj = json_ss.str();

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            this->data_dic["seed_file_str"] = json_obj;

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long de_json()
        {
            namespace bp = boost::property_tree;

            timespec start_time, end_time;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
            
            bp::ptree json_obj;
            boost::any &str = this->data_dic["seed_file_str"];
            std::stringstream json_ss(boost::any_cast<std::string>(str));
            
            bp::json_parser::read_json(json_ss, json_obj);
            
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            return end_time.tv_nsec - start_time.tv_nsec;
        }


    private:
        std::string tool;
        KeyLevel &data_dic;
    };
}

#endif

