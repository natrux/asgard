#pragma once

#include <string>
#include <vector>
#include <memory>


namespace asgard{
namespace codegen{


class DataType;


class Declaration{
public:
	std::string to_string() const;

private:
	std::string name;
	std::shared_ptr<DataType> type;
	std::vector<std::shared_ptr<DataType>> template_types;
	std::unique_ptr<std::string> initializer;
	bool is_const = false;
	bool is_reference = false;
};


}
}
