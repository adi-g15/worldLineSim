#include <iostream>
#include <thread>
#include <chrono>
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