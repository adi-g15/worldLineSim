#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <map>

enum class Gender {
	MALE,
	FEMALE
};

class db {
	using id_type = unsigned int;

	static inline std::vector<std::string> male_names, female_names;
	static constexpr int num_names = 200;	// will store 200 names only, at max

	static inline std::map<id_type, std::string_view> id_to_name_mapping;	// used to save storage for multiple HumanState objects

public:
	static void init() {
		std::string name;	name.reserve(20);

		male_names.reserve(num_names);
		female_names.reserve(num_names);
		int counter = num_names;

		std::string source_path(__FILE__);
		auto last_slash_index = source_path.find_last_of('/');
		if (last_slash_index == std::string::npos) {
			last_slash_index = source_path.find_last_of('\\');
		}

		source_path.erase(last_slash_index+1);	// we want to keep the '/' || '\\'
		std::ifstream name_reader( source_path + "Indian_Male_Names.list");
		if (name_reader) {
			while ((counter-- > 0) && !name_reader.eof())
			{
				std::getline(name_reader, name);
				name_reader.tellg();

				male_names.push_back(name);
			}
		}
		else
		{
			male_names.emplace_back("Human");
		}

		name_reader.close();

		name_reader.open(source_path + "Indian_Female_Names.list");
		counter = num_names;
		if (name_reader) {
			while ((counter-- > 0) && !name_reader.eof())
			{
				std::getline(name_reader, name);

				female_names.push_back(name);
			}
		}
		else
		{
			female_names.emplace_back("Human");	// general name, since failed to fetch names
		}
	}

	static const std::string& getRandomName(Gender gender, id_type id) {
		const id_type rand_index = rand() % (gender == Gender::MALE ? male_names.size(): female_names.size());	// random index

		switch (gender)
		{
		case Gender::MALE:
			id_to_name_mapping.insert({ id, male_names[rand_index] });
			return male_names[rand_index];
		case Gender::FEMALE:
			id_to_name_mapping.insert({ id, female_names[rand_index] });
			return female_names[rand_index];
		}
	}

	inline static std::string getNameFromId(uint32_t id) {
		std::string name(id_to_name_mapping[id]);
		return name;
	}
};
