#pragma once
#include "../inline.h"

namespace mkd::inlines {
    extern const std::vector<char> specialChars;

    class Text : public Inline {
    public:
        Text(const std::string& text);

        static std::shared_ptr<Text> parse(Source& src);
        static void coalesceText(std::vector<std::shared_ptr<Inline>>& inlines);

        const std::string text;
    };
}