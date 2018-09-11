#if !defined(SOFTWARE_H)
#define SOFTWARE_H

#include "switch_matrix.h"

namespace state {
class kb {
  // Active layer
  // Current 'down' keys?

  // Operations:
  // Not sure the right return type yet
  // Possible results:
  //  A HID keyreport (mods + keys)
  //  A consumer key press
  //  An "LED state" change
  //  An *internal only* state change (i.e. nothing)
  //  A list of most everything above
  void resolveChanges(const switch_matrix& beforeLeft,
                      const switch_matrix& afterleft,
                      const switch_matrix& beforeRight,
                      const switch_matrix& afterRight);
}
} // namespace state

#endif