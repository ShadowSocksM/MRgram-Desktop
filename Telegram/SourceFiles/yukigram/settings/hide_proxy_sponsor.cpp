#include "yukigram/settings/hide_proxy_sponsor.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HideProxySponsor = Yukigram::Options::make<bool>(
        kOptionHideProxySponsor,
        {
                .keywords = {
                        u"mrgram"_q,
                        u"proxy"_q,
                        u"sponsor"_q,
                        u"privacy"_q,
                },
                .category = "privacy",
                .restartRequired = false,
        });

const char kOptionHideProxySponsor[] = "hide-proxy-sponsor";

} // namespace Yukigram::Settings
