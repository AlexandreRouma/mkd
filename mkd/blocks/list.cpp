#include "list.h"

namespace mkd::blocks {
    List::List(bool ordered, const std::vector<std::shared_ptr<ListItem>>& items) :
        Block(BLOCK_TYPE_LIST),
        ordered(ordered),
        items(items)
    {}

    std::shared_ptr<List> List::parse(Source& src) {
        // Parse as many items of the same type
        std::vector<std::shared_ptr<ListItem>> items;
        char lastMarker = 0;
        while (true) {
            // Begin parsing session
            src.begin();

            // Expect a list item
            auto item = ListItem::parse(src);
            if (!item) {
                src.abort();
                break;
            }

            // If not the first item, make sure the marker matches
            if (lastMarker && item->marker != lastMarker && !(std::isdigit(item->marker) && std::isdigit(lastMarker))) {
                src.abort();
                break;
            }
            lastMarker = item->marker;

            // Add the new item to the list
            items.push_back(item);

            // Commit the parsing session
            src.commit();
        }

        // If no item was found, give up
        if (items.empty()) { return NULL; }

        // Return a new list
        return std::make_shared<List>(false/*TODO*/, items);
    }
}