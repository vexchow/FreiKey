#if !defined(SOFTWARE_H)
#define SOFTWARE_H

#include "bit_array.h"

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
  void resolveChanges(const bit_array& beforeLeft,
                      const bit_array& afterleft,
                      const bit_array& beforeRight,
                      const bit_array& afterRight);
}
} // namespace state

#endif
