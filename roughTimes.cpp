/******************************************************************************
* cs325-400-W17	Analysis of Algorithms	29 Jan 2017	Project 1
* Jonathon Horton	hortonjo@oregonstate.edu
* Chris Kearns		kearnsc@oregonstate.edu
* Dustin Pack		packdu@oregonstate.edu
*
* Program maxSumSubarray.cpp reads a text file of space delimited ints and 
* appends the Maximum Sum Subarray and sum of each line to a seperate text
* file. Compile with: g++ maxSumSubarray.cpp -o maxSum -g -Wall -std=c++11
* Call from CLI: "maxSum 1" to show with execution times, "maxSum 0" otherwise.
******************************************************************************/
#include "maxSumSubarray.hpp"

int main(int argc, char** argv) {
	if (argc < 2 || atoi(argv[1]) < 0 || atoi(argv[1]) > 1) {
		cout << "Usage: \"maxSum 1\" or \"maxSum 0\"\n" <<
			"0 for do not show execution times, 1 otherwise.\n";
		exit(1);
	}

	int showTime = atoi(argv[1]);


	std::vector<std::vector<int> > allData;	// 2D input vector.
	std::vector<std::vector<int> > results;	// 2d results vector.

	std::ifstream inputFile("tdata.txt");
	std::string eachLine;
	int eachInt;

	// Input the text file of ints to allData.
	while (std::getline(inputFile, eachLine)) {
		std::vector<int> lineData;		// 1D vector.
		std::stringstream inputStream(eachLine);

		while (inputStream >> eachInt) {
			// Add the ints from each line to the 1D vector.
			lineData.push_back(eachInt);
		}
		// Push the 1D vector onto the 2D vector as one "line".
		allData.push_back(lineData);
	}
	inputFile.close();

	// Since we will have multiple write/append calls, we delete the old
	// MSS_Results.txt file first to ensure a "clean start".
	if (fileExists("MSS_Results.txt")) {
		if (remove("MSS_Results.txt") != 0) {
			perror("Error deleting old MSS_Results.txt: ");
		}
	}

	// Find the maxSumSubarray and append to MSS_Results.txt. Vector results modified in place.
	maxSumSubArray_1(allData, results, showTime);
	maxSumSubArray_2(allData, results, showTime);

	/* Since function maxSumsubarray_3() is recursive and involves multiple return statements,
	   we manually build results 2D vector here in main. Additionally, we time the call to 
	   maxSumSubarray_3 from main, so the call and final return, considered constants, are
	   included but considered negligible when comparing to the other maxSumSubArray functions.*/

	int maxSum;
 	std::vector<int> mssTotal; // 1D int vector (of length 1) to capture the maxSumSubarray total.
	results.clear();
	for (unsigned int i = 0; i < allData.size(); i++) {
		results.push_back(allData[i]);									// Add the input vector.
		results.push_back(getSubArray(allData[i]));							// Add the derived vector.

		//Timer algoTime;
		// TIME FROM HERE...
		//double start_ = algoTime.elapsed();

		// typical clock method from stackoverflow
		std::clock_t start;
		double execTime;
		start = std::clock();

		maxSum = maxSumSubArray_3(allData[i], 0, allData[i].size() - 1);			// Get the maxSum.

		// ... TO HERE.
		//algoTime.reset();
		//double stop = algoTime.elapsed();
		//double execTime = ((start_ - stop) / (double)CLOCKS_PER_SEC);
		execTime = ( std::clock() - start) / (double) CLOCKS_PER_SEC;

		if (showTime) {
			cout << std::fixed << "Elapsed time for maxSumSubArray_3, lineNum: "
				<< i + 1 << " = " << execTime << '\n';
		}

		mssTotal.clear();
		mssTotal.push_back(maxSum);										// Add maxSum to mssTotal.
		results.push_back(mssTotal);										// Add the maxSum vector.
	}
	append2file(results);

	maxSumSubArray_4(allData, results, showTime);

	return 0;
}


/* Determine max sum of subarray with brute force method. All posible combinations
   of subarrays checked. O(n^3) [3] */
void maxSumSubArray_1(std::vector<std::vector<int> > allData, std::vector<std::vector<int> > &results, int showTime) {
	// 1D int vector to capture the maxSumSubarray elements.
	std::vector<int> resultsData;

	// 1D int vector (of length 1) to capture the maxSumSubarray total. 
	std::vector<int> mssTotal;

	// Clear 2D results vector (passed by ref, so old 1D vectors still hanging around).
	results.clear();

	Timer algoTime;

	// Process allData vector.
	for (unsigned int lineNum = 0; lineNum < allData.size(); lineNum++) {
		// TIME FROM HERE...
		//double start = algoTime.elapsed();
		
		// typical clock method from stackoverflow
		std::clock_t start;
		double execTime;
		start = std::clock();


		int maxSum = allData[lineNum][0];
		unsigned int maxSumBegin = 0, maxSumEnd = 0;

		// The brute force algorithm.
		for (unsigned int i = 1; i < allData[lineNum].size(); i++) {
			int temp = 0;
			int j = i;
			while (j >= 0) {
				temp += allData[lineNum][j];
				if (temp > maxSum) {
					maxSum = temp;
					maxSumBegin = j;
					maxSumEnd = i;
				}
				j--;
			}
		}

		// ...TO HERE!
		//algoTime.reset();
		//double stop = algoTime.elapsed();
		//double execTime = ((start - stop) / (double)CLOCKS_PER_SEC);
		execTime = ( std::clock() - start) / (double) CLOCKS_PER_SEC;
		if (showTime) {
			cout << std::fixed << "Elapsed time for maxSumSubArray_1, lineNum: " << lineNum + 1 << " = " << execTime << '\n';
		}

		// Push the allData[lineNum] onto results.
		results.push_back(allData[lineNum]);

		// Push allData[lineNum][i] from index maxSumStart to maxSumEnd onto resultsData.
		resultsData.clear();
		for (unsigned int i = maxSumBegin; i <= maxSumEnd; i++) {
			resultsData.push_back(allData[lineNum][i]);
		}

		// Push resultsData onto results.
		results.push_back(resultsData);

		// Push maxSum onto results[++i]
		mssTotal.clear();
		mssTotal.push_back(maxSum);
		results.push_back(mssTotal);
	}
	append2file(results);
}


/* Determine max sum of subarray with brute force method. Keeps track of largest
	subarray checked during iteration. O(n^2) [3] */
void maxSumSubArray_2(std::vector<std::vector<int> > allData, std::vector<std::vector<int> > &results, int showTime) {
	// 1D int vector to capture the maxSumSubarray elements.
	std::vector<int> resultsData;

	// 1D int vector (of length 1) to capture the maxSumSubarray total. 
	std::vector<int> mssTotal;

	// Clear 2D results vector (passed by ref, so old 1D vectors still hanging around).
	results.clear();

	Timer algoTime;

	// Process allData vector.
	for (unsigned int lineNum = 0; lineNum < allData.size(); lineNum++) {
		// TIME FROM HERE...
		//double start = algoTime.elapsed();
		
		// typical clock method from stackoverflow
		std::clock_t start;
		double execTime;
		start = std::clock();


		int maxSum = allData[lineNum][0];
		unsigned int maxSumBegin = 0, maxSumEnd = 0;

		// The enhanced brute force algorithm.
		for (unsigned int i = 1; i < allData[lineNum].size(); i++) {
			int temp = 0;
			int j = i;
			int start = maxSumBegin;
			while (j >= start) {
				temp += allData[lineNum][j];
				if (temp > maxSum) {
					maxSum = temp;
					maxSumBegin = j;
					maxSumEnd = i;
				}
				j--;
			}
		}

		// ...TO HERE!
		//algoTime.reset();
		//double stop = algoTime.elapsed();
		//double execTime = ((start - stop) / (double)CLOCKS_PER_SEC);
		execTime = ( std::clock() - start) / (double) CLOCKS_PER_SEC;
		if (showTime) {
			cout << std::fixed << "Elapsed time for maxSumSubArray_2, lineNum: " << lineNum + 1 << " = " << execTime << '\n';
		}

		// Push the allData[lineNum] onto results.
		results.push_back(allData[lineNum]);

		// Push allData[lineNum][i] from index maxSumStart to maxSumEnd onto resultsData.
		resultsData.clear();
		for (unsigned int i = maxSumBegin; i <= maxSumEnd; i++) {
			resultsData.push_back(allData[lineNum][i]);
		}

		// Push resultsData onto results.
		results.push_back(resultsData);

		// Push maxSum onto results[++i]
		mssTotal.clear();
		mssTotal.push_back(maxSum);
		results.push_back(mssTotal);
	}
	append2file(results);
}

/***********************************************************************************
*                         Algorithm 3: Divide and Conquer
* This algorithm will split the array into halves recursively and recombine.
* We know the maximum subarray will be in the first half, the second half, or made
* from the end of the first half and the beginning of the second half. O(nlogn)
***********************************************************************************/
int maxSumSubArray_3(std::vector<int> arr, int start, int end) {
	if (start == end)	// Base case n = 1
		return arr[start];
	else {			// Anything bigger gets broken down.
		int mid = (start + end) / 2;

		int leftMax = maxSumSubArray_3(arr, start, mid);
		int rightMax = maxSumSubArray_3(arr, mid + 1, end);

		int suffix = MaxSuffix(arr, start, mid);
		int prefix = MaxPrefix(arr, mid + 1, end);

		// Return whichever has the highest value.
		if (leftMax >= rightMax && leftMax >= (suffix + prefix)) {
			return leftMax;
		}
		else if (rightMax >= leftMax && rightMax >= (suffix + prefix)) {
			return rightMax;
		}
		else {
			return (suffix + prefix);
		}
	}
}

/* The iterative method that scans the array left to right, keeping the best
   sum and current sum. Runs O(n). */
void maxSumSubArray_4(std::vector<std::vector<int> > allData, std::vector<std::vector<int> > &results, int showTime) {
	std::vector<int> resultsData;	// 1D int vector to capture the maxSumSubarray elements.
	std::vector<int> mssTotal;	// 1D int vector (of length 1) to capture the maxSumSubarray total.

	// Clear 2D results vector (passed by ref, so old 1D vectors still hanging around).
	results.clear();

	Timer algoTime;

	// Process the 2D vector allData.
	for (unsigned int lineNum = 0; lineNum < allData.size(); lineNum++) {
		int currentSum = 0;
		int maxSum = std::numeric_limits<int>::min();  //Initially set MaxSum to the lowest possible integer.
		int currentArrayStart = 0;
		int maxStart = 0; //Beginning index of the max subarray
		int maxEnd = 0; //Ending index of the max subarray

		// TIME FROM HERE...
		//double start_ = algoTime.elapsed();
		
		// typical clock method from stackoverflow
		std::clock_t start;
		double execTime;
		start = std::clock();


		/* Determine max sum. O(n) */
		for (unsigned int i = 0; i < allData[lineNum].size(); i++) {
			currentSum = currentSum + allData[lineNum][i];
			if (currentSum > maxSum) {
				maxSum = currentSum;
				maxStart = currentArrayStart;
				maxEnd = i;
			}
			else if (currentSum < 0) {
				currentSum = 0;
				currentArrayStart = i + 1;
			}
		}

		// ...TO HERE!
		//algoTime.reset();
		//double stop = algoTime.elapsed();
		//double execTime = ((start_ - stop) / (double)CLOCKS_PER_SEC);
		execTime = ( std::clock() - start) / (double) CLOCKS_PER_SEC;
		if (showTime) {
			cout << std::fixed << "Elapsed time for maxSumSubArray_4, lineNum: " << lineNum + 1 << " = " << execTime << '\n';
		}

		// Push the allData[lineNum] onto results.
		results.push_back(allData[lineNum]);

		// Push allData[lineNum][i] from index maxSumStart to maxSumEnd onto resultsData.
		resultsData.clear();
		for (int i = maxStart; i <= maxEnd; i++) {
			resultsData.push_back(allData[lineNum][i]);
		}

		// Push resultsData onto results.
		results.push_back(resultsData);

		// Push maxSum onto results[++i]
		mssTotal.clear();
		mssTotal.push_back(maxSum);
		results.push_back(mssTotal);
	}
	append2file(results);
}


// Uses stat() to check for file existence. [1]
bool fileExists(std::string fileName) {
	struct stat buffer;
	return (stat(fileName.c_str(), &buffer) == 0);
}


// Appends results data to text file. 
void append2file(std::vector<std::vector<int> > &results) {

	// Open the results file for appending.	[2]
	std::ofstream resultFile("MSS_Results.txt", std::ios_base::app);

	int skipLines = 0;
	// Test print results.  Change "cout" to "resultFile" for file writing.
	for (unsigned int i = 0; i < results.size(); i++) {
		skipLines++;
		for (unsigned int j = 0; j < results[i].size(); j++) {
			//cout << results[i][j];
//			resultFile << results[i][j];
			//cout << " ";
//			resultFile << " ";

		}
		//cout << '\n';
//		resultFile << '\n';
		if (skipLines % 3 == 0) {
			//cout << '\n';
//			resultFile << '\n';
		}
	}
	resultFile.close();
}


/******************************************************************************
* Helper functions for maxSumSubArray_3, which will compute maximum prefix and
* suffixes and append maxSumSubArray found info to MSS_Results.txt
******************************************************************************/

// Finds the rightmost maximum sum
int MaxSuffix(std::vector<int> arr, int start, int end) {
	int maxSum = arr[end];
	int sum = 0;
	for (int j = end; j >= start; j--) {
		sum = sum + arr[j];
		if (sum > maxSum)
			maxSum = sum;
	}
	return maxSum;
}

// Finds the leftmost maximum sum
int MaxPrefix(std::vector<int> arr, int start, int end) {
	int maxSum = arr[start];
	int sum = 0;
	for (int j = start; j <= end; j++) {
		sum = sum + arr[j];
		if (sum > maxSum)
			maxSum = sum;
	}
	return maxSum;
}


// Returns subArray "?" from input vector arr.
std::vector<int> getSubArray(std::vector<int> arr) {
	int n = arr.size();			// Get size of input vector.
	std::vector<int> subArray;	// Create vector to be returned.
	int max2date = INT_MIN;
	int maxEnd = 0;
	int start = 0;
	int end = 0;
	int temp = 0;
	// O(n).
	for (int i = 0; i < n; i++) {
		maxEnd += arr[i];
		if (max2date < maxEnd) {
			max2date = maxEnd;
			start = temp;
			end = i;
		}
		if (maxEnd < 0) {
			maxEnd = 0;
			temp = i + 1;
		}
	}
	for (int i = start; i <= end; i++) {
		subArray.push_back(arr[i]);
	}
	return subArray;
}


/* CITATIONS: Code adapted from the following sources:
[1] http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
[2] http://stackoverflow.com/questions/26084885/appending-to-a-file-with-ofstream
[3] http://cpluspluscode.blogspot.com/2012/11/maximum-subarray-problem.html
*/
