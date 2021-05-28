#include <vector>
#include <fstream>
#include <string>
#include "types.hpp"

constexpr u32 MAX_OFFSETS = 500;

class ArrayFile {
	using OffsetType = u64;

	std::string m_file_name;
	std::fstream m_file;
	static inline u64 ZEROES[MAX_OFFSETS]{0};

	struct _ArrayFileOffsetData {
		u64 extended_index_offset;	/*ignore: As of now, it's always 0 as of now, and non-zero case isn't covered*/
		/*
		* We don't need to store how many offsets are valid (ie how many objects in array), since it is simply = number of NON-ZERO offsets
			u32 num_valid_offsets{ MAX_OFFSETS };
		*/

		u64 next_valid_offset;	// this MUST be updated again and again

		std::vector<u64> index_offsets;
	} m_offset_data;

	u64 next_object_index;	// similar to next_valid_offset, but this stores the "LOCATION OF 'THE' OFFSET that is to be updated when a new object is added at next_valid_offset"

	void update_next_valid_offset(u64 new_next_valid);

public:
	ArrayFile(const std::string& file_name);
	~ArrayFile();

	void add_object(u32 obj_size_bytes, void* obj_data);
	void get_object(u32 obj_index);
};
