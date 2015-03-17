
/* This C++ project implements a random number generator which outputs a 
   unique 32-bit integer each time it's called, and verifies that the 
   first 232 integers returns really are unique.

   taken from here: https://github.com/preshing/RandomSequence */

/*
    unsigned int seed = (unsigned int) time(NULL);
    RandomSequenceOfUnique rsu(seed, seed + 1);
    unsigned int i = 0;
    do {
        printf("0x%08x\n", rsu.next());
    }
    while (++i != 0);
*/

#ifndef __RANDOM_SEQUENCE_H__
#define __RANDOM_SEQUENCE_H__

class RandomSequenceOfUnique {
private:
    unsigned int m_index;
    unsigned int m_intermediateOffset;

    static unsigned int permuteQPR(unsigned int x) {
        static const unsigned int prime = 4294967291u;
        if (x >= prime)
            return x;  // The 5 integers out of range are mapped to themselves.
        unsigned int residue = ((unsigned long long) x * x) % prime;
        return (x <= prime / 2) ? residue : prime - residue;
    }

public:
    RandomSequenceOfUnique(unsigned int seedBase, unsigned int seedOffset) {
        m_index = permuteQPR(permuteQPR(seedBase) + 0x682f0161);
        m_intermediateOffset = permuteQPR(permuteQPR(seedOffset) + 0x46790905);
    }

    unsigned int next() {
        return permuteQPR((permuteQPR(m_index++) + m_intermediateOffset) ^ 0x5bf03635);
    }
};


#endif // __RANDOM_SEQUENCE_H__
