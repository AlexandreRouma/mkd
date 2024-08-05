#include "image.h"
#include "link.h"
#include "flog/flog.h"
#include <ranges>

namespace mkd::inlines {
    Image::Image(const std::vector<std::shared_ptr<Inline>>& inlines, const std::string& destination, const std::string& title) :
        Inline(INLINE_TYPE_IMAGE),
        inlines(inlines),
        destination(destination),
        title(title)
    {}

    std::shared_ptr<Image> Image::parse(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect an exclamation mark
        if (!src.expect('!')) {
            src.abort();
            return NULL;
        }

        // Parse the rest of the image definition as a link or give up
        auto link = Link::parse(src);
        if (!link) {
            src.abort();
            return NULL;
        }
        
        // Commit parsing session and return the link object
        src.commit();
        return std::make_shared<Image>(link->inlines, link->destination, link->title);
    }
}