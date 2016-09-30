#ifndef _KERNEL_RECT_H
#define _KERNEL_RECT_H

#include <stdint.h>
#include <kernel/list.h>

class Rect {
public:
  Rect (int32_t top, int32_t left, int32_t bottom, int32_t right) :
    top (top), left (left), bottom (bottom), right (right) { };

  inline int32_t getTop     () const { return this->top; };
  inline int32_t getLeft    () const { return this->left; };
  inline int32_t getBottom  () const { return this->bottom; };
  inline int32_t getRight   () const { return this->right; };

  inline void SetTop (int32_t val) { this->top = val; }
  inline void SetLeft (int32_t val) { this->left = val; }
  inline void SetBottom (int32_t val) { this->bottom = val; }
  inline void SetRight (int32_t val) { this->right = val; }

  List<Rect> *split (Rect *other);
  Rect *intersect (Rect *other);

private:
  int32_t top;
  int32_t left;
  int32_t bottom;
  int32_t right;
};

#endif
