/****************************************************
 * Fiddling around to try to get algorithm 3 working
 ****************************************************/

#include <iostream>
using std::cout;
using std::endl;

// Prototypes
int AlGore3(int [], int, int);
int MaxSuffix(int [], int, int);
int MaxPrefix(int [], int, int); 


/*******
 * main
 *******/
int main() {
	
	// I'm just creating some data for testing.
	int arr[] = {31, -41, 59, 26, -53, 58, 97, -93, -23, 84};
	int n = sizeof(arr)/sizeof(arr[0]);
	//std::vector<int> lineData (arr, arr + sizeof(arr) / sizeof(arr[0]) );

	cout << "Calling algorithm" << endl;
	// Now I just call the algorithm here.
	int maxSub = AlGore3(arr, 0, (n - 1)); 

	cout << "maxSub is: " << maxSub << endl;

	return 0;
}


/***********************************************************************************
 *                         Algorithm 3: Divide and Conquer
 * This algorithm will split the array into halves recursively and recombine.
 * We know the maximum subarray will be in the first half, the second half, or made
 * from the end of the first half and the beginning of the second half.
 ***********************************************************************************/
int AlGore3(int arr[], int start, int end) {
	if(start == end)	// for tiny array of size 1
		return arr[start];
	else {			// anything bigger gets broken down
		int mid = (start + end) / 2;

		int leftMax = AlGore3(arr, start, mid);
		int rightMax = AlGore3(arr, mid + 1, end);
		
		int suffix = MaxSuffix(arr, start, mid);
		int prefix = MaxPrefix(arr, mid + 1, end);
		
		// Return whichever has the highest value
		if (leftMax >= rightMax && leftMax >= (suffix + prefix))
			return leftMax;
		else if (rightMax >= leftMax && rightMax >= (suffix + prefix))
			return rightMax;
		else
			return (suffix + prefix);
	}
}


/*******************************************************************************
 * Helper functions for AlGore3, which will compute maximum prefix and suffixes
 *******************************************************************************/

// Finds the rightmost maximum sum
int MaxSuffix(int arr[], int start, int end) {
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
int MaxPrefix(int arr[], int start, int end) {
	int maxSum = arr[start];
	int sum = 0;
	for (int j = start; j <= end; j++) {
		sum = sum + arr[j];
		if (sum > maxSum)
			maxSum = sum;
	}
	return maxSum;
}
