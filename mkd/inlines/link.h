#pragma once
#include "../inline.h"
#include "../source.h"
#include "../inlines/text.h"

namespace mkd::inlines {
    class Link : public Inline {
    public:
        Link(const std::vector<std::shared_ptr<Inline>>& inlines, const std::string& destination, const std::string& title);
        static std::shared_ptr<Link> parse(Source& src);

        const std::vector<std::shared_ptr<Inline>> inlines;
        const std::string destination;
        const std::string title;
    };
}