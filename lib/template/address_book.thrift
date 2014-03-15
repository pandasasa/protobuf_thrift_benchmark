namespace py AddressBook
namespace cpp AddressBook
namespace java AddressBook

enum PhoneType {
    MOBILE,
    HOME,
    WORK,
    OTHER
}

struct PhoneNumber {
    1: required string number;
    2: optional PhoneType type = PhoneType.HOME;
}

struct Person {
    1: required string name;
    2: required i32 id;
    3: optional string email;
    4: list<PhoneNumber> phone;
}

struct AddressBook {
    1: list<Person> person
}
