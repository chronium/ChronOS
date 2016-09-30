#include <kernel/rect.h>
#include <kernel/list.h>

Rect *Rect::intersect (Rect *other) {
  if (!(this->left <= other->getRight () &&
        this->right >= other->getLeft () &&
        this->top <= other->getBottom () &&
        this->bottom >= other->getTop ()))
    return nullptr;

  Rect *result = new Rect (this->top, this->left, this->bottom, this->right);

  if (other->getLeft () >= result->getLeft () && other->getLeft () <= result->getRight ())
    result->SetLeft (other->getLeft ());

  if (other->getTop () >= result->getTop () && other->getTop () <= result->getBottom ())
    result->SetTop (other->getTop ());

  if (other->getRight () >= result->getLeft () && other->getRight () <= result->getRight ())
    result->SetRight (other->getRight ());

  if (other->getBottom () >= result->getTop () && other->getBottom () <= result->getBottom ())
    result->SetBottom (other->getBottom ());

  return result;
}

List<Rect> *Rect::split (Rect *other) {
  List<Rect> *output = new List<Rect> ();

  Rect subject(this->top, this->left, this->bottom, this->right);

  Rect *temp;

  if (other->getLeft () >= subject.getLeft () && other->getLeft () <= subject.getRight ()) {
    temp = new Rect (subject.getTop (), subject.getLeft (), subject.getBottom (), other->getLeft () - 1);

    if (temp == nullptr) {
      delete output;
      return nullptr;
    }

    output->add (temp);

    subject.left = other->getLeft ();
  }

  if (other->getTop () >= subject.getTop () && other->getTop () <= subject.getBottom ()) {
    temp = new Rect (subject.getTop (), subject.getLeft (), other->getTop () - 1, subject.getRight());

    if (!temp) {
      delete output;
      return nullptr;
    }

    output->add (temp);

    subject.top = other->getTop ();
  }

  if (other->getRight () >= subject.getLeft () && other->getRight () <= subject.getRight ()) {
    temp = new Rect (subject.getTop (), other->getRight () + 1, subject.getBottom (), subject.getRight ());

    if (!temp) {
      delete output;
      return nullptr;
    }

    output->add (temp);
    subject.right = other->right;
  }

  if (other->getBottom () >= subject.getTop () && other->getBottom () <= subject.getBottom ()) {
    temp = new Rect (other->getBottom () + 1, subject.getLeft (), subject.getBottom (), subject.getRight ());

    if (!temp) {
      delete output;
      return nullptr;
    }

    output->add (temp);
    subject.bottom = other->bottom;
  }

  return output;
}
