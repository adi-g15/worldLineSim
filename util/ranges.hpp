#pragma once

#include <algorithm>

namespace util {
	template<typename Collection>
	bool contains(Collection&& collection, typename std::remove_reference_t<Collection>::value_type& val) {
		return
			std::find(std::cbegin(collection), std::cend(collection), val)
			!=
			std::cend(collection);
	}
}
