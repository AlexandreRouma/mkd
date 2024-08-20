#pragma once
#include "../block.h"
#include "../source.h"
#include "../inline.h"

namespace mkd::blocks {
    class Paragraph : public Block {
    public:
        Paragraph(const std::vector<std::shared_ptr<Inline>>& inlines);
        static std::shared_ptr<Paragraph> parse(Source& src);

        const std::vector<std::shared_ptr<Inline>> inlines;
    };
}