#include <filesystem>
#include <fstream>
#include <map>
#include <optional>

#include "ArrayFile.hpp"
#include "types.hpp"

using std::map, std::reference_wrapper;

/*The type used for indexing, by default an integer*/
// template<typename IndexType = u32>
class MultiArrayManager {
	/* for each array, we use a new file */
	//static inline unordered_map<std::string, ArrayFile> array_files;
	static inline map<u32, ArrayFile> array_files;

public:
	static void add_array_file(u32 file_id);

	static std::optional<reference_wrapper<ArrayFile>> get_file_ref(u32 file_id);
};
