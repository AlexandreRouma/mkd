#include "html.h"

namespace mkd::inlines {
    HTML::HTML(const std::string& html) :
        Inline(INLINE_TYPE_HTML),
        html(html)
    {}

    std::shared_ptr<HTML> HTML::parse(Source& src) {
        // Begin parsing session
        src.begin();

        std::string html;

        // TODO: Implement
        src.abort();
        return NULL;

        // Commit the parsing session and return successfully
        src.commit();
        return std::make_shared<HTML>(html);
    }

    std::string HTML::parseTagName(Source& src) {
        // TODO: Implement
        return "";
    }

}