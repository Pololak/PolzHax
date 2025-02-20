#include "state.h"
#include <filesystem>
#include <fstream>
#include "utils.hpp"
#include <type_traits>

SettingStruct& setting() {
	static SettingStruct inst;
	return inst;
}

LayersStruct& layers() {
	static LayersStruct inst;
	return inst;
}

auto get_save_path() {
	return get_exe_path() / "polzhax.txt";
}

void SettingStruct::loadState() {
	const auto path = get_save_path();
	if (std::filesystem::exists(path)) {
		std::ifstream file(path);
		std::unordered_map<std::string, std::string> values;
		std::string line;
		while (std::getline(file, line)) {
			if (!line.empty())
				values.insert(split_once(line, '='));
		}

		visit_schema(*this, [&] <size_t Index> (auto & value) {
			using Schema = get_schema<SettingStruct>;
			const auto it = values.find(Schema::names[Index]);
			if (it != values.end()) {
				const auto str = it->second;
				using type = std::remove_cvref_t<decltype(value)>;
				if constexpr (std::is_same_v<type, bool>) {
					value = str == "true";
				}
				else if constexpr (std::is_same_v<type, float>) {
					value = std::stof(str);
				}
				else if constexpr (std::is_same_v<type, int>) {
					value = std::stoi(str);
				}
				else if constexpr (std::is_same_v<type, std::string>) {
					value = str;
				}
				else {
					static_assert(!std::is_same_v<type, type>, "unsupported type");
				}
			}
		});
	}
}

void SettingStruct::saveState() {
	std::ofstream file(get_save_path());

	visit_schema(*this, [&] <size_t Index> (const auto & value) {
		using Schema = get_schema<SettingStruct>;
		using type = std::remove_cvref_t<decltype(value)>;
		file << Schema::names[Index] << '=';
		if constexpr (std::is_same_v<type, bool>) {
			file << (value ? "true" : "false");
		}
		else if constexpr (std::is_same_v<type, float>) {
			file << value;
		}
		else if constexpr (std::is_same_v<type, int>) {
			file << value;
		}
		else if constexpr (std::is_same_v<type, std::string>) {
			file << value;
		}
		else {
			static_assert(!std::is_same_v<type, type>, "unsupported type");
		}
		file << std::endl;
	});
}