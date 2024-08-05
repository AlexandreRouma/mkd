#pragma once
#include "../inline.h"
#include "../source.h"

namespace mkd::inlines {
    class SoftBreak : public Inline {
    public:
        SoftBreak();
        static std::shared_ptr<SoftBreak> parse(Source& src);
    };
}