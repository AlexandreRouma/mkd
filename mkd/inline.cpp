#include "inline.h"

namespace mkd {
    Inline::Inline(InlineType type) :
        type(type)
    {}

    Inline::~Inline() {}

    std::shared_ptr<Inline> Inline::parse(Source& src, bool allowBreak) {
        // Attempt to parse a code span
        std::shared_ptr<Inline> inl = inlines::CodeSpan::parse(src);
        if (inl) { return inl; }
        
        // Attempt to parse a link
        inl = inlines::Link::parse(src);
        if (inl) { return inl; }

        // Attempt to parse an image
        inl = inlines::Image::parse(src);
        if (inl) { return inl; }

        // Attempt to parse an autolink
        inl = inlines::Autolink::parse(src);
        if (inl) { return inl; }

        // Attempt to parse breaks if allowed
        if (allowBreak) {
            // Attempt to parse a hard break
            inl = inlines::HardBreak::parse(src);
            if (inl) { return inl; }

            // Attempt to parse a shoft break
            inl = inlines::SoftBreak::parse(src);
            if (inl) { return inl; }
        }

        // Attempt to parse text
        inl = inlines::Text::parse(src);
        if (inl) { return inl; }

        // Give up
        return NULL;
    }
}