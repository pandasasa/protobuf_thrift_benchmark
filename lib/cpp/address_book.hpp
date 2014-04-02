#include "basic_lib.hpp"


namespace Benchmark {
    class AddressBook : public Scenario
    {
    public:
        AddressBook(const string &tool, const KeyLevel &data_info_dict)
            : Scenario(tool, data_info_dict) {}

        double se_protobuf() {}
        double de_protobuf() {}
        double se_thrift() {}
        double de_thrift() {}
        double se_json() {}
        double de_json() {}
    };
}
