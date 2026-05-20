/*
 * The greatest common divisor (gcd) of two integers is the largest integer that divides 
 * both. Thus, gcd(50, 15) = 5.
 */

long long gcd(long long m, long long n) 
{
    while (n != 0) {
        long long rem = m % n;
        m = n;
        n = rem;
    }

    return m;
}
