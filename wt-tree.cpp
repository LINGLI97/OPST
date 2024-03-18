#include <vector>
#include <algorithm>
#include <iostream>

// Define a type alias for convenience
//using Index = int;

int successorNV(const std::vector<int>& w, int a, int b) {
    if (w.empty()) return -1; // Handle empty vector case

    int aimElement = w[b + 1];
    int successorIndex = -1; // Default to "not found"

    // Find the rightmost occurrence of the smallest element greater than or equal to the aimelement
    for (int j = a; j < b+1 ; ++j) {
        if (w[j] >= aimElement) {
            if (successorIndex == -1 || w[j] <= w[successorIndex]) {
                successorIndex = j;
            }
        }
    }


    return successorIndex;
}

int main() {
    // Example usage
    std::vector<int> w = {1, 0, 1, 3, 3, 1, 4, 2};
    int u = 6;
    int a = 2;
    int b = u -1;
    int successorIndex = successorNV(w,a,b);

    std::cout << "The index of the successor is: " << successorIndex << std::endl;
    return 0;
}
