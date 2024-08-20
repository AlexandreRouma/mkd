#include "list.h"

namespace mkd::blocks {
    List::List() : Block(BLOCK_TYPE_HEADING) {}

    static std::shared_ptr<List> parse(Source& src) {
        
    }
}