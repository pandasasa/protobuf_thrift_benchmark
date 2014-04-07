#include <time.h>

#include <string>
#include <boost/property_tree/ptree.hpp>

#include <thrift/transport/TBufferTransport.h>
#include <thrift/protocol/TCompactProtocol.h>

#include "basic_lib.hpp"
#include "address_book_protobuf.pb.h"
#include "address_book_thrift_types.h"


namespace Benchmark {
    class AddressBook : public Scenario
    {
    public:
        AddressBook(const string &tool, const KeyLevel &data_info_dict)
            : Scenario(tool, data_info_dict) {}

        long se_protobuf()
        {
            GOOGLE_PROTOBUF_VERIFY_VERSION;
            namespace bp = boost::property_tree;
            namespace BPro = BenchmarkProtobuf;

            timespec start_time, end_time;
            BPro::AddressBook address_book;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

            for (bp::iterator person = this->data_dic["input_data"].begin();
                person != this->data_dict["input_data"].end();
                ++person)
            {
                BPro::Person *one_person = address_book.add_person();

                one_person->set_name(person->second.get<std::string>("name"));
                
                one_person->set_id(person->second.get<int>("id"));
                
                if (person->second.get<std::string>("email") == "")
                    one_person->set_email(
                            person->second.get<std::string>("email"));
                
                for (bp::iterator phone
                        = person->second.get<bp::ptree>("phone").begin();
                    phone != person->second.get<bp::ptree>("phone").end();
                    ++phone)
                {
                    BPro::Person_PhoneNumber *one_phone
                        = one_person->add_phone();

                    one_phone->set_number(
                            phone->second.get<std::string>("number"));
                    
                    if (phone->second.get<std::string>("type") == "HOME"
                        || phone->second.get<std::string>("type") == "")
                        one_phone->set_type(BPro::Person::HOME);
                    else if (phone->second.get<std::string>("type") == "MOBILE")
                        one_phone->set_type(BPro::Person::MOBILE);
                    else if (phone->second.get<std::string>("type") == "WORK")
                        one_phone->set_type(BPro::Person::WORK);
                    else
                        one_phone->set_type(BPro::Person::OTHER);
                }
            }
            
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            this->data_dic["seed_file_str"] = address_book.SerializeToString();

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long de_protobuf()
        {
            GOOGLE_PROTOBUF_VERIFY_VERSION;
            timespec start_time, end_time;

            BenchmarkProtobuf::AddressBook address_book;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
            address_book.ParseFromString(this->data_dic["seed_file_str"]);
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

            att::TMemoryBuffer mem_buf;
            atp::TCompactProtocol bin_proto(mem_buf);

            for (bp::iterator person = this->data_dic["input_data"].begin();
                person != this->data_dict["input_data"].end();
                ++person)
            {
                BThr::Person one_person;

                one_person.__set_name(person->second.get<std::string>("name"));
                
                one_person.set_id(person->second.get<int>("id"));
                
                if (person->second.get<std::string>("email") == "")
                    one_person->set_email(
                            person->second.get<std::string>("email"));
                
                for (bp::iterator phone
                        = person->second.get<bp::ptree>("phone").begin();
                    phone != person->second.get<bp::ptree>("phone").end();
                    ++phone)
                {
                    BThr::PhoneNumber one_phone;
                    
                    one_phone.__set_number(
                            phone->second.get<std::string>("number"));
                    
                    if (phone->second.get<std::string>("type") == "HOME"
                        || phone->second.get<std::string>("type") == "")
                        one_phone->__set_type(BThr::PhoneType::HOME);
                    else if (phone->second.get<std::string>("type") == "MOBILE")
                        one_phone->__set_type(BThr::PhoneType::MOBILE);
                    else if (phone->second.get<std::string>("type") == "WORK")
                        one_phone->__set_type(BThr::PhoneType::WORK);
                    else
                        one_phone->__set_type(BThr::PhoneType::OTHER);
                    
                    one_person.phone.push_back(one_phone);
                }
                
                address_book.person.push_back(one_person);
            }
            
            address_book.write(bin_proto);

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            this->data_dic["seed_file_str"] = mem_buf.getvalue();

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long de_thrift()
        {
            BenchmarkThrift::AddressBook address_book;
            namespace atp = apache::thrift::protocol;
            namespace att = apache::thrift::transport;

            timespec start_time, end_time;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
            att::TMemoryBuffer mem_buf(this->data_dic["seed_file_str"]);
            atp::TCompactProtocol bin_proto(mem_buf);
            address_book.read(bin_proto);
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long se_json()
        {
            namespace bp = boost::property_tree;

            bp::ptree se_dict;

            timespec start_time, end_time;

            for (bp::iterator person = this->data_dic["input_data"].begin();
                person != this->data_dict["input_data"].end();
                ++person)
            {
                BThr::Person one_person;

                one_person.__set_name(person->second.get<std::string>("name"));
                
                one_person.set_id(person->second.get<int>("id"));
                
                if (person->second.get<std::string>("email") == "")
                    one_person->set_email(
                            person->second.get<std::string>("email"));
                
                for (bp::iterator phone
                        = person->second.get<bp::ptree>("phone").begin();
                    phone != person->second.get<bp::ptree>("phone").end();
                    ++phone)
                {
                    BThr::PhoneNumber one_phone;
                    
                    one_phone.__set_number(
                            phone->second.get<std::string>("number"));
                    
                    if (phone->second.get<std::string>("type") == "HOME"
                        || phone->second.get<std::string>("type") == "")
                        one_phone->__set_type(BThr::PhoneType::HOME);
                    else if (phone->second.get<std::string>("type") == "MOBILE")
                        one_phone->__set_type(BThr::PhoneType::MOBILE);
                    else if (phone->second.get<std::string>("type") == "WORK")
                        one_phone->__set_type(BThr::PhoneType::WORK);
                    else
                        one_phone->__set_type(BThr::PhoneType::OTHER);
                    
                    one_person.phone.push_back(one_phone);
                }
                
                address_book.person.push_back(one_person);
            }
            
            address_book.write(bin_proto);

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

        }


        long de_json() {}
    };
}

