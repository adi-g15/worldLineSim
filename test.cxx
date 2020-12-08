#include <thread>
#include <vector>
#include <iostream>
#include <atomic>

std::atomic_bool my_flag = false;
std::atomic_flag lock = ATOMIC_FLAG_INIT;

void f(int n)
{
    for (int cnt = 0; cnt < 100; ++cnt) {
        while (lock.test_and_set(std::memory_order_acquire))  // acquire lock
            ; // spin
        std::cout << "Output from thread " << n << '\n';
        lock.clear(std::memory_order_release);               // release lock
    }
}

int main()
{
    std::vector<std::thread> v;
    for (int n = 0; n < 10; ++n) {
        v.emplace_back(f, n);
    }
    for (auto& t : v) {
        t.join();
    }
}

#if 0
#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include "world_node.hpp"

using namespace std::chrono_literals;

class test
{
	World_Node* new_world;
public:
	void emulate() {
		new_world = new World_Node();
	}
};

int main() {
	std::thread(&test::emulate, test()).detach();

	std::this_thread::sleep_for( 10s );
}
#endif