#include "thematic_break.h"

namespace mkd::blocks {
    ThematicBreak::ThematicBreak() :
        Block(BLOCK_TYPE_THEMATIC_BREAK)
    {}

    std::shared_ptr<ThematicBreak> ThematicBreak::parse(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect 0 to 3 spaces
        for (int i = 0; i < 2; i++) {
            if (!src.expect(' ')) { break; }
        }

        // If no more characters are available, give up
        if (!src.available()) {
            src.abort();
            return NULL;
        }

        // Expect either a star, a dash or an underscore or give up
        char bc;
        if (!src.expect([&bc](char c) { return bc = c, (c == '*' || c == '-' || c == '_'); })) {
            src.abort();
            return NULL;
        }

        // Expect at least two more of the same character or give up
        for (int i = 0; i < 2; i++) {
            if (!src.expect(bc)) {
                src.abort();
                return NULL;
            }
        }

        // Expect only these characters until a newline or empty file
        while (src.expect([bc](char c) { return c == bc; }));

        // If characters are still available and it's not a newline, give up
        if (src.available() && src[0] != '\n') {
            src.abort();
            return NULL;
        }

        // If data is available and is a newline, consume it
        else if (src.available()) { src.consume(1); }

        // Commit the parsing session and return successfully
        src.commit();
        return std::make_shared<ThematicBreak>();
    }
}