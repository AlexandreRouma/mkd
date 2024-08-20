#include "link.h"
#include "flog/flog.h"
#include <ranges>

namespace mkd::inlines {
    Link::Link(const std::vector<std::shared_ptr<Inline>>& inlines, const std::string& destination, const std::string& title) :
        Inline(INLINE_TYPE_LINK),
        inlines(inlines),
        destination(destination),
        title(title)
    {}

    bool parseLinkDestination(Source& src, std::string& destination) {
        // Begin parsing session
        src.begin();

        // Skip any number of spaces and/or a single newline
        bool firstLF = true;
        while (src.expect([&firstLF](char c){
            if (c == '\n' && firstLF) {
                firstLF = false;
                return true;
            }
            return c == ' ' || c == '\t';
        }));

        // Save whether the text starts with a less then symbole
        bool lt = src.expect('<');

        // Parse text with different rules depending on if it started with a less than symbol
        if (lt) {
            // Read all possible text characters until illegal character
            bool escaped = false;
            while (src.expect([&escaped, &destination](char c){
                // Line endings are not allowed
                if (c == '\n') { return false; }

                // If not in escaped mode, process special characters
                if (!escaped) {
                    // Backslashes turn on escaped mode
                    if (c == '\\') {
                        escaped = true;
                        return true;
                    }
                    
                    // Less than and greater than symbols are illegal
                    if (c == '<' || c == '>') { return false; }
                }

                // Add character to the string and turn off escaped mode
                destination += c;
                escaped = false;
                return true;
            }));
        }
        else {
            // Read all possible text characters until illegal character
            bool escaped = false;
            int parenthDepth = 0;
            while (src.expect([&escaped, &parenthDepth, &destination](char c){
                // ASCII Control Characters are not allowed
                if (c <= ' ' || c == 127) { return false; }

                // If not in escaped mode, process special characters
                if (!escaped) {
                    // Backslashes turn on escaped mode
                    if (c == '\\') {
                        escaped = true;
                        return true;
                    }
                    
                    // Count the opening and closing parenthesis to ensure they match later
                    if (c == '(') { parenthDepth++; }
                    if (c == ')') {
                        // If no depth, it can't be allowed
                        if (!parenthDepth) { return false; }

                        // Decrement depth
                        parenthDepth--;
                    }
                }

                // Add character to the string and turn off escaped mode
                destination += c;
                escaped = false;
                return true;
            }));

            // If the number of opening and closing parenthesis don't match, give up
            if (parenthDepth) {
                src.abort();
                return false;
            }
        }

        // If the string started with a less than symbol, expect one at the end
        if (lt && !src.expect('>')) {
            src.abort();
            return false;
        }

        // Commit parsing session and return successfully
        src.commit();
        return true;
    }

    bool parseLinkTitle(Source& src, std::string& title) {
        // Begin parsing session
        src.begin();

        // Skip any number of spaces and/or a single newline
        bool firstLF = true;
        while (src.expect([&firstLF](char c){
            if (c == '\n' && firstLF) {
                firstLF = false;
                return true;
            }
            return c == ' ' || c == '\t';
        }));

        // Expect either a double quote, a quote or an left parenthesis or give up
        char bc;
        if (!src.expect([&bc](char c) { return bc = c, (c == '"' || c == '\'' || c == '('); })) {
            src.abort();
            return NULL;
        }

        // If a left parenthesis is used, convert it to a right one since that will be the closing character
        if (bc == '(') { bc = ')'; }

        // Read all possible text characters until illegal character
        bool escaped = false;
        while (src.expect([&escaped, &title, bc](char c){
            // If not in escaped mode, process special characters
            if (!escaped) {
                // Backslashes turn on escaped mode
                if (c == '\\') {
                    escaped = true;
                    return true;
                }
                
                // Less than and greater than symbols are illegal
                if (c == bc) { return false; }
            }

            // Add character to the string and turn off escaped mode
            title += c;
            escaped = false;
            return true;
        }));

        // Expect the closing symbol
        if (!src.expect(bc)) {
            src.abort();
            return false;
        }

        // Commit parsing session and return successfully
        src.commit();
        return true;
    }

    std::shared_ptr<Link> Link::parse(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect a left square bracket or give up
        if (!src.expect('[')) {
            src.abort();
            return NULL;
        }

        // Parse inlines until a right square bracket
        std::vector<std::shared_ptr<Inline>> inlines;
        while (true) {
            // If the current right char is a square bracket, stop parsing inlines
            if (src.expect(']')) { break; }

            // Parse the next inline
            auto inl = Inline::parse(src, inlines.empty() || inlines.back()->type != INLINE_TYPE_SOFT_BREAK);
            if (inl) {
                // If contains a link, it will have priority, so give up
                if (inl->type == INLINE_TYPE_LINK) {
                    src.abort();
                    return NULL;
                }
                inlines.push_back(inl);
            }
            else {
                flog::error("Failed to parse inline in link, wtf would happen...");
                break;
            }
        }

        // If the last inline is a line break, remove it
        if (!inlines.empty() && inlines.back()->type == INLINE_TYPE_SOFT_BREAK) { inlines.pop_back(); }

        // Expect a left parenthesis or give up
        if (!src.expect('(')) {
            src.abort();
            return NULL;
        }

        // Parse the link destination if there is one (TODO: Move clear to parser)
        std::string destination;
        if (!parseLinkDestination(src, destination)) { destination.clear(); }

        // Parse the link title if there is one (TODO: Move clear to parser)
        std::string title;
        if (!parseLinkTitle(src, title)) { title.clear(); }

        // Skip any number of spaces and/or a single newline
        bool firstLF = true;
        while (src.expect([&firstLF](char c){
            if (c == '\n' && firstLF) {
                firstLF = false;
                return true;
            }
            return c == ' ' || c == '\t';
        }));

        // Expect a right parenthesis or give up
        if (!src.expect(')')) {
            src.abort();
            return NULL;
        }

        // Coalesce all the text objects
        inlines::Text::coalesceText(inlines);
        
        // Commit parsing session and return the link object
        src.commit();
        return std::make_shared<Link>(inlines, destination, title);
    }
}