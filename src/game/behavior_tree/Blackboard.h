#pragma once

#include <unordered_map>
#include <string>

namespace BehaviorTree {
	class Blackboard {
	public:

	private:
		std::unordered_map<std::string, void*> data;
	};

}