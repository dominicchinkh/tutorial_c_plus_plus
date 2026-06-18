#pragma once

// Helper function to check if a number is prime
bool isPrime(int num)
{
    if (num <= 1) {
        return false;
    }

    // 2 and 3 are prime
    if (num <= 3) {
        return true;
    }

    // Eliminate multiples of 2 and 3
    if (num % 2 == 0 || num % 3 == 0) {
        return false;
    }

    /*
     * Check factors up to the square root of num. All primes greater than 3 are of the form 6k ± 1
     *
     * 1. The Square Root Limit
     *    If a number is composite (not prime), it can be factored into two numbers: a x b = num.
     *    Mathematically, both a and b cannot be greater than the square root of num. For a number
     *    like 1,000,000, we only need to check up to 1,000.
     *
     * 2. The 6k +/- 1 Rule 
     *    Every single integer on earth can be expressed in relation to a multiple of 6 (let's call 
     *    the multiple 6k)
     *
     *      6k, 6k+1, 6k+2, 6k+3, 6k+4, 6k+5 (the same as 6k-1),
     *
     *    Because our main function already filtered out numbers divisible by 2 and 3 at the very 
     *    beginning, we know our prime candidate cannot have factors that are multiples of 2 or 3. 
     *    Therefore, the only potential factors we need to test are numbers that land on 6k - 1 or 
     *    6k + 1. 
     *
     * The loop starts at i = 5, as 5 is the first prime after 2 and 3, and it represents 6(1) - 1.
     *
     * Inside the loop, it checks two numbers at a time:
     *    i: This represents 6k - 1 (Starts at 5)
     *    i + 2: This represents 6k + 1 (Starts at 7)
     *
     */
        
    for (int i = 5; i*i < num; i+=6) {
        if (num % i == 0 || num % (i+ 2) == 0) {
            return false;
        }
    }

    return true;
}

// Function to find the smallest prime number strictly greater than n
int nextPrime(int num)
{
    // If n is less than 2, the next prime is 2
    if (num < 2) {
        return 2;
    }

    int prime = num;
    bool found = false;

    // Loop until a prime number is found
    while (!found) {
        ++prime;
        if (isPrime(prime)) {
            found = true;
        }
    }

    return prime;
}
