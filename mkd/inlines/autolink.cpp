#include "autolink.h"
#include "flog/flog.h"
#include "link.h"

namespace mkd::inlines::Autolink {
    bool parseScheme(Source& src, std::string& scheme) {
        // Begin parsing session
        src.begin();

        // Expect an ascii letter
        if (!src.expect([&scheme](char c){
            if (std::isalpha(c)) {
                scheme += c;
                return true;
            }
            return false;
        })) {
            src.abort();
            return false;
        }

        // Expect at least one more letter, digit, '+', '-' or '.'
        while (src.expect([&scheme](char c){
            if (std::isalpha(c) || std::isdigit(c) || c == '+' || c == '-' || c == '.') {
                scheme += c;
                return true;
            }
            return false;
        }));

        // If not enough characters were found, give up
        if (scheme.size() < 2) {
            scheme.clear();
            return false;
        }

        // Commit and return successfully
        src.commit();
        return true;
    }

    bool parseAbsoluteURI(Source& src, std::string& uri) {
        // Begin parsing session
        src.begin();

        // Expect a scheme
        std::string scheme;
        if (!parseScheme(src, scheme)) {
            src.abort();
            return false;
        }

        // Expect a colon
        if (!src.expect(':')) {
            src.abort();
            return false;
        }

        // Expect any number of characters that aren't a space, '<' or '>'
        uri = scheme + ':';
        while (src.expect([&uri](char c){
            if (c != ' ' && c != '<' && c != '>') {
                uri += c;
                return true;
            }
            return false;
        }));

        // Commit and return successfully
        src.commit();
        return true;
    }

    // TODO: Find a way to not need to return a generic inline because of the include loop
    std::shared_ptr<Inline> parse(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect a less than sign or give up
        if (!src.expect('<')) {
            src.abort();
            return NULL;
        }

        // Attempt to parse an absolute uri
        std::string uri;
        if (!parseAbsoluteURI(src, uri)) {
            // Clear the URI in case the absolute parser wrote some characters
            uri.clear();
            src.abort();
            return NULL;
        }

        // Expect a greater than sign or give up
        if (!src.expect('>')) {
            src.abort();
            return NULL;
        }
        
        // Create the text inline for the link
        std::vector<std::shared_ptr<Inline>> inlines;
        inlines.push_back(std::make_shared<Text>(uri));

        // Commit parsing session and create link object
        src.commit();
        return std::make_shared<Link>(inlines, uri, uri /* TODO: Check if this title is correct */);
    }
}