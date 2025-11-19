#ifndef FordJohnson_HPP
#define FordJohnson_HPP

#include <iostream>
#include <vector>
#include <deque>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <utility>
#include <climits>
#include <iomanip>
#include <set>

class FordJohnson
{
private:
	std::vector<int> _vectorData;
	std::deque<int> _dequeData;
	int _vectorComparisons;
	int _dequeComparisons;

	void validateInput(char **argv);
	int generateJacobsthal(int n);

	std::vector<int> mergeInsertSortVector(std::vector<int> &data, int &comparisons);
	void performInsertionSortVector(std::vector<int> &mainChain, std::vector<int> &pendChain, int &comparisons);
	int binarySearchPositionVector(std::vector<int> &mainChain, int value, int upperBound, int &comparisons);
	std::vector<int> generateJacobsthalSequenceVector(int size);
	void adjustJacobsthalIndicesVector(std::vector<int> &jacobIndices, std::vector<int> &pendChain);
	void insertPendingElementsVector(std::vector<int> &mainChain, std::vector<int> &pendChain, std::vector<int> &jacobIndices, int &comparisons);

	std::deque<int> mergeInsertSortDeque(std::deque<int> &data, int &comparisons);
	void performInsertionSortDeque(std::deque<int> &mainChain, std::deque<int> &pendChain, int &comparisons);
	int binarySearchPositionDeque(std::deque<int> &mainChain, int value, int upperBound, int &comparisons);
	std::deque<int> generateJacobsthalSequenceDeque(int size);
	void adjustJacobsthalIndicesDeque(std::deque<int> &jacobIndices, std::deque<int> &pendChain);
	void insertPendingElementsDeque(std::deque<int> &mainChain, std::deque<int> &pendChain, std::deque<int> &jacobIndices, int &comparisons);

public:
	FordJohnson();
	~FordJohnson();
	FordJohnson(const FordJohnson &other);
	FordJohnson &operator=(const FordJohnson &other);

	void sort(char **argv);
	void displayResults();
};

#endif