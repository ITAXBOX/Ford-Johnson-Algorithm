#include "FordJohnson.hpp"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Usage: ./FordJohnson <numbers>" << std::endl;
		return 1;
	}

	try
	{
		FordJohnson sorter;
		sorter.sort(argv);
		sorter.displayResults();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}