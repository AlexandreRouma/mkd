#include "text.h"
#include <algorithm>

namespace mkd::inlines {
    const std::vector<char> specialChars = { '[', ']', '*', '`', ' ' };

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

    void Text::coalesceText(std::vector<std::shared_ptr<Inline>>& inlines) {
        int offset = 0;
        while (true) {
            // Skip non-text objects
            for (; offset < inlines.size() && inlines[offset]->type != INLINE_TYPE_TEXT; offset++);

            // If no objects are left, we're done
            if (offset >= inlines.size()) { break; }

            // Append all consecutive text objects
            int end = offset;
            std::string text;
            for (; end < inlines.size() && inlines[end]->type == INLINE_TYPE_TEXT; end++) {
                std::shared_ptr<mkd::inlines::Text> to = std::dynamic_pointer_cast<mkd::inlines::Text>(inlines[end]);
                text += to->text;
            }

            // Replace first text object
            inlines[offset] = std::make_shared<Text>(text);

            // Remove all elements between in [offset;end]
            inlines.erase(inlines.begin()+offset+1, inlines.begin()+end);

            // Go to the next object
            offset++;
        }
    }
}