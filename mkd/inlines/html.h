#pragma once
#include "../inline.h"

namespace mkd::inlines {
    class HTML : public Inline {
    public:
        HTML(const std::string& html);

        static std::shared_ptr<HTML> parse(Source& src);

        const std::string html;
    
    private:
        static std::string parseTagName(Source& src);
    };
}