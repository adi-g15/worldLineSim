#include "world_node.hpp"
#include <iostream>

id_type _ID::_curr_ID = util::Random::random(100000);   // @note - Maybe temporarily, so that ids dont' just start with 0,1,2 and so on

class test
{
	World_Node* new_world;
public:
	void emulate() {
		new_world = new World_Node();
	}
};

int main() {
	std::thread(&test::emulate, test()).join();

	std::cin.ignore();
}

#if 0
#include <type_traits>

auto func2(char) -> int (*)()
{
    return nullptr;
}

int main()
{
    auto s= func2('c');
    //static_assert(std::is_invocable<int>::value, "Type should be invocable");
    static_assert(std::is_invocable<int()>::value);
    static_assert(std::is_invocable_r<int, int()>::value);
    static_assert(std::is_invocable_r<void, void(int), int>::value);
    static_assert(std::is_invocable_r<int(*)(), decltype(func2), char>::value);
}
#endif