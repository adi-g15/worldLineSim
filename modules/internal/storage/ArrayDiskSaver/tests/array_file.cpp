#include <array>
#include <cstring>
#include <iostream>

#include "MultiArraysManager.hpp"

struct serialisable {
    static inline char buffer[5000]; // a 512 Bytes buffer (use whatever max
                                     // size an object may take)

    std::pair<char *, u32> serialise() {
        int i = 0;
        for (; i < std::size(buffer); i += 4) {
            auto tmp = float(std::rand()) / 10.0f;
            std::memcpy(reinterpret_cast<void *>(buffer + i), &tmp,
                        sizeof(float));
        }

        return {buffer, i};
    }
};

std::vector<float> vec;

// cl test.cpp src/impl.cpp /Iinclude /std:c++17 /EHsc
int main() {
    try {
        MultiArrayManager::add_array_file(500);

        auto ref1 = MultiArrayManager::get_file_ref(500);
        auto ref2 = MultiArrayManager::get_file_ref(6);

        serialisable s;
        auto p = s.serialise();
        auto afile = ref1.value();
        afile.get().add_object(p.second, p.first);

        std::cout << std::boolalpha << ref1.has_value() << " "
                  << ref2.has_value() << std::endl;

        std::cout << "Reading...\n";
        vec.resize(5000 / 4);

        // Reading back
        // afile.get().get_object();

    } catch (...) {
        std::cout << "Error\n";
    }
}
