#include "yukigram/settings/hide_stories.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HideStories = Yukigram::Options::make<bool>(
        kOptionHideStories,
        {
                .keywords = {
                        u"mrgram"_q,
                        u"stories"_q,
                        u"hide"_q,
                        u"privacy"_q,
                },
                .category = "privacy",
                .restartRequired = false,
        });

const char kOptionHideStories[] = "hide-stories";

} // namespace Yukigram::Settings
