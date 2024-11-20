#pragma once
#include <utility>
//DO NOT INCLUDE SORTEDBAGITERATOR

//DO NOT CHANGE THIS PART
typedef int TComp;
typedef TComp TElem;
typedef bool(*Relation)(TComp, TComp);
#define NULL_TCOMP -11111;

typedef std::pair<TComp, TElem> TPair;
#define NULL_TVALUE -111111
#define NULL_TPAIR pair<TComp, TElem>(-111111, -111111)

class SortedBagIterator;

class SortedBag {
	friend class SortedBagIterator;

private:
    struct cNode {
        TPair aPair;
        int next;
    };
    //array stuff
    int maxCapacity;
    int sizeBag;
    //float alphaLoad;
    int firstEmpty;
    Relation rel;
    //hash func
    int h(TComp) const;
    //array address
    cNode * Table;
    
public:
	//stuff
    int computeActualSize() const;
    void updateAlpha();
    void resizeRehash();
    void updateFirstEmpty();
    
    //constructor
	SortedBag(Relation r);

	//adds an element to the sorted bag
	void add(TComp e);

	//removes one occurence of an element from a sorted bag
	//returns true if an eleent was removed, false otherwise (if e was not part of the sorted bag)
	bool remove(TComp e);

	//checks if an element appearch is the sorted bag
	bool search(TComp e) const;

	//returns the number of occurrences for an element in the sorted bag
	int nrOccurrences(TComp e) const;

	//returns the number of elements from the sorted bag
	int size() const;

	//returns an iterator for this sorted bag
	SortedBagIterator iterator() const;

	//checks if the sorted bag is empty
	bool isEmpty() const;

	//destructor
	~SortedBag();
};
