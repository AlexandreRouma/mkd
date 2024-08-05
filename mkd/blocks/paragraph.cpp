#include "paragraph.h"

namespace mkd::blocks {
    Paragraph::Paragraph(const std::vector<std::shared_ptr<Inline>>& inlines) :
        Block(BLOCK_TYPE_PARAGRAPH),
        inlines(inlines)
    {}

    std::shared_ptr<Paragraph> Paragraph::parse(Source& src) {
        // Parse as many inlines as possible
        std::vector<std::shared_ptr<Inline>> inlines;
        std::shared_ptr<Inline> inl = NULL;
        while (inl = Inline::parse(src, !inlines.empty() && inlines.back()->type != INLINE_TYPE_SOFT_BREAK)) {
            inlines.push_back(inl);
        }

        // If nothing was parsed, give up
        if (inlines.empty()) { return NULL; }

        // If the last inline is a line break, remove it
        if (inlines.back()->type == INLINE_TYPE_SOFT_BREAK) { inlines.pop_back(); }

        // Create paragraph ob
        return std::make_shared<Paragraph>(inlines);
    }
}