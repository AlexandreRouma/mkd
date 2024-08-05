#include "soft_break.h"

namespace mkd::inlines {
    SoftBreak::SoftBreak() :
        Inline(INLINE_TYPE_SOFT_BREAK)
    {}

    std::shared_ptr<SoftBreak> SoftBreak::parse(Source& src) {
        // Expect a newline or give up
        if (!src.expect('\n')) { return NULL; }

        // Create the soft break object
        return std::make_shared<SoftBreak>();
    }
}