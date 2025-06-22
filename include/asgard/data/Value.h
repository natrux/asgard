#pragma once

#include <asgard/io/typecode_t.h>


namespace asgard{

namespace io{
class TypeReader;
class TypeWriter;
}

namespace data{


class Value{
public:
	virtual ~Value() = default;
};



}
}
