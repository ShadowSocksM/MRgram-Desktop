#include "yukigram/settings/hide_send_as_premium.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HideSendAsPremium = Yukigram::Options::make<bool>(kOptionHideSendAsPremium, {
	.keywords = { u"yukigram"_q, u"send"_q, u"channels"_q, u"premium"_q },
	.category = "compose",
	.restartRequired = true,
});

const char kOptionHideSendAsPremium[] = "hide-send-as-premium";

}
