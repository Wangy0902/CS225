/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
		if(first[curDim] < second[curDim]){
			return true;
		}else if(first[curDim] == second[curDim]){
			return first < second;
		}else{
			return false;
		}
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential) const
{
    /**
     * @todo Implement this function!
     */
		double distance_tc = 0;
		double distance_tp = 0;
		for(unsigned i = 0; i < Dim; ++i){
			distance_tc += (target[i] - currentBest[i]) * (target[i] - currentBest[i]);
			distance_tp += (target[i] - potential[i]) * (target[i] - potential[i]);
		}
		if(distance_tp < distance_tc){
			return true;
		}else if(distance_tp == distance_tc){
			return potential < currentBest;
		}else{
			return false;
		}
}


// quickselect:-------------------------------------------------------------------------------------------------------
// 1. partition( vector<Points<Dim>> &newPoints, left(0), right(newPoints.size()), pivotIndex(newPoints.size()) );
template <int Dim>
int KDTree<Dim>::partition(vector<Point<Dim>>& list, unsigned left, unsigned right, unsigned pivotIndex, int curDim){
	Point<Dim> pivotValue = list[pivotIndex];
	swap(list[pivotIndex], list[right]);	// Move pivot to end
	int storeIndex = left;
	for(unsigned i = left; i < right; ++i){
		if(smallerDimVal(list[i], pivotValue, curDim)){
			swap(list[storeIndex], list[i]);
			storeIndex++;
		}
	}
	swap(list[right], list[storeIndex]);	// Move pivot to its final place
	return storeIndex;
}

// 2. select( vector<Points<Dim>>& newPoints, left, right, k(kth smallest elements))
template <int Dim>
Point<Dim> KDTree<Dim>::select(vector<Point<Dim>>& list, unsigned left, unsigned right, unsigned k, int curDim){
	if(left == right){return list[left];}
	unsigned pivotIndex = (left + right) / 2;	// <-------------------------------------- PivotPoint chose here
	pivotIndex = partition(list,left,right,pivotIndex,curDim);
	if(k == pivotIndex){
		return list[k];
	}else if(k < pivotIndex){
		return select(list,left,pivotIndex - 1,k,curDim);
	}else{
		return select(list,pivotIndex + 1,right,k,curDim);
	}
}

// 3. swap(Points<Dim>& first, Points<Dim>& second)
template <int Dim>
void KDTree<Dim>::swap(Point<Dim>& first, Point<Dim>& second){
	Point<Dim> temp = first;
	first = second;
	second = temp;
}
// -------------------------------------------------------------------------------------------------------------------

// Recursive function help to build the tree
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::recursiveBuild(vector<Point<Dim>>& list, unsigned left, unsigned right, int dim){
	// build New Node
	if(list.empty() || right>=list.size() || left>=list.size()){return NULL;}
	if(left > right){return NULL;}
	KDTreeNode* newNode = new KDTreeNode(select(list, left, right, (left + right) / 2, dim));
	size++;
	dim++;
	newNode->left = recursiveBuild(list, left, (left + right)/2 - 1, dim%Dim);
	newNode->right = recursiveBuild(list, (left + right) / 2 + 1, right, dim%Dim);
	return newNode;

}



template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
  /**
  * @todo Implement this function!
  */
	if(newPoints.empty()){
		root = NULL;
		size = 0;
	}else{
		// copy the vector
		vector<Point<Dim>> list = newPoints;
		size = 0;
		root = recursiveBuild(list, 0, list.size() - 1, 0);
		// this->printTree(cout);
	}
}






template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */
	copy__(root, other.root);
	size = other.size;
}


template <int Dim>
void KDTree<Dim>::copy__(KDTreeNode*& root, KDTreeNode*& other) {
  /**
   * @todo Implement this function!
   */
	if(!other){return;}
	root = new KDTreeNode(other->point);
	// root->point = other->point;
	copy__(root->left, other->left);
	copy__(root->right, other->right);
}



template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
	if(this == &rhs){return *this;}
	delete__(root);
	copy__(root, rhs.root);
	size = rhs.size;
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
	delete__(root);
}

template <int Dim>
void KDTree<Dim>::delete__(KDTreeNode* subroot){
	if(subroot == NULL) {return;}
	else{
		delete__(subroot->left);
		delete__(subroot->right);
		delete subroot;
		subroot = NULL;
	}
}











template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
		Point<Dim> currBest = root->point;
		Point<Dim> target = query;
		recursiveFind(root, target, currBest, 0);
    return currBest;
}

template <int Dim>
void KDTree<Dim>::recursiveFind(KDTreeNode* curr, Point<Dim>& query, Point<Dim> & currBest, int dim) const{
	// traversal base case
	if(!curr){return;}
	bool goLeft = true;

	if(smallerDimVal(curr->point, query, dim)){
		goLeft = false;
		recursiveFind(curr->right, query, currBest, (dim+1)%Dim);
	}else{
		recursiveFind(curr->left, query, currBest, (dim+1)%Dim);
	}

	if(shouldReplace(query, currBest, curr->point)){
		currBest = curr->point;
	}
	// currBest = (shouldReplace(query, currBest, curr->point)) ? curr->point : currBest = currBest;

	double r = 0;
	for(int i = 0; i < Dim; ++i){r += (currBest[i] - query[i]) * (currBest[i] - query[i]);}
	double d = (curr->point[dim] - query[dim]) * (curr->point[dim] - query[dim]);
	if(r >= d){
		(goLeft) ? recursiveFind(curr->right, query, currBest, (dim+1)%Dim) : recursiveFind(curr->left, query, currBest, (dim+1)%Dim);
	}
}
