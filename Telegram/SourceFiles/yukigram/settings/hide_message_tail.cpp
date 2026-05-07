#include "yukigram/settings/hide_message_tail.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HideMessageTail = Yukigram::Options::make<bool>(kOptionHideMessageTail, {
	.keywords = { u"yukigram"_q, u"message"_q, u"bubble"_q, u"tail"_q },
	.category = "style",
	.restartRequired = true,
});

const char kOptionHideMessageTail[] = "hide-message-tail";

}
