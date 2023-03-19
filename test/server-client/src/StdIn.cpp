#include <StdIn.h>
#include <TextLine.hxx>

#include <iostream>
#include <thread>


StdIn::StdIn(const std::string &name_):
	StdInModule(name_)
{
}


void StdIn::main(){
	std::thread read_thread(&StdIn::read_loop, this);

	Super::main();

	if(read_thread.joinable()){
		read_thread.join();
	}
}


void StdIn::read_loop(){
	while(node_should_run()){
		std::string next_line;
		std::getline(std::cin, next_line);

		auto sample = std::make_shared<TextLine>();
		sample->line = next_line;
		publish(sample, output_lines);
	}
}

