#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <typeindex>
#include <iostream>

namespace BT {
	class Blackboard {
	public:
		Blackboard() = default;
		~Blackboard() = default;
		
		template <typename T>
		void setValue(const std::string& key, T value) {
			data.insert_or_assign(key, std::make_pair(std::type_index(typeid(T)), std::make_shared<T>(std::move(value))));
		}

		template <typename T>
		T getValue(const std::string& key) const {
			auto it = data.find(key);
			if (it == data.end()) {
				std::cout << "[Blackboard] Key <" << key << "> not found." << std::endl;
				return T{};
			}

			if (it->second.first != std::type_index(typeid(T))) {
				std::cout << "[Blackboard] Type mismatch when getting for key <" << key << ">" << std::endl;
				return T{};
			}

			return *std::static_pointer_cast<T>(it->second.second);
		}

		bool hasKey(const std::string& key) const {
			return data.find(key) != data.end();
		}

		void remove(const std::string& key) {
			data.erase(key);
		}

	private:
		std::unordered_map<std::string, std::pair<std::type_index, std::shared_ptr<void>>> data;

	};

}