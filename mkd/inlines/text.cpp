#include "text.h"
#include <algorithm>

namespace mkd::inlines {
    const std::vector<char> specialChars = { '[', ']', '*', '`' };

    Text::Text(const std::string& text) :
        Inline(INLINE_TYPE_TEXT),
        text(text)
    {}

    std::shared_ptr<Text> Text::parse(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect at least one character or give up
        std::string text;
        while (src.expect([&text](char c) {
            if (c != '\n' && (text.empty() ||std::find(specialChars.begin(), specialChars.end(), c) == specialChars.end())) {
                text += c;
                return true;
            }
            return false;
        }));
        if (text.empty()) {
            src.abort();
            return NULL;
        }

        // Commit the parsing session and return successfully
        src.commit();
        return std::make_shared<Text>(text);
    }
}