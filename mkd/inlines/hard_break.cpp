#include "hard_break.h"

namespace mkd::inlines {
    HardBreak::HardBreak() :
        Inline(INLINE_TYPE_HARD_BREAK)
    {}

    std::shared_ptr<HardBreak> HardBreak::parse(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect at two spaces
        if (!src.expect(' ') || !src.expect(' ')) {
            src.abort();
            return NULL;
        }

        // Expect any more number of spaces or space-like characters
        while (src.expect([](char c) {
            return c == ' ' || c == '\t' || c == '\r';
        }));

        // Expect a newline or give up
        if (!src.expect('\n')) {
            src.abort();
            return NULL;
        }

        // Create the soft break object
        src.commit();
        return std::make_shared<HardBreak>();
    }
}