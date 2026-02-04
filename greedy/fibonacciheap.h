/*

Author: Louis Ye
Date : December 2013

*/


#ifndef FIBONACCIHEAP_H
#define FIBONACCIHEAP_H

#include <iostream>
#include <math.h>


//------------------------------------------------------------------------------------------
//------------------------------------ HEAP ------------------------------------------------
template<class T>
class FibonacciHeap {
public:

	//--------------------------------------------------------------------------------------
	//------------------------------------ NODE --------------------------------------------
	class FibNode {
	public:
		T getKey() { return key; }
		void setKey(T k) { key = k; }
		bool compare(FibNode * other) {
    //        if((other->getKey())==NULL) return true;
     //       if((*other->getKey()).empty()) return true;
			return -(*key).size() < -(*(other->getKey())).size();
		}

		int getID() { return id; }
		void setID(int i) { id = i; }

		FibNode * getParent() { return parent; }
		void setParent(FibNode * p) { parent = p; }

		FibNode * getLeftSib() { return leftSib; }
		void setLeftSib(FibNode * p) { leftSib = p; }

		FibNode * getRightSib() { return rightSib; }
		void setRightSib(FibNode * p) { rightSib = p; }

		FibNode * getChild() { return child; }
		void setChild(FibNode * p) { child = p; }

		void mark() { flag = true; }
		void unmark() { flag = false; }
		bool isMark() { return flag; }

		int getRank() { return rank; }
		void incRank() { rank += 1; }
		void decRank() { rank -= 1; }

		FibNode (T k, int i) {
			key = k;
			parent = NULL;
			leftSib = NULL;
			rightSib = NULL;
			child = NULL;
			flag = false;
			rank = 0;
			id = i;
		}

	private:
		T key;
		FibNode * parent;
		FibNode * leftSib;
		FibNode * rightSib;
		FibNode * child;
		bool flag;
		int rank;
		int id;

	};
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------

	bool empty() {
		return rootEntry == NULL;
	}


	//.........................................................................
	FibNode * topNode() {
		return minNode;
	}


	//.........................................................................
	T top() {
		return minNode->getKey();
	}


	//.........................................................................
	void pop() {
		if (empty()) {
            heapSize = 0;
            return;
        }
        std::cout << "pop" << std::endl;

		FibNode * cur = minNode->getChild();
		FibNode * next = NULL;
		while (cur != NULL) {
			next = cur->getRightSib();
			addToRootList(cur);
			cur = next;
		}


		deleteFromRootList(minNode);
		delete minNode;
		minNode = rootEntry;
		heapSize -= 1;
        std::cout << "heapsize: " << heapSize << std::endl;
		if (rootEntry == NULL) return; //If the heap is already empty
		int mRank = (int)log2((double)heapSize) + 1;
		FibNode * ranks[mRank];
		for (int i=0; i<mRank; i++) ranks[i] = NULL;
		cur = rootEntry;
		next = cur->getRightSib();
		while (cur != NULL) {

			int curRank = cur->getRank(); //Record the rank before merge
			if (ranks[curRank] != NULL) {
				cur = merge(cur, ranks[curRank]);
				ranks[curRank] = NULL;
			}
			if (cur->compare(minNode)) minNode = cur; //Update min node
			if (ranks[cur->getRank()] != NULL) continue; //If there is tree to be merged at the new rank, continue

			ranks[cur->getRank()] = cur;
			cur = next;
			if (cur != NULL) next = cur->getRightSib();
		}
	}


	//.........................................................................
	FibNode * push(T k) {
		if (empty()) {
			rootEntry = new FibNode(k, totalNum++);
			minNode = rootEntry;
			heapSize = 1;
		}
		else {
			addToRootList(new FibNode(k, totalNum++));
			heapSize += 1;
		}
		return rootEntry;
	}

	FibNode * push(T k, int id) {
		if (empty()) {
			rootEntry = new FibNode(k, id);
			minNode = rootEntry;
			heapSize = 1;
		}
		else {
			addToRootList(new FibNode(k, id));
			heapSize += 1;
		}
		totalNum++;
		return rootEntry;
	}


	//.........................................................................
	int size() {
		return heapSize;
	}


	//.........................................................................
	void decreaseKey(FibNode * nd, T k) {
		if (empty()) return;

		//Judge if the new key is actually larger than the previous one
		FibNode tmp(k, -1);
		if (nd->compare(&tmp)) return; //If yes, cancel the decreaseKey operation

		//keep doing the decreaseKey operation
		nd->setKey(k);
		if (nd->compare(minNode)) minNode = nd;
		if (nd->getParent() != NULL) {
			FibNode * par = nd->getParent();
			if (nd->compare(par)) { //If nd is smaller than the parent
				deleteFromParent(nd, par);
			}
		}
	}


	void printHeap() { //For testing

		FibNode * allnodes[heapSize];
		FibNode * cur = rootEntry;
		FibNode * next = NULL;
		int f=0, s=0;

		printf("---------------------heapSize=%d-------------------------------\n", heapSize);
		printf("minNode:(key=%d, id=%d)\n", minNode->getKey(), minNode->getID());

		while (cur != NULL) {
			next = cur->getRightSib();
			allnodes[s++] = cur;
			printf("#########\nroot : (key=%d, id=%d, rank=%d)\n", cur->getKey(), cur->getID(), cur->getRank());
			while (f < s) {
				FibNode * father = allnodes[f];
				FibNode * son = father->getChild();
				while (son != NULL) {
					printf("father:(key=%d, id=%d, rank=%d), son:(key=%d, id=%d, rank=%d)\n",father->getKey(), father->getID(), father->getRank(), son->getKey(), son->getID(), son->getRank());
					allnodes[s++] = son;
					son = son->getRightSib();
				}
				printf("\n");
				f++;
			}
			cur = next;
		}
	}


	//.........................................................................
	FibonacciHeap() {
		rootEntry = NULL;
		minNode = NULL;
		heapSize = 0;
		totalNum = 0;
	}

	~FibonacciHeap() {
		FibNode * allnodes[heapSize];
		FibNode * cur = rootEntry;
		FibNode * next = NULL;
		int f=0, s=0;

		while (cur != NULL) {
			next = cur->getRightSib(); 
			allnodes[s++] = cur;
			while (f < s) {			//Do DFS for each tree
				FibNode * father = allnodes[f];
				FibNode * son = father->getChild();
				while (son != NULL) {
					allnodes[s++] = son;
					son = son->getRightSib();
				}
				f++;
			}
			cur = next;
		}

//		for (int i=0; i<heapSize; i++) delete allnodes[i]; //Deleting all the FibNodes
	}

	int static const MAX_RANK = 32;

private:
	FibNode * rootEntry;
	FibNode * minNode;
	int heapSize;
	int totalNum;

	void addToRootList(FibNode * nd) {
		nd->setLeftSib(NULL);
		nd->setRightSib(rootEntry);
		rootEntry->setLeftSib(nd);
		rootEntry = nd;
		if ( rootEntry->compare(minNode) ) minNode = rootEntry;
	}

	void deleteFromRootList(FibNode * nd) {
		FibNode * left = nd->getLeftSib();
		FibNode * right = nd->getRightSib();
		if (rootEntry == nd) rootEntry = right;
		if (left != NULL) left->setRightSib(right);
		if (right != NULL) right->setLeftSib(left);
	}

	void deleteFromParent(FibNode * nd, FibNode * par) {
		nd->setParent(NULL);
		par->decRank();
		FibNode * left = nd->getLeftSib();
		FibNode * right = nd->getRightSib();

		if (par->getChild() == nd) par->setChild(right); //nd is the first child
		if (left != NULL) left->setRightSib(right);
		if (right != NULL) right->setLeftSib(left);

		addToRootList(nd);

		if (par->isMark()) {
			FibNode * grandPar = par->getParent();
			par->unmark();
			if (grandPar != NULL) deleteFromParent(par, grandPar);
		}
		else par->mark();
	}

	FibNode * merge(FibNode * a, FibNode * b) {
		FibNode * big = a->compare(b) ? b : a;
		FibNode * sma = a->compare(b) ? a : b;
		deleteFromRootList(big);
		FibNode * child = sma->getChild();
		sma->setChild(big);
		sma->incRank();
		big->setParent(sma);
		big->setRightSib(child);
		if (child != NULL) child->setLeftSib(big);
		return sma;
	}

};
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

#endif
