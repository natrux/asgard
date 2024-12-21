#pragma once


#include <asgard/data/Data.hxx>
#include <asgard/time/time.h>


namespace asgard{
namespace data{

class EndOfFile : public Data{
public:
	time::time time;
};


}
}
