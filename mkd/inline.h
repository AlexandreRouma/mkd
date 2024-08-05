#pragma once
#include "source.h"
#include <memory>

namespace mkd {
    enum InlineType {
        INLINE_TYPE_LINK,
        INLINE_TYPE_IMAGE,
        INLINE_TYPE_SOFT_BREAK,
        INLINE_TYPE_TEXT
    };

    class Inline {
    public:
        Inline(InlineType type);
        virtual ~Inline();

        static std::shared_ptr<Inline> parse(Source& src, bool allowSoftBreak = true);

        const InlineType type;
    };
}

#include "inlines/link.h"
#include "inlines/image.h"
#include "inlines/autolink.h"
#include "inlines/soft_break.h"
#include "inlines/text.h"