#pragma once
#include "../block.h"
#include "../source.h"

namespace mkd::blocks {
    class BlankLine : public Block {
    public:
        BlankLine();
        static std::shared_ptr<BlankLine> parse(Source& src);
    };
}