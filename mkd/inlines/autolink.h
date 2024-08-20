#pragma once
#include "../inline.h"

namespace mkd::inlines::Autolink {
    std::shared_ptr<Inline> parse(Source& src);
}