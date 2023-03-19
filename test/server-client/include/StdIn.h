#pragma once

#include <StdInModule.hxx>


class StdIn : public StdInModule{
public:
	StdIn(const std::string &name_);

protected:
	void main() override;

private:
	void read_loop();
};


