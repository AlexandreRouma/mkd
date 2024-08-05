#include "autolink.h"
#include "flog/flog.h"

namespace mkd::inlines::Autolink {
    bool parseAbsoluteURI(Source& src, std::string& uri) {
        // TODO
        return false;
    }

    bool parseEmailAddress(Source& src, std::string& uri) {
        // No... fuck off
        return false;
    }

    std::shared_ptr<Link> parse(Source& src) {
        // Begin parsing session
        src.begin();

        // Expect a less than sign or give up
        if (!src.expect('<')) {
            src.abort();
            return NULL;
        }

        // Attempt to parse an absolute uri
        std::string uri;
        bool absURI = parseAbsoluteURI(src, uri);
        if (!absURI) {
            // Clear the URI in case the absolute parser wrote some characters
            uri.clear();

            // Try parsing an email instead or give up
            if (!parseEmailAddress(src, uri)) {
                src.abort();
                return NULL;
            }
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
        return std::make_shared<Link>(inlines, absURI ? uri : ("mailto:" + uri));
    }
}