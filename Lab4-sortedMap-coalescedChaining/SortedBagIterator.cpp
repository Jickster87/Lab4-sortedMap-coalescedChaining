#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
    currentPosition = 0;
    currentFrequency = 1;
    
    while (currentPosition < bag.maxCapacity && bag.Table[currentPosition].aPair == NULL_TPAIR) {
        currentPosition++;
    }
}

TComp SortedBagIterator::getCurrent() {
    if (!valid()) {
        throw exception();
    }
    // Return element from Pair(elem, freq)
    return bag.Table[currentPosition].aPair.first;
}

bool SortedBagIterator::valid() {
    return currentPosition < bag.maxCapacity && bag.Table[currentPosition].aPair != NULL_TPAIR;
}

void SortedBagIterator::next() {
    if (!valid()) {
        throw exception();
    }
    currentFrequency++;
    
    if (currentFrequency > bag.Table[currentPosition].aPair.second) {
        currentPosition++;
        currentFrequency = 1;
        
        while (currentPosition < bag.maxCapacity && bag.Table[currentPosition].aPair == NULL_TPAIR) {
            currentPosition++;
        }
    }
}

void SortedBagIterator::first() {
    currentPosition = 0;
    currentFrequency = 1;
    
    while (currentPosition < bag.maxCapacity && bag.Table[currentPosition].aPair == NULL_TPAIR) {
        currentPosition++;
    }
}
