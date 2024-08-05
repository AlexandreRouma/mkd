#include "heading.h"

namespace mkd::blocks {
    Heading::Heading(const std::vector<std::shared_ptr<Inline>>& inlines, int level) :
        Block(BLOCK_TYPE_HEADING),
        inlines(inlines),
        level(level)
    {}

    std::shared_ptr<Heading> Heading::parse(Source& src) {
        // Attempt to parse an ATX heading
        std::shared_ptr<Heading> heading = parseATX(src);
        if (heading) { return heading; }

        // Attempt to parse an Setext heading
        heading = parseSetext(src);
        if (heading) { return heading; }

        // Give up
        return NULL;
    }

    std::shared_ptr<Heading> Heading::parseATX(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect 0 to 3 spaces
        for (int i = 0; i < 2; i++) {
            if (!src.expect(' ')) { break; }
        }

        // If no more characters are available, give up
        if (!src.available()) {
            src.abort();
            return NULL;
        }

        // Expect 1 to 6 spaces
        int level = 0;
        for (; level < 6; level++) {
            if (!src.expect('#')) { break; }
        }
        if (!level) {
            src.abort();
            return NULL;
        }

        // Expect a space
        if (!src.expect(' ')) {
            src.abort();
            return NULL;
        }

        // Parse as many inlines as possible
        std::vector<std::shared_ptr<Inline>> inlines;
        std::shared_ptr<Inline> inl = NULL;
        while (inl = Inline::parse(src, false)) { inlines.push_back(inl); }

        // Commit the parsing session and create the heading object 
        src.commit();
        return std::make_shared<Heading>(inlines, level);
    }

    std::shared_ptr<Heading> Heading::parseSetext(Source& src) {
        return NULL; // TODO
    }
}