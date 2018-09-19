#if !defined(SOFTWARE_H)
#define SOFTWARE_H

#include "boardio.h"

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
  void resolveChanges(const BoardIO::bits& beforeLeft,
                      const BoardIO::bits& afterleft,
                      const BoardIO::bits& beforeRight,
                      const BoardIO::bits& afterRight);
}
} // namespace state

#endif
