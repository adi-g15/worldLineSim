#include <iostream>

#include "MultiArraysManager.hpp"

using std::vector, std::ios_base;

void MultiArrayManager::add_array_file(u32 file_id) {
    /*If already the file id is there, don't create or affect the already
     * exisiting file*/
    if (array_files.find(file_id) != array_files.end()) {
        return;
    }

    array_files.emplace(file_id, std::to_string(file_id));
}

std::optional<reference_wrapper<ArrayFile>>
MultiArrayManager::get_file_ref(u32 file_id) {
    if (array_files.find(file_id) == array_files.end()) /*[[unlikely]]*/ {
        return {};
    }
    return array_files.at(file_id);
}

ArrayFile::ArrayFile(const std::string &file_name) {
    m_file.open(file_name, ios_base::binary | ios_base::trunc | ios_base::in |
                               ios_base::out);

    if (!m_file.is_open()) {
        std::cerr << "FAILED CREATING FILE" << std::endl;
    }

    m_offset_data.extended_index_offset = 0;
    m_offset_data.next_valid_offset =
        sizeof m_offset_data
            .extended_index_offset + // extended index offset - 8 bytes
        sizeof m_offset_data.next_valid_offset + // self - 8 bytes
        MAX_OFFSETS * sizeof(u64) // offset index - MAX_OFFSETS * 8 bytes
        ;                         // ie. 2 * sizeof u64 = 16
    m_offset_data.index_offsets.resize(
        MAX_OFFSETS); // by default, each file has 500 offset locations reserved

    m_file.write(reinterpret_cast<char *>(&m_offset_data.extended_index_offset),
                 sizeof(m_offset_data.extended_index_offset));
    m_file.write(reinterpret_cast<char *>(&m_offset_data.next_valid_offset),
                 sizeof(m_offset_data.next_valid_offset));

    m_file.write(
        reinterpret_cast<char *>(ArrayFile::ZEROES),
        MAX_OFFSETS *
            sizeof(u64)); /*gcc doesn't recognise 'sizeof u64', though MSVC
                             accepts, likely a compiler extension then*/
}

ArrayFile::~ArrayFile() { m_file.close(); }

void ArrayFile::update_next_valid_offset(u64 new_next_valid) {
    const auto original_pos = m_file.tellp();

    m_file.seekp(
        sizeof m_offset_data.extended_index_offset); // since next_valid_offset,
                                                     // stored JUST after this
    m_file.write(reinterpret_cast<char *>(&new_next_valid),
                 sizeof new_next_valid);

    m_file.seekp(original_pos);
}

void ArrayFile::add_object(u32 obj_size_bytes, void *obj_data) {
    std::cout << "Adding " << obj_size_bytes << " bytes to file: \""
              << m_file_name << "\"";
    using std::cout, std::endl;

    cout << "Currently at " << m_file.tellp() << endl;
    m_file.seekp(m_offset_data.next_valid_offset);
    cout << "Now at " << m_file.tellp() << " (Should be "
         << m_offset_data.next_valid_offset << ")" << endl;

    // TODO: NOT WRITING TO CORRECT PLACE
    /*For each object (array element), first we store its size (a 4 byte uint)*/
    m_file.write(reinterpret_cast<char *>(&obj_size_bytes),
                 sizeof obj_size_bytes);
    cout << "Now at " << m_file.tellp() << endl;
    /*Then the data itself*/
    m_file.write(reinterpret_cast<char *>(obj_data), obj_size_bytes);

    cout << "Now at " << m_file.tellp() << endl;
    update_next_valid_offset(m_offset_data.next_valid_offset + obj_size_bytes);
}
