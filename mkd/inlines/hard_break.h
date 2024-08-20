#pragma once
#include "../inline.h"
#include "../source.h"

namespace mkd::inlines {
    class HardBreak : public Inline {
    public:
        HardBreak();
        static std::shared_ptr<HardBreak> parse(Source& src);
    };
}