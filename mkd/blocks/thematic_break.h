#pragma once
#include "../block.h"
#include "../source.h"

namespace mkd::blocks {
    class ThematicBreak : public Block {
    public:
        ThematicBreak();
        static std::shared_ptr<ThematicBreak> parse(Source& src);
    };
}