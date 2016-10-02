#ifndef _KERNEL_LIST_H
#define _KERNEL_LIST_H

#include <kernel/list_node.h>

#include <stdbool.h>
#include <stddef.h>

template <typename T>
class List {
public:
  List (): size (0), root (nullptr) { };
  ~List () {
    ListNode<T> *current = this->root;

    if (current != nullptr)
      while (current->next) {
        ListNode<T> *old = current;
        current = current->next;
        delete old;
      }

    root = nullptr;
    size = 0;
  };

  int add (T *data);
  T *remove (int index);
  T *get (int index);
  void clear ();

  inline int Count () const { return this->size; };

private:
  int size;

  ListNode<T> *root;
};

template <typename T>
int List<T>::add (T *data) {
  ListNode<T> *new_node = new ListNode<T> (data);

  if (!this->root)
    this->root = new_node;
  else {
    ListNode<T> *current = this->root;

    while (current->next)
      current = current->next;

    current->next = new_node;
    new_node->prev = current;
  }

  return this->size++;
}

template <typename T>
T *List<T>::remove (int index) {
  T* payload;

  if (this->size == 0 || index >= this->size)
    return nullptr;

  ListNode<T> *current = this->root;

  for (int curr_index = 0; (curr_index < index) && current; curr_index++)
    current = current->next;

  if (!current)
    return nullptr;

  payload = current->getData ();

  if (current->prev)
    current->prev->next = current->next;

  if (current->next)
    current->next->prev = current->prev;

  if (index == 0)
    this->root = current->next;

  delete current;

  this->size--;

  return payload;
}

template <typename T>
T *List<T>::get (int index) {
  if (this->size == 0 || index >= this->size)
    return nullptr;

  ListNode<T> *current = this->root;

  for (int curr_index = 0; (curr_index < index) && current; curr_index++)
    current = current->next;

  return current ? current->data : nullptr;
}

template <typename T>
void List<T>::clear () {
  while (this->size)
    this->remove (0);
}

#endif
