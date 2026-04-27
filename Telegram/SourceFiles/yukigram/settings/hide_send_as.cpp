#include "yukigram/settings/hide_send_as.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HideSendAs = Yukigram::Options::make<bool>(kOptionHideSendAs, {
	.keywords = { u"yukigram"_q, u"send"_q, u"channels"_q },
	.category = "compose",
});

const char kOptionHideSendAs[] = "hide-send-as";

}
