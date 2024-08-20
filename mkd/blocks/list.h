#pragma once
#include "../block.h"
#include "../source.h"
#include "list_item.h"

namespace mkd::blocks {
    class List : public Block {
    public:
        List(bool ordered, const std::vector<std::shared_ptr<ListItem>>& items);

        static std::shared_ptr<List> parse(Source& src);
        
        const bool ordered;
        const std::vector<std::shared_ptr<ListItem>> items;
    };
}