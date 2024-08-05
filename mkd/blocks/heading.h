#pragma once
#include "../block.h"
#include "../source.h"
#include "../inlines/text.h"

namespace mkd::blocks {
    class Heading : public Block {
    public:
        Heading(const std::vector<std::shared_ptr<Inline>>& inlines, int level);

        static std::shared_ptr<Heading> parse(Source& src);

        const std::vector<std::shared_ptr<Inline>> inlines;
        const int level;

    private:
        static std::shared_ptr<Heading> parseATX(Source& src);
        static std::shared_ptr<Heading> parseSetext(Source& src);
    };
}