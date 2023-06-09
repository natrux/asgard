#pragma once

#include <asgard/com/Gateway.h>
#include <TextLine.hxx>


class ShoutGatewayModule : public asgard::com::Gateway{
public:
	using Super = asgard::com::Gateway;

	ShoutGatewayModule(const std::string &name_);

protected:
	asgard::topic::TopicPtr input_text = "input.terminal";

	using Super::process;
	void process(std::shared_ptr<const asgard::data::Sample> sample) override;
	virtual void process(std::shared_ptr<const asgard::data::Sample> sample, std::shared_ptr<const TextLine> data);
	virtual void process(std::shared_ptr<const TextLine> value);
};

