#include "yukigram/settings/ghost_mode.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *GhostMode = Yukigram::Options::make<bool>(kOptionGhostMode, {
        .keywords = { u"yukigram"_q, u"ghost"_q, u"privacy"_q, u"online"_q, u"read"_q, u"typing"_q },
        .category = "interface",
        .restartRequired = false,
});

const char kOptionGhostMode[] = "ghost-mode";

}
