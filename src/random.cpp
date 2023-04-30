#include <asgard/core/random.h>

#include <random>
#include <mutex>
#include <chrono>


namespace asgard{
namespace core{


class Random{
public:
	static bool rand1();
	static uint64_t rand64();
	static double rand();

private:
	static std::mutex mutex_random;
	static bool is_initialized;

	static std::random_device device;
	static std::mt19937_64 generator;
	static std::bernoulli_distribution laplace_distribution;
	static std::uniform_real_distribution<double> uniform_0_1_distribution;

	static void init();
};


std::mutex Random::mutex_random;
bool Random::is_initialized = false;

std::random_device Random::device;
std::mt19937_64 Random::generator(Random::device());
std::bernoulli_distribution Random::laplace_distribution(0.5);
std::uniform_real_distribution<double> Random::uniform_0_1_distribution(0, 1);



bool Random::rand1(){
	std::lock_guard<std::mutex> lock(mutex_random);
	init();
	return laplace_distribution(generator);
}


uint64_t Random::rand64(){
	std::lock_guard<std::mutex> lock(mutex_random);
	init();
	return generator();
}


double Random::rand(){
	std::lock_guard<std::mutex> lock(mutex_random);
	init();
	return uniform_0_1_distribution(generator);
}


// internal function. Mutex must be locked.
void Random::init(){
	if(!is_initialized){
		const auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		const auto seed = static_cast<decltype(generator)::result_type>(time);
		generator.seed(seed);
		is_initialized = true;
	}
}




// public API


bool rand1(){
	return Random::rand1();
}


uint64_t rand64(){
	return Random::rand64();
}


double rand(){
	return Random::rand();
}


}
}

