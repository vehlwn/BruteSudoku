#include "StdRandom.h"

#include <chrono>
#include <thread>

namespace {
using Sed_t = decltype(vehlwn::StdRandom::gen)::result_type;
const auto seed = [] {
    static const auto rdSed = std::random_device{}();
    const auto        chronoSed = static_cast<Sed_t>(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());
    const auto tidSed =
        static_cast<Sed_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
    const auto ret = rdSed + chronoSed + tidSed;
    return ret;
};
} // namespace

namespace vehlwn {
thread_local std::mt19937 StdRandom::gen{seed()};
}
