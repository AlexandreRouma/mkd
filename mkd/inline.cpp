#include "inline.h"

namespace mkd {
    Inline::Inline(InlineType type) :
        type(type)
    {}

    Inline::~Inline() {}

    std::shared_ptr<Inline> Inline::parse(Source& src, bool allowSoftBreak) {
        // Attempt to parse a link
        std::shared_ptr<Inline> inl = inlines::Link::parse(src);
        if (inl) { return inl; }

        // Attempt to parse an image
        inl = inlines::Image::parse(src);
        if (inl) { return inl; }

        // Attempt to parse an autolink
        inl = inlines::Autolink::parse(src);
        if (inl) { return inl; }

        // Attempt to parse a soft break if allowed
        if (allowSoftBreak) {
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