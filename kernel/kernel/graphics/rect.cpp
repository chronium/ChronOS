#include <kernel/rect>
#include <kernel/list>

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

    output->insert (temp);

    subject.left = other->getLeft ();
  }

  if (other->getTop () >= subject.getTop () && other->getTop () <= subject.getBottom ()) {
    temp = new Rect (subject.getTop (), subject.getLeft (), other->getTop () - 1, subject.getRight());

    if (!temp) {
      delete output;
      return nullptr;
    }

    output->insert (temp);

    subject.top = other->getTop ();
  }

  if (other->getRight () >= subject.getLeft () && other->getRight () <= subject.getRight ()) {
    temp = new Rect (subject.getTop (), other->getRight () + 1, subject.getBottom (), subject.getRight ());

    if (!temp) {
      delete output;
      return nullptr;
    }

    output->insert (temp);
    subject.right = other->right;
  }

  if (other->getBottom () >= subject.getTop () && other->getBottom () <= subject.getBottom ()) {
    temp = new Rect (other->getBottom () + 1, subject.getLeft (), subject.getBottom (), subject.getRight ());

    if (!temp) {
      delete output;
      return nullptr;
    }

    output->insert (temp);
    subject.bottom = other->bottom;
  }

  return output;
}
