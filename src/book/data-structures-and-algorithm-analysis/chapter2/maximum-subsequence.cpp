#include "maximum-subsequence.h"

#include <iostream>

/**
 * ==========================================================================================
 * 📝 PROBLEM DEFINITION: MAXIMUM SUBSEQUENCE SUM (MAXIMUM SUBARRAY SUM)
 * ==========================================================================================
 * 
 * DESCRIPTION:
 * Given a sequence of (possibly negative) integers A1, A2, . . . , AN, find the maximum 
 * value of the sum of a contiguous subsequence:
 * 
 *                     j
 *                    ===
 *                    \     A_k
 *                    /
 *                    ===
 *                    k=i
 * 
 * CONSTRAINTS & EDGE CASES:
 * - The elements must be contiguous (adjacent to one another in the sequence).
 * - For convenience, if all integers in the sequence are negative, the maximum 
 *   subsequence sum is defined to be 0 (equivalent to choosing an empty subsequence).
 * 
 * EXAMPLE:
 * - Input Sequence:  [-2, 11, -4, 13, -5, -2]
 * - Best Subsequence: [11, -4, 13] (Elements A2 through A4)
 * - Maximum Sum:      11 + (-4) + 13 = 20
 * 
 * ==========================================================================================
 */

// vector<int> a = {-2, 11, -4, 13, -5, -2};
// cout << "Max Sub Sum 1: " << maxSubSum1(a) << endl;
// cout << "Max Sub Sum 2: " << maxSubSum2(a) << endl;
// cout << "Max Sub Sum 3: " << maxSubSum3(a) << endl;
// cout << "Max Sub Sum 4: " << maxSubSum4(a) << endl;

int maxSubSum1(const vector<int> & a)
{
    int maxSum = 0;

    for (int i = 0; i< a.size(); i++) {
        for (int j = i; j < a.size(); j++) {
            int thisSum = 0;

            for (int k = i; k <=j; k++) {
                thisSum += a[k];
            }

            if (thisSum > maxSum) {
                maxSum = thisSum;
            }
        }
    }

    return maxSum;
}

int maxSubSum2(const vector<int> & a)
{
    int maxSum = 0;

    for (int i = 0; i< a.size(); i++) {

        int thisSum = 0;

        for (int j = i; j < a.size(); j++) {

            // Note than in `maxSubSum1` (line 15-17), 
            //   Sum(i -> j) = a[j] + Sum(i -> j-1)
            thisSum += a[j];

            if (thisSum > maxSum) {
                maxSum = thisSum;
            }
        }
    }

    return maxSum;
}

/**
 * ==========================================================================================
 * 🚀 DIVIDE AND CONQUER APPROACH
 * ==========================================================================================
 * 
 * PURPOSE:
 * This code implements an efficient O(N log N) Divide and Conquer algorithm to find 
 * the maximum contiguous subarray sum within a given vector of integers. If all 
 * numbers are negative, the algorithm treats an empty subarray as valid and returns 0.
 * 
 * HOW THE ALGORITHM WORKS:
 * The problem is solved recursively by splitting the array into two halves at the center.
 * According to the Divide and Conquer strategy, the maximum subarray sum can only exist 
 * in one of three possible locations:
 * 
 * 1. Entirely in the LEFT half of the array.
 * 2. Entirely in the RIGHT half of the array.
 * 3. Straddling the CENTER boundary (crossing from the left half into the right half).
 * 
 * ------------------------------------------------------------------------------------------
 * FUNCTION BREAKDOWN:
 * 
 * 1. max3(int, int, int)
 *    - A helper utility that quickly determines and returns the maximum of three integers.
 *    - Uses nested ternary operators for optimal performance.
 * 
 * 2. maxSumRec(const vector<int> & a, int left, int right)
 *    - The core recursive engine of the algorithm.
 *    - BASE CASE: If 'left == right' (a single-element subarray), it returns the element 
 *      if it is positive, or 0 if it is negative.
 *    - DIVIDE: Calculates the midpoint ('center') and recursively finds the maximum 
 *      subarray sums for the left and right halves ('maxLeftSum' and 'maxRightSum').
 *    - CONQUER (Border Sums): Since a contiguous sum can cross the middle, it calculates:
 *        a) 'maxLeftBorderSum': The max sum starting at 'center' and extending left.
 *        b) 'maxRightBorderSum': The max sum starting at 'center + 1' and extending right.
 *      Adding these two together gives the maximum sum that crosses the boundary.
 *    - COMBINE: Uses 'max3' to return the largest of the Left Sum, Right Sum, or Crossed Sum.
 * 
 * 3. maxSubSum3(const vector<int> & a)
 *    - The public wrapper function. It simplifies the API for the user by abstracting 
 *      away the initial tracking indices, automatically passing 0 and (size - 1) 
 *      into the recursive function.
 * 
 * ------------------------------------------------------------------------------------------
 * COMPLEXITY ANALYSIS:
 * - Time Complexity:  O(N log N) - The array is split in half at each step (log N layers), 
 *                     and at each layer, a linear scan O(N) is performed to find border sums.
 * - Space Complexity: O(log N) - Due to the memory allocation on the recursion stack.
 * ==========================================================================================
 */

int max3(int num1, int num2, int num3)
{
    if (num1 > num2) {
        return (num1 > num3)? num1 : num3;
    } else {
        return (num2 > num3)? num2 : num3;
    }
}

int maxSumRec(const vector<int> & a, int left, int right)
{
    // Base case
    if (left == right) {
        return (a[left] > 0)? a[left] : 0;
    }

    int center = (left + right) / 2;
    int maxLeftSum  = maxSumRec(a, left, center);
    int maxRightSum = maxSumRec(a, center + 1, right);

    int maxLeftBorderSum = 0, leftBorderSum = 0;
    for (int i = center; i >= left; i--) {
        leftBorderSum += a[i];
        if (leftBorderSum > maxLeftBorderSum) {
            maxLeftBorderSum = leftBorderSum;
        }
    }

    int maxRightBorderSum = 0, rightBorderSum = 0;
    for (int i = center + 1; i <= right; i++) {
        rightBorderSum += a[i];
        if (rightBorderSum > maxRightBorderSum) {
            maxRightBorderSum = rightBorderSum;
        }
    }

    return max3(maxLeftSum, maxRightSum, maxLeftBorderSum + maxRightBorderSum);
}

int maxSubSum3(const vector<int> & a)
{
    return maxSumRec(a, 0, a.size() - 1);
}

/**
 * ==========================================================================================
 * 🚀 KADANE'S ALGORITHM - LINEAR APPROACH
 * ==========================================================================================
 * 
 * PURPOSE:
 * This function solves the Maximum Subarray Sum problem using a highly optimized, 
 * single-pass greedy approach known as Kadane's Algorithm. It finds the maximum 
 * contiguous subarray sum within a given vector of integers in a single sweep.
 * 
 * CORE LOGIC (THE "RESET" PRINCIPLE):
 * The algorithm iterates through the array, accumulating a running total ('thisSum').
 * The breakthrough insight here is simple: 
 * If 'thisSum' ever drops below 0, it becomes a liability. Any future subarray sum 
 * would only be dragged down by keeping this negative accumulation. Therefore, the moment 
 * 'thisSum' becomes negative, we discard it and reset it to 0, effectively starting 
 * a brand new candidate subarray from the next element.
 * 
 * ------------------------------------------------------------------------------------------
 * STEP-BY-STEP EXECUTION:
 * 1. Initialize 'maxSum' (tracks the overall best sum seen so far) and 'thisSum' 
 *    (tracks the current rolling subarray sum) both to 0.
 * 2. Loop through each element 'a[i]' in the vector:
 *    - Add the current element to 'thisSum'.
 *    - If 'thisSum' beats our all-time high ('maxSum'), update 'maxSum'.
 *    - If 'thisSum' drops below 0, reset 'thisSum' to 0 to cut our losses.
 * 3. Return 'maxSum'. If the array contains only negative numbers (or is empty), 
 *    it naturally returns 0 (representing an empty subarray).
 * 
 * ------------------------------------------------------------------------------------------
 * COMPLEXITY ANALYSIS:
 * - Time Complexity:  O(N) - Excellent. It inspects each element exactly once in a single 
 *                     'for' loop, making it significantly faster than the O(N log N) 
 *                     Divide and Conquer approach.
 * - Space Complexity: O(1) - Optimal. It only uses two integer variables ('maxSum' and 
 *                     'thisSum'), requiring no extra memory regardless of input size.
 * ==========================================================================================
 */

/*
 * 1. One observation is that if a[i] is negative, then it cannot possibly be the start of the optimal subsequence, 
 *    since any subsequence that begins by including a[i] would be improved by beginning with a[i+1]. 
 * 
 * 3. Similarly, any negative subsequence cannot possibly be a prefix of the optimal subsequence (same logic).
 * 
 *    If, in the inner loop, we detect that the subsequence from a[i] to a[j] is negative, then we can advance it  
 *    all the way to j+1. To see this, let p be any index between i+1 and j. Any subsequence that starts at index
 *    p is not larger than the corresponding subsequence that starts at index i and includes the subsequence from 
 *    a[i] to a[p-1], since the latter subsequence is not negative (j is the first index that causes the 
 *    subsequence starting at index i to become negative).
 */

int maxSubSum4(const vector<int> & a)
{
    int maxSum = 0, thisSum = 0;

    for (int i = 0; i < a.size(); i++) {
        thisSum += a[i];

        if (thisSum > maxSum) {
            maxSum = thisSum;
        } 
        else if (thisSum < 0) {
            thisSum = 0;
        }
    }

    return maxSum;
}
