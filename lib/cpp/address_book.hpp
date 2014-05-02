#ifndef ADDRESS_BOOK_HPP
#define ADDRESS_BOOK_HPP

#include <ctime>

#include <boost/serialization/string.hpp>
#include <jsoncpp/json/json.h>

#include <thrift/transport/TBufferTransports.h>
#include <thrift/protocol/TCompactProtocol.h>
#include <thrift/protocol/TBinaryProtocol.h>

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

        const std::string &get_seed_str() const
            { return this->local_seed_str; }

        long se_protobuf()
        {
            GOOGLE_PROTOBUF_VERIFY_VERSION;
            namespace BPro = BenchmarkProtobuf;

            timespec start_time, end_time;
            BPro::AddressBook address_book;
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

            const Json::Value &input_data = this->data_dic["input_data"];

            for (int person_index = 0;
                person_index < input_data["person"].size();
                person_index++)
            {
                BPro::Person *one_person = address_book.add_person();
                const Json::Value &person = input_data["person"][person_index];

                one_person->set_name(person["name"].asString());
                
                one_person->set_id(person["id"].asInt());
                
                if (person["email"].asString() != "")
                    one_person->set_email(person["email"].asString());
                
                for (int phone_index = 0;
                    phone_index < person["phone"].size();
                    phone_index++)
                {
                    BPro::Person_PhoneNumber *one_phone
                        = one_person->add_phone();
                    const Json::Value &phone = person["phone"][phone_index];

                    one_phone->set_number(phone["number"].asString());
                    
                    if (phone["type"].asString() == "HOME"
                        || phone["type"].asString() == "")
                        one_phone->set_type(BPro::Person::HOME);
                    else if (phone["type"].asString() == "MOBILE")
                        one_phone->set_type(BPro::Person::MOBILE);
                    else if (phone["type"].asString() == "WORK")
                        one_phone->set_type(BPro::Person::WORK);
                    else
                        one_phone->set_type(BPro::Person::OTHER);
                }
            }

            address_book.SerializeToString(&(this->local_seed_str));
            
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            this->data_dic["seed_file_size"] = Json::Value();
            int size = this->local_seed_str.length();
            this->data_dic["seed_file_size"]["value"] = size;

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long de_protobuf()
        {
            GOOGLE_PROTOBUF_VERIFY_VERSION;
            timespec start_time, end_time;

            BenchmarkProtobuf::AddressBook address_book;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
            
            address_book.ParseFromString(this->local_seed_str);
            
            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long se_thrift_compact()
        {
            namespace BThr = BenchmarkThrift;
            namespace atp = apache::thrift::protocol;
            namespace att = apache::thrift::transport;

            timespec start_time, end_time;
            BThr::AddressBook address_book;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

            boost::shared_ptr<att::TMemoryBuffer>
                mem_buf(new att::TMemoryBuffer);
            atp::TCompactProtocol bin_proto(mem_buf);
            const Json::Value &input_data = this->data_dic["input_data"];

            for (int person_index = 0;
                person_index < input_data["person"].size();
                person_index++)
            {
                BThr::Person one_person;
                const Json::Value &person = input_data["person"][person_index];

                one_person.__set_name(person["name"].asString());
                
                one_person.__set_id(person["id"].asInt());
                
                if (person["email"].asString() != "")
                    one_person.__set_email(person["email"].asString());
                
                for (int phone_index = 0;
                    phone_index < person["phone"].size();
                    phone_index++)
                {
                    BThr::PhoneNumber one_phone;
                    const Json::Value &phone = person["phone"][phone_index];
                    
                    one_phone.__set_number(phone["number"].asString());
                    
                    if (phone["type"].asString() == "HOME"
                        || phone["type"].asString() == "")
                        one_phone.__set_type(BThr::PhoneType::HOME);
                    else if (phone["type"].asString() == "MOBILE")
                        one_phone.__set_type(BThr::PhoneType::MOBILE);
                    else if (phone["type"].asString() == "WORK")
                        one_phone.__set_type(BThr::PhoneType::WORK);
                    else
                        one_phone.__set_type(BThr::PhoneType::OTHER);
                    
                    one_person.phone.push_back(one_phone);
                }
                
                address_book.person.push_back(one_person);
            }
            
            atp::TProtocol *bin_proto_ptr = &bin_proto;
            address_book.write(bin_proto_ptr);

            this->local_seed_str = mem_buf->getBufferAsString();

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            this->data_dic["seed_file_size"] = Json::Value();
            int size = this->local_seed_str.length();
            this->data_dic["seed_file_size"]["value"] = size;

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long de_thrift_compact()
        {
            BenchmarkThrift::AddressBook address_book;
            namespace atp = apache::thrift::protocol;
            namespace att = apache::thrift::transport;

            timespec start_time, end_time;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
            
            const std::string &std_str = this->local_seed_str;
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


        long se_thrift_binary()
        {
            namespace BThr = BenchmarkThrift;
            namespace atp = apache::thrift::protocol;
            namespace att = apache::thrift::transport;

            timespec start_time, end_time;
            BThr::AddressBook address_book;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

            boost::shared_ptr<att::TMemoryBuffer>
                mem_buf(new att::TMemoryBuffer);
            atp::TBinaryProtocol bin_proto(mem_buf);
            const Json::Value &input_data = this->data_dic["input_data"];

            for (int person_index = 0;
                person_index < input_data["person"].size();
                person_index++)
            {
                BThr::Person one_person;
                const Json::Value &person = input_data["person"][person_index];

                one_person.__set_name(person["name"].asString());
                
                one_person.__set_id(person["id"].asInt());
                
                if (person["email"].asString() != "")
                    one_person.__set_email(person["email"].asString());
                
                for (int phone_index = 0;
                    phone_index < person["phone"].size();
                    phone_index++)
                {
                    BThr::PhoneNumber one_phone;
                    const Json::Value &phone = person["phone"][phone_index];
                    
                    one_phone.__set_number(phone["number"].asString());
                    
                    if (phone["type"].asString() == "HOME"
                            || phone["type"].asString() == "")
                        one_phone.__set_type(BThr::PhoneType::HOME);
                    else if (phone["type"].asString() == "MOBILE")
                        one_phone.__set_type(BThr::PhoneType::MOBILE);
                    else if (phone["type"].asString() == "WORK")
                        one_phone.__set_type(BThr::PhoneType::WORK);
                    else
                        one_phone.__set_type(BThr::PhoneType::OTHER);
                    
                    one_person.phone.push_back(one_phone);
                }
                
                address_book.person.push_back(one_person);
            }
            
            atp::TProtocol *bin_proto_ptr = &bin_proto;
            address_book.write(bin_proto_ptr);

            this->local_seed_str = mem_buf->getBufferAsString();

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            this->data_dic["seed_file_size"] = Json::Value();
            int size = this->local_seed_str.length();
            this->data_dic["seed_file_size"]["value"] = size;

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long de_thrift_binary()
        {
            BenchmarkThrift::AddressBook address_book;
            namespace atp = apache::thrift::protocol;
            namespace att = apache::thrift::transport;

            timespec start_time, end_time;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);
            
            const std::string &std_str = this->local_seed_str;
            uint8_t *buf = new uint8_t [std_str.length()];
            memcpy(buf, std_str.c_str(), std_str.length());

            boost::shared_ptr<att::TMemoryBuffer>
                mem_buf(new att::TMemoryBuffer(buf, std_str.length()));
            atp::TBinaryProtocol bin_proto(mem_buf);
            
            atp::TProtocol *bin_proto_ptr = &bin_proto;
            address_book.read(bin_proto_ptr);

            delete [] buf;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long se_json()
        {
            timespec start_time, end_time;
            Json::Value se_dict;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

            Json::Value person_list;
            const Json::Value &input_data = this->data_dic["input_data"];

            for (int person_index = 0;
                person_index < input_data["person"].size();
                person_index++)
            {
                Json::Value person_dict;
                const Json::Value &person = input_data["person"][person_index];

                person_dict["name"] = person["name"].asString();
                person_dict["id"] = person["id"].asInt();

                if (person["email"].asString() != "")
                    person_dict["email"] = person["email"].asString();

                Json::Value phone_list;

                for (int phone_index = 0;
                    phone_index < person["phone"].size();
                    phone_index++)
                {
                    Json::Value phone_dict;
                    const Json::Value &phone = person["phone"][phone_index];

                    phone_dict["number"] = phone["number"].asString();

                    if (phone["type"].asString() == "")
                        phone_dict["type"] = "HOME";
                    else
                        phone_dict["type"] = phone["type"].asString();

                    phone_list.append(phone_dict);
                }

                person_dict["phone"] = phone_list;
                person_list.append(person_dict);
            }

            se_dict["person"] = person_list;

            Json::FastWriter writer;
            std::string json_obj = this->writer.write(se_dict);

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            this->data_dic["seed_file_str"] = Json::Value();
            this->data_dic["seed_file_str"]["value"] = json_obj;
            this->local_seed_str = json_obj;

            return end_time.tv_nsec - start_time.tv_nsec;
        }


        long de_json()
        {
            timespec start_time, end_time;

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start_time);

            Json::Reader reader;
            Json::Value root;
            const std::string &json_str
                = this->data_dic["seed_file_str"]["value"].asString();

            reader.parse(json_str, root, false);

            clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end_time);

            return end_time.tv_nsec - start_time.tv_nsec;
        }


    private:
        std::string tool;
        KeyLevel &data_dic;
        std::string local_seed_str;

        Json::FastWriter writer;
    };
}

#endif

