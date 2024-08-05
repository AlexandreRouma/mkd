#pragma once
#include "../inline.h"
#include "../source.h"
#include "../inlines/text.h"

namespace mkd::inlines {
    class Image : public Inline {
    public:
        Image(const std::vector<std::shared_ptr<Inline>>& inlines, const std::string& destination, const std::string& title);
        static std::shared_ptr<Image> parse(Source& src);

        const std::vector<std::shared_ptr<Inline>> inlines;
        const std::string destination;
        const std::string title;
    };
}