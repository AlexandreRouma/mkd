#include "list_item.h"

namespace mkd::blocks {
    ListItem::ListItem(char marker, const std::vector<std::shared_ptr<Inline>>& inlines) :
        Block(BLOCK_TYPE_LIST_ITEM),
        marker(marker),
        inlines(inlines)
    {}

    static std::shared_ptr<ListItem> parse(Source& src) {
        // Begin parsing session
        src.begin();
        
        // Expect a marker char
        char marker;
        if (!src.expect([&marker](char c){ return marker = c, (c == '-' || c == '+' || c == '*' || std::isdigit(c)); })) {
            src.abort();
            return NULL;
        }

        // If the marker is a digit, expect up to 8 more digits
        for (int i = 0; i < 8; i++) {
            if (!src.expect([](char c){ return std::isdigit(c); })) { break; }
        }

        // Expect either a '.' or ')'
        if (!src.expect('.') && !src.expect(')')) {
            src.abort();
            return NULL;
        }

        // TODO: Finish
        src.abort();
        return NULL;
    }
}