#include "FordJohnson.hpp"

// ============== Orthodox Canonical Form ==============

FordJohnson::FordJohnson() : _vectorComparisons(0), _dequeComparisons(0)
{
}

FordJohnson::~FordJohnson()
{
}

FordJohnson::FordJohnson(const FordJohnson &other)
{
	*this = other;
}

FordJohnson &FordJohnson::operator=(const FordJohnson &other)
{
	if (this != &other)
	{
		_vectorData = other._vectorData;
		_dequeData = other._dequeData;
		_vectorComparisons = other._vectorComparisons;
		_dequeComparisons = other._dequeComparisons;
	}
	return *this;
}

// ============== UTILITY METHODS ==============

void FordJohnson::displayResults()
{
	std::cout << "Total comparisons for vector-based sorting: " << _vectorComparisons << std::endl;
	std::cout << "Total comparisons for deque-based sorting:  " << _dequeComparisons << std::endl;
}

void FordJohnson::validateInput(char **argv)
{
	if (argv[1] == NULL)
		throw std::invalid_argument("Error: No input provided.");

	std::set<int> seenNumbers;

	for (int i = 1; argv[i]; i++)
	{
		std::string arg = argv[i];
		std::stringstream ss(arg);
		std::string number;

		while (std::getline(ss, number, ' '))
		{
			if (number.empty())
				continue;

			for (size_t j = 0; j < number.size(); j++)
				if (!isdigit(number[j]))
					throw std::invalid_argument("Error: Input contains non-numeric characters.");

			long num = std::atol(number.c_str());
			if (num < INT_MIN || num > INT_MAX)
				throw std::invalid_argument("Error: Number is out of range (must be between 0 and 2147483647).");

			if (seenNumbers.find(static_cast<int>(num)) != seenNumbers.end())
				throw std::invalid_argument("Error: Duplicate numbers are not allowed.");
			seenNumbers.insert(static_cast<int>(num));
		}
	}
}

// ============== JACOBSTHAL NUMBER GENERATION ==============

int FordJohnson::generateJacobsthal(int n)
{
	if (n == 0)
		return 0;
	if (n == 1)
		return 1;

	int prev2 = 0, prev1 = 1, curr = 0;
	for (int i = 2; i <= n; i++)
	{
		curr = prev1 + 2 * prev2;
		prev2 = prev1;
		prev1 = curr;
	}
	return curr;
}

// ============== VECTOR IMPLEMENTATION ==============

std::vector<int> FordJohnson::mergeInsertSortVector(std::vector<int> &data, int &comparisons)
{
	if (data.empty())
		return std::vector<int>();
	if (data.size() == 1)
		return data;
	if (data.size() == 2)
	{
		std::vector<int> result = data;
		comparisons++;
		if (result[0] > result[1])
			std::swap(result[0], result[1]);
		return result;
	}

	std::vector<int> mainChain, pendChain, sortedMain, sortedPend;
	bool hasOddElement = (data.size() % 2 == 1);

	std::vector<int>::iterator it = data.begin();
	while (it != data.end())
	{
		if ((it + 1) == data.end())
			break;

		comparisons++;
		if (*it > *(it + 1))
		{
			mainChain.push_back(*it);
			pendChain.push_back(*(it + 1));
		}
		else
		{
			pendChain.push_back(*it);
			mainChain.push_back(*(it + 1));
		}
		it += 2;
	}

	if (hasOddElement)
		pendChain.push_back(data.back());

	sortedMain = mergeInsertSortVector(mainChain, comparisons);

	sortedPend.clear();
	sortedPend.resize(sortedMain.size());

	for (size_t i = 0; i < sortedMain.size(); ++i)
	{
		for (size_t j = 0; j < mainChain.size(); ++j)
		{
			if (mainChain[j] == sortedMain[i])
			{
				sortedPend[i] = pendChain[j];
				break;
			}
		}
	}

	if (hasOddElement)
		sortedPend.push_back(pendChain.back());

	performInsertionSortVector(sortedMain, sortedPend, comparisons);
	return sortedMain;
}

int FordJohnson::binarySearchPositionVector(std::vector<int> &mainChain, int value, int upperBound, int &comparisons)
{
	if (mainChain.empty())
		return 0;

	int low = 0;
	int high = (upperBound >= static_cast<int>(mainChain.size()))
				   ? mainChain.size() - 1
				   : upperBound;

	while (low <= high)
	{
		int mid = (low + high) / 2;
		comparisons++;

		if (mainChain[mid] == value)
			return mid;
		else if (mainChain[mid] > value)
			high = mid - 1;
		else
			low = mid + 1;
	}

	if (low < static_cast<int>(mainChain.size()) && value < mainChain[low])
		return low;

	return mainChain.size();
}

std::vector<int> FordJohnson::generateJacobsthalSequenceVector(int size)
{
	std::vector<int> jacobSequence;
	int jacobIndex = 3;

	while (generateJacobsthal(jacobIndex) <= size)
	{
		jacobSequence.push_back(generateJacobsthal(jacobIndex));
		jacobIndex++;
	}

	return jacobSequence;
}

void FordJohnson::adjustJacobsthalIndicesVector(std::vector<int> &jacobIndices, std::vector<int> &pendChain)
{
	std::vector<int>::iterator it = jacobIndices.begin();
	std::vector<int> processedNumbers;
	std::vector<int> adjustedIndices;

	// Adjust Jacobsthal indices to ensure they do not exceed pendChain size
	// This loop expands the Jacobsthal indices to cover all necessary positions
	// Example : If Jacobsthal indices are [3, 5], and pendChain size is 6,
	// it will expand to [3, 2, 5, 4]
	while (it != jacobIndices.end())
	{
		int x;
		if (*it >= 0)
		{
			x = *it;
			while (x > 1)
			{
				if (std::find(processedNumbers.begin(), processedNumbers.end(), x) == processedNumbers.end())
				{
					adjustedIndices.push_back(x);
					processedNumbers.push_back(x);
				}
				else
					break;
				x--;
			}
		}
		it++;
	}

	// Final adjustment to ensure all pendChain elements are covered
	// if the adjusted indices is [3, 2, 5, 4] and the pendChain size is 6,
	// it will add 6 to make it complete
	if (adjustedIndices.size() == pendChain.size())
		jacobIndices = adjustedIndices;
	else
	{
		int x = pendChain.size();
		while (adjustedIndices.size() < pendChain.size())
		{
			// Ensure no duplicates are added
			if (std::find(processedNumbers.begin(), processedNumbers.end(), x) == processedNumbers.end())
			{
				adjustedIndices.push_back(x);
				processedNumbers.push_back(x);
			}
			else
				break;
			x--;
		}
		jacobIndices = adjustedIndices;
	}
}

void FordJohnson::insertPendingElementsVector(std::vector<int> &mainChain, std::vector<int> &pendChain,
										   std::vector<int> &jacobIndices, int &comparisons)
{
	std::vector<int>::iterator it = jacobIndices.begin();
	// Initially, when inserting, we only look inside the first 3 elements
	// Ford Jhonson's algorithm proves we dont need to look further initially
	int searchBound = 3;

	// Insert the first pending element
	mainChain.insert(mainChain.begin(), pendChain[0]);

	while (it != jacobIndices.end())
	{
		// this condition is true if:
		// its not the first index AND
		// the current Jacobsthal index is greater than the previous one
		if (it != jacobIndices.begin() && *it > *(it - 1))
			searchBound = 2 * searchBound + 1;
		// Insert only if the index is within bounds
		if (*it <= static_cast<int>(pendChain.size()) && *it != 1)
		{
			int pos = binarySearchPositionVector(mainChain, pendChain[*it - 1], searchBound - 1, comparisons);
			mainChain.insert(mainChain.begin() + pos, pendChain[*it - 1]);
		}
		it++;
	}
}

void FordJohnson::performInsertionSortVector(std::vector<int> &mainChain, std::vector<int> &pendChain, int &comparisons)
{
	std::vector<int> jacobIndices = generateJacobsthalSequenceVector(pendChain.size());
	adjustJacobsthalIndicesVector(jacobIndices, pendChain);
	insertPendingElementsVector(mainChain, pendChain, jacobIndices, comparisons);
}

// ============== DEQUE IMPLEMENTATION ==============

std::deque<int> FordJohnson::mergeInsertSortDeque(std::deque<int> &data, int &comparisons)
{
	if (data.empty())
		return std::deque<int>();
	if (data.size() == 1)
		return data;
	if (data.size() == 2)
	{
		std::deque<int> result = data;
		comparisons++;
		if (result[0] > result[1])
			std::swap(result[0], result[1]);
		return result;
	}

	std::deque<int> mainChain, pendChain, sortedMain, sortedPend;
	bool hasOddElement = (data.size() % 2 == 1);

	std::deque<int>::iterator it = data.begin();
	while (it != data.end())
	{
		if ((it + 1) == data.end())
			break;

		comparisons++;
		if (*it > *(it + 1))
		{
			mainChain.push_back(*it);
			pendChain.push_back(*(it + 1));
		}
		else
		{
			pendChain.push_back(*it);
			mainChain.push_back(*(it + 1));
		}
		it += 2;
	}

	if (hasOddElement)
		pendChain.push_back(data.back());

	sortedMain = mergeInsertSortDeque(mainChain, comparisons);

	sortedPend.clear();
	sortedPend.resize(sortedMain.size());

	for (size_t i = 0; i < sortedMain.size(); ++i)
	{
		for (size_t j = 0; j < mainChain.size(); ++j)
		{
			if (mainChain[j] == sortedMain[i])
			{
				sortedPend[i] = pendChain[j];
				break;
			}
		}
	}

	if (hasOddElement)
		sortedPend.push_back(pendChain.back());

	performInsertionSortDeque(sortedMain, sortedPend, comparisons);
	return sortedMain;
}

int FordJohnson::binarySearchPositionDeque(std::deque<int> &mainChain, int value, int upperBound, int &comparisons)
{
	if (mainChain.empty())
		return 0;

	int low = 0;
	int high = (upperBound >= static_cast<int>(mainChain.size()))
				   ? mainChain.size() - 1
				   : upperBound;

	while (low <= high)
	{
		int mid = (low + high) / 2;
		comparisons++;

		if (mainChain[mid] == value)
			return mid;
		else if (mainChain[mid] > value)
			high = mid - 1;
		else
			low = mid + 1;
	}

	if (low < static_cast<int>(mainChain.size()) && value < mainChain[low])
		return low;

	return mainChain.size();
}

std::deque<int> FordJohnson::generateJacobsthalSequenceDeque(int size)
{
	std::deque<int> jacobSequence;
	int jacobIndex = 3;

	while (generateJacobsthal(jacobIndex) <= size)
	{
		jacobSequence.push_back(generateJacobsthal(jacobIndex));
		jacobIndex++;
	}

	return jacobSequence;
}

void FordJohnson::adjustJacobsthalIndicesDeque(std::deque<int> &jacobIndices, std::deque<int> &pendChain)
{
	std::deque<int>::iterator it = jacobIndices.begin();
	std::deque<int> processedNumbers;
	std::deque<int> adjustedIndices;

	while (it != jacobIndices.end())
	{
		int x;
		if (*it >= 0)
		{
			x = *it;
			while (x > 1)
			{
				if (std::find(processedNumbers.begin(), processedNumbers.end(), x) == processedNumbers.end())
				{
					adjustedIndices.push_back(x);
					processedNumbers.push_back(x);
				}
				else
					break;
				x--;
			}
		}
		it++;
	}

	if (adjustedIndices.size() == pendChain.size())
		jacobIndices = adjustedIndices;
	else
	{
		int x = pendChain.size();
		while (adjustedIndices.size() < pendChain.size())
		{
			if (std::find(processedNumbers.begin(), processedNumbers.end(), x) == processedNumbers.end())
			{
				adjustedIndices.push_back(x);
				processedNumbers.push_back(x);
			}
			else
				break;
			x--;
		}
		jacobIndices = adjustedIndices;
	}
}

void FordJohnson::insertPendingElementsDeque(std::deque<int> &mainChain, std::deque<int> &pendChain,
										  std::deque<int> &jacobIndices, int &comparisons)
{
	std::deque<int>::iterator it = jacobIndices.begin();
	int searchBound = 3;

	mainChain.insert(mainChain.begin(), pendChain[0]);

	while (it != jacobIndices.end())
	{
		if (it != jacobIndices.begin() && *it > *(it - 1))
			searchBound = 2 * searchBound + 1;

		if (*it <= static_cast<int>(pendChain.size()) && *it != 1)
		{
			int pos = binarySearchPositionDeque(mainChain, pendChain[*it - 1], searchBound - 1, comparisons);
			mainChain.insert(mainChain.begin() + pos, pendChain[*it - 1]);
		}
		it++;
	}
}

void FordJohnson::performInsertionSortDeque(std::deque<int> &mainChain, std::deque<int> &pendChain, int &comparisons)
{
	std::deque<int> jacobIndices = generateJacobsthalSequenceDeque(pendChain.size());
	adjustJacobsthalIndicesDeque(jacobIndices, pendChain);
	insertPendingElementsDeque(mainChain, pendChain, jacobIndices, comparisons);
}

// ============== SORTING AND DISPLAY METHODS ==============

void FordJohnson::sort(char **argv)
{
	validateInput(argv);

	for (int i = 1; argv[i]; i++)
	{
		std::string arg = argv[i];
		std::stringstream ss(arg);
		std::string number;

		// the getline function to split by space
		while (std::getline(ss, number, ' '))
		{
			if (number.empty())
				continue;
			int num = std::atoi(number.c_str());
			_vectorData.push_back(num);
			_dequeData.push_back(num);
		}
	}

	std::cout << "Before:  ";
	for (std::size_t i = 0; i < _vectorData.size(); ++i)
	{
		std::cout << _vectorData[i];
		if (i < _vectorData.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;

	// Sort vector
	_vectorComparisons = 0;
	clock_t startVector = clock();
	_vectorData = mergeInsertSortVector(_vectorData, _vectorComparisons);
	clock_t endVector = clock();
	// convert to microseconds
	double timeVector = static_cast<double>(endVector - startVector) / CLOCKS_PER_SEC * 1000000;

	// Sort deque
	_dequeComparisons = 0;
	clock_t startDeque = clock();
	_dequeData = mergeInsertSortDeque(_dequeData, _dequeComparisons);
	clock_t endDeque = clock();
	// convert to microseconds
	double timeDeque = static_cast<double>(endDeque - startDeque) / CLOCKS_PER_SEC * 1000000;

	std::cout << "After:   ";
	for (std::size_t i = 0; i < _vectorData.size(); ++i)
	{
		std::cout << _vectorData[i];
		if (i < _vectorData.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;

	std::cout << "Time to process a range of " << _dequeData.size()
			  << " elements with std::deque:  " << std::fixed
			  << std::setprecision(6) << timeDeque << " us" << std::endl;
	std::cout << "Time to process a range of " << _vectorData.size()
			  << " elements with std::vector: " << std::fixed
			  << std::setprecision(6) << timeVector << " us" << std::endl;
}
