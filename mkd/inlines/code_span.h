#pragma once
#include "../inline.h"

namespace mkd::inlines {
    class CodeSpan : public Inline {
    public:
        CodeSpan(const std::string& code);

        static std::shared_ptr<CodeSpan> parse(Source& src);

        const std::string code;
    
    private:
        static int parseBacktickString(Source& src);
    };
}