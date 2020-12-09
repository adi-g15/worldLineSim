//#include <thread>
//#include <vector>
//#include <iostream>
//#include <atomic>
//
//std::atomic_bool my_flag = false;
//std::atomic_flag lock = ATOMIC_FLAG_INIT;
//
//void f(int n)
//{
//    for (int cnt = 0; cnt < 100; ++cnt) {
//        while (lock.test_and_set(std::memory_order_acquire))  // acquire lock
//            ; // spin
//        std::cout << "Output from thread " << n << '\n';
//        lock.clear(std::memory_order_release);               // release lock
//    }
//}
//
//int main()
//{
//    std::vector<std::thread> v;
//    for (int n = 0; n < 10; ++n) {
//        v.emplace_back(f, n);
//    }
//    for (auto& t : v) {
//        t.join();
//    }
//}

#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <algorithm>
#include <numeric>
#include "world_node.hpp"

using namespace std::chrono_literals;

std::random_device util::Random::device;
std::mt19937 util::Random::generator(util::Random::device()) ;
id_type _ID::_curr_ID = util::Random::random(100000);   // @note - Maybe temporarily, so that ids dont' just start with 0,1,2 and so on

class test
{
	World_Node* new_world;
public:
	void emulate() {
		//new_world = new World_Node();
	}
};

int main() {
	std::thread(&test::emulate, test()).detach();

	std::vector<int> v(20);
	std::iota(v.rbegin(), v.rend(), 0);
	std::shuffle(v.rbegin(), v.rend(), util::Random::generator);

	for (auto& i : v)
	{
		std::cout << i << ' ';
	}

	std::cout << std::endl;
}
