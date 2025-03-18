#include "module/all.hpp"

int main()
{
    auto out = ts::io::logging();

    out.info("init ok");
    return 0;
}