
/**
 * @file heap.cpp
 * Implementation of a heap class.
 */
#include <iostream>
#include <tgmath.h>

template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    // @TODO Update to return the index you are choosing to be your root.
    return 1;
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the left child.
    return 2 * currentIdx;
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the right child.
    return 1 + 2 * currentIdx;
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    // @TODO Update to return the index of the parent.
    return floor(currentIdx / 2);
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    // @TODO Update to return whether the given node has a child
    if (currentIdx * 2 < _elems.size()) {
      return true;
    } else {
      return false;
    }
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the child with highest priority
    ///   as defined by higherPriority()
    if (currentIdx * 2 == _elems.size() - 1) {
      return leftChild(currentIdx);
    } else {
      if (higherPriority(_elems[leftChild(currentIdx)], _elems[rightChild(currentIdx)])) {
        return leftChild(currentIdx);
      } else  {
        return rightChild(currentIdx);
      }
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{
    // @TODO Implement the heapifyDown algorithm.
    size_t maxIdx = currentIdx;
    size_t leftIdx = currentIdx * 2;
    size_t rightIdx = currentIdx * 2 + 1;
    if (leftIdx < _elems.size() && higherPriority(_elems[leftIdx], _elems[maxIdx])) {
      maxIdx = leftIdx;
    }
    if (rightIdx < _elems.size() && higherPriority(_elems[rightIdx], _elems[maxIdx])) {
      maxIdx = rightIdx;
    }
    if (maxIdx != currentIdx) {
      std::swap(_elems[currentIdx], _elems[maxIdx]);
      heapifyDown(maxIdx);
    }
}

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
  if (currentIdx > 1) {
    if (higherPriority(_elems[currentIdx], _elems[currentIdx / 2])) {
      std::swap(_elems[currentIdx], _elems[currentIdx / 2]);
      heapifyUp(currentIdx / 2);
    }
  }
}

template <class T, class Compare>
heap<T, Compare>::heap()
{
    // @TODO Depending on your implementation, this function may or may
    ///   not need modifying
    _elems.push_back(T());
}

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems)
{
    // @TODO Construct a heap using the buildHeap algorithm
    _elems.push_back(T());
    for (T elem: elems)
      _elems.push_back(elem);
    int idx = _elems.size()-1;
    for (int i = idx; i > 0; i--) {
      heapifyDown(i);
    }
}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    // @TODO Remove, and return, the element with highest priority
    T output = _elems[1];
    std::swap(_elems[1], _elems.back());
    _elems.pop_back();
    heapifyDown(1);
    return output;
}

template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    // @TODO Return, but do not remove, the element with highest priority
    return _elems[1];
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    // @TODO Add elem to the heap
    _elems.push_back(elem);
    heapifyUp(_elems.size() - 1);
}

template <class T, class Compare>
void heap<T, Compare>::updateElem(const size_t & idx, const T& elem)
{
    // @TODO In-place updates the value stored in the heap array at idx
    // Corrects the heap to remain as a valid heap even after update
    _elems[idx] = elem;
    heapifyUp(idx);
    heapifyDown(idx);
}


template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    // @TODO Determine if the heap is empty
    if (_elems.size() <= 1) {
      return true;
    }
    return false;
}

template <class T, class Compare>
void heap<T, Compare>::getElems(std::vector<T> & heaped) const
{
    for (size_t i = root(); i < _elems.size(); i++) {
        heaped.push_back(_elems[i]);
    }
}
