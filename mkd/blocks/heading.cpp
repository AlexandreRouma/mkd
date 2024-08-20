#include "heading.h"
#include "paragraph.h"

#include "../inlines/text.h"

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

        // Coalesce all the text objects
        inlines::Text::coalesceText(inlines);

        // Commit the parsing session and create the heading object 
        src.commit();
        return std::make_shared<Heading>(inlines, level);
    }

    std::shared_ptr<Heading> Heading::parseSetext(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect at most three spaces
        for (int i = 0; i < 2; i++) {
            if (!src.expect(' ')) { break; }
        }

        // Expect inlines until encountering a setex underline
        std::vector<std::shared_ptr<Inline>> inlines;
        std::shared_ptr<Inline> inl = NULL;
        int level = 0;
        while (true) {
            // Stop if a setex underline is found
            level = parseSetextUnderline(src);
            if (level) { break; }

            // Parse an inline
            inl = Inline::parse(src, !inlines.empty() && inlines.back()->type != INLINE_TYPE_SOFT_BREAK);
            if (!inl) { break; }
            inlines.push_back(inl);
        }

        // If no inlines or no underline were found, give up
        if (inlines.empty() || !level) {
            src.abort();
            return NULL;
        }

        // Remove the soft-break
        inlines.pop_back();

        // Commit the parsing session and create the heading object 
        src.commit();
        return std::make_shared<Heading>(inlines, level);
    }

    int Heading::parseSetextUnderline(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect at most three spaces
        for (int i = 0; i < 2; i++) {
            if (!src.expect(' ')) { break; }
        }

        // Expect either '=' or '-'
        char bc = 42;
        if (!src.expect([&bc](char c) { return bc = c, (c == '=' || c == '-'); })) {
            src.abort();
            return 0;
        }

        // Expect any number of this characters
        while (src.expect([bc](char c) { return c == bc; }));

        // Expect optional spaces
        while (src.expect([bc](char c) { return c == ' ' || c == '\t' || c == '\r'; }));

        // Expect either a newline or end of file
        if (!src.expect('\n') && src.available()) {
            src.abort();
            return 0;
        }

        // Commit and return successfully
        src.commit();
        return (bc == '=') ? 1 : 2;
    }
}