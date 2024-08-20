#pragma once
#include "../block.h"
#include "../source.h"
#include "../inlines/text.h"

namespace mkd::blocks {
    class List : public Block {
    public:
        List();

        static std::shared_ptr<List> parse(Source& src);
    };
}