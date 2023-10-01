#pragma once

#include <stdexcept>
#include <memory>
#include <asgard/data/Exception.hxx>


namespace asgard{
namespace core{


class exception : public std::exception{
public:
	exception(std::shared_ptr<const data::Exception> error);

	const char *what() const noexcept override;
	std::shared_ptr<const data::Exception> get_exception() const;

private:
	std::string error_string;
	std::shared_ptr<const data::Exception> exception_object;
};


}
}
