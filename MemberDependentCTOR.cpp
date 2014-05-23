#include <iostream>
#include <string>
#include <type_traits>

template<class Tag> class tag {};
class Type1Policy: std::true_type {};
class Type2Policy: std::false_type {};

template <class TypePolicy>
class MemberDependentCTOR {
public: /* CTORs, DTORs */
    explicit MemberDependentCTOR(): MemberDependentCTOR( tag<TypePolicy>{} ) {} // dispatch

    explicit MemberDependentCTOR( tag<Type1Policy> ): member_ { "foo" } {}
    explicit MemberDependentCTOR( tag<Type2Policy> ): member_ { 42 }    {}

    virtual ~MemberDependentCTOR() = default;


private: /* Members */
    using member_type = typename
        std::conditional<
            std::is_same<TypePolicy, Type1Policy>::value,   // if
            std::string,                                    // then
            int                                             // else
        >::type;
    member_type member_;

public: /* Methods */
    member_type get() const {
        return member_;
    }
};

auto main() -> int { // C++11
    MemberDependentCTOR<Type1Policy>   wrapped1;
    MemberDependentCTOR<Type2Policy>   wrapped2;

    std::cout << "wrapped1: " << wrapped1.get() << std::endl; // "foo"
    std::cout << "wrapped2: " << wrapped2.get() << std::endl; // 42

    return 0;
}