#ifndef _KERNEL_LIST_NODE_H
#define _KERNEL_LIST_NODE_H

template <typename T>
class List;

template <typename T>
class ListNode {
  friend class List<T>;

public:
  ListNode (T *data): data (data), prev (nullptr), next (nullptr) { };
  ~ListNode () {
    prev = nullptr;
    next = nullptr;
    data = nullptr;
  }

  inline T* getData () { return this->data; };

private:
  T *data;

  ListNode *prev;
  ListNode *next;
};

#endif
