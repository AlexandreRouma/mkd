#pragma once#pragma once
#include "../block.h"
#include "../source.h"
#include "../inline.h"

namespace mkd::blocks {
    class ListItem : public Block {
    public:
        ListItem(char marker, const std::vector<std::shared_ptr<Inline>>& inlines);

        static std::shared_ptr<ListItem> parse(Source& src);

        const char marker;
        const std::vector<std::shared_ptr<Inline>> inlines;
    };
}