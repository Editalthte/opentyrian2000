// https://en.cppreference.com/w/cpp/numeric/random/mersenne_twister_engine.html

#pragma once

#include <ctime>
#include <random>


class RNG
{
	private:
		std::mt19937_64 mersenne_twister_engine;


	public:
		// Default Constructor
		RNG()
		{ this->mersenne_twister_engine.seed(time(NULL)); };

		unsigned long generate()
		{ return this->mersenne_twister_engine(); }
		
};