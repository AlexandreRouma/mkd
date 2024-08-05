#pragma once
#include "../inline.h"

namespace mkd::inlines::Autolink {
    std::shared_ptr<Link> parse(Source& src);
}