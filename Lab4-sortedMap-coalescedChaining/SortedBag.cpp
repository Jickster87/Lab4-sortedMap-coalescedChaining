#include "SortedBag.h"
#include <iostream>
#include "SortedBagIterator.h"
using namespace std;

SortedBag::SortedBag(Relation r) {
    maxCapacity = 31;
    sizeBag = 0;
    firstEmpty = 0;
    rel = r;
    //alphaLoad = 0.0f;
    
    Table = new cNode[maxCapacity];
    for (int i = 0; i < maxCapacity; i++) {
        Table[i].aPair = NULL_TPAIR;
        Table[i].next = -1;
    }
}
//
//void SortedBag::updateAlpha(){
//    alphaLoad = static_cast<float>(sizeBag) / maxCapacity;
//}

void SortedBag::updateFirstEmpty(){
    firstEmpty++;
    while (firstEmpty < maxCapacity && Table[firstEmpty].aPair != NULL_TPAIR) {
        firstEmpty++;
    }
}

int SortedBag::h(TComp elem) const {
    return abs(elem) % maxCapacity;
}

void SortedBag::resizeRehash() {
    // Double the capacity
    int oldCapacity = maxCapacity;
    maxCapacity *= 2;

    //point to a new table array of type chnode with maxcap
    cNode * newTable = new cNode[maxCapacity];
    // Initialize a new table
    for (int i = 0; i < maxCapacity; i++) {
        //assign to all Tcomps - null t pair and next pointer in to -1
        newTable[i].aPair = NULL_TPAIR;
        newTable[i].next = -1;
    }

    // Rehash all elements into the new table
    for (int i = 0; i < oldCapacity; i++) {
        if (Table[i].aPair != NULL_TPAIR) {
            int pos = abs(Table[i].aPair.first) % maxCapacity; // New hash position
            if (newTable[pos].aPair == NULL_TPAIR) {
                // Directly place in the bucket if empty
                newTable[pos].aPair = Table[i].aPair;
                newTable[pos].next = -1;
            } else {
                // Collision handling: find the end of the chain
                int current = pos;
                //while it's not -1 meaning not end of chain move to next node
                while (newTable[current].next != -1) {
                    current = newTable[current].next;
                }
                // Place the element in the first available empty slot
                
                ///gpt improvement?s
                if (firstEmpty < maxCapacity) {
                    newTable[firstEmpty].aPair = Table[i].aPair;
                    newTable[firstEmpty].next = -1;
                    newTable[current].next = firstEmpty;
                    updateFirstEmpty();  // Update the firstEmpty pointer
                }
                ///gpt
//                for (int j = 0; j < maxCapacity; j++) {
//                    if (newTable[j].aPair == NULL_TPAIR) {
//                        newTable[j].aPair = Table[i].aPair;
//                        newTable[j].next = -1;
//                        newTable[current].next = j;
//                        break;
//                    }
//                }
                ///end-gpt
                
            }
        }
    }

    // Clean up old table and set the new table
    delete[] Table;
    Table = newTable;

    // Update firstEmpty
    firstEmpty = 0;
    updateFirstEmpty();
}

void SortedBag::add(TComp e) {
    std::cout << "Adding element: " << e << std::endl;
    // Resize if the table is full
    if (firstEmpty == maxCapacity) {
        resizeRehash();
    }

    // Hash position to get a bucket
    int pos = h(e);

    // Check if the element already exists in the chain
    int current = pos;
    while (current != -1) {
        if (Table[current].aPair.first == e) {
            // Increment frequency and return if found
            Table[current].aPair.second++;
            sizeBag++;
            return;
        }
        current = Table[current].next;
    }

    // If the bucket is empty, place the element directly
    if (Table[pos].aPair == NULL_TPAIR) {
        Table[pos].aPair.first = e;
        Table[pos].aPair.second = 1;
        if (pos == firstEmpty) {
            updateFirstEmpty();
        }
    } else {
        // If the bucket is occupied, traverse to the end of the chain
        int prev = -1;
        current = pos;
        //FORGOT REL HERE !
        while (current != -1 && rel(Table[current].aPair.first, e)) {
                 prev = current;
                 current = Table[current].next;
             }


        // Insert the element at the first available empty slot
        Table[firstEmpty].aPair.first = e;
        Table[firstEmpty].aPair.second = 1;
        Table[firstEmpty].next = -1;
        Table[current].next = firstEmpty;

        // Update firstEmpty
        updateFirstEmpty();
    }

    // Increment the size
    sizeBag++;
    std::cout << "Updated sizeBag: " << sizeBag << std::endl;
}


bool SortedBag::remove(TComp e) {
    std::cout << "Removing element: " << e << std::endl;
	//navigate with current and prev
    int pos = h(e);
    int prevPos = -1;
    //move while elem not found
    while (pos != -1 && Table[pos].aPair.first != e) {
        prevPos = pos;
        pos = Table[pos].next;
    }
    
    //if pos -1 didnt find the elem
    if (pos == -1) {
        return false;
    }
    
    if (Table[pos].aPair.first > 1) {
        Table[pos].aPair.second--;
        sizeBag--;
        return true;
    }
    
    bool over = false;
    do {
        //get new traveler int's (SAME ARRAY) NEW INT POINTERS
        //old prev, old current, old next
        //  [-][op][oc][on][-][-][-][-] TABLE
        //          /  /
        //    [-][np][nc][nn][-][-][-][-] TABLE
        //new prev, new current, new next
        int xprevPos = pos;
        int xPos = Table[pos].next;
        // hwhhaa??
        //so... while it's not the end of the "chain" and hash it's not first hash [index]
        //move forward
        while (xPos != -1 && h(Table[xPos].aPair.first) != pos) {
            xprevPos = xPos;
            xPos = Table[xPos].next;
        }
        //if that remains -1 , spot open to remove?
        if (xPos == -1) {
            over = true;
        } else {
            Table[pos].aPair = Table[xPos].aPair;
            prevPos = xprevPos;
            pos = xPos;
        }
        
    } while (!over);
    
    if (prevPos != -1) {
        Table[prevPos].next = Table[pos].next;
    }
    Table[pos].aPair = NULL_TPAIR;
    Table[pos].next = -1;
    
    if (pos < firstEmpty) {
        firstEmpty = pos;
    }
    
    sizeBag--;
    std::cout << "Removing element: " << e << std::endl;
	return false;
    
}


int SortedBag::computeActualSize() const {
    int countz = 0;
    for (int i = 0; i < maxCapacity; i++) {
        if (Table[i].aPair != NULL_TPAIR) {
            countz += Table[i].aPair.second;
            std::cout << "Element: " << Table[i].aPair.first << ", Frequency: " << Table[i].aPair.second << std::endl;
        }
    }
    return countz;
}

bool SortedBag::search(TComp elem) const {
    int newPos = h(elem);
    while (newPos != -1) {
        if (Table[newPos].aPair.first == elem) {
            return true;
        }
        newPos = Table[newPos].next;
    }
	return false;
}


int SortedBag::nrOccurrences(TComp elem) const {
    int newPos = h(elem);
    while (newPos != -1) {
        if (Table[newPos].aPair.first == elem) {
            return Table[newPos].aPair.second;
        }
        newPos = Table[newPos].next;
    }
	return 0;
}


int SortedBag::size() const {
    int count = 0;
    for (int i = 0; i < maxCapacity; i++) {
        if (Table[i].aPair != NULL_TPAIR) {
            count += Table[i].aPair.second;
        }
    }
    std::cout << "Actual size computed: " << count << ", sizeBag: " << sizeBag << std::endl;
    return sizeBag;
}


bool SortedBag::isEmpty() const {
    return sizeBag == 0;
}


SortedBagIterator SortedBag::iterator() const {
	return SortedBagIterator(*this);
}


SortedBag::~SortedBag() {
    delete [] Table;
}
