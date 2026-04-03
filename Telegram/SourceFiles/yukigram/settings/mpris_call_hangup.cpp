#include "yukigram/settings/mpris_call_hangup.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *MprisCallHangup = Yukigram::Options::make<bool>(kOptionMprisCallHangup, {
	.keywords = { u"yukigram"_q, u"mpris"_q, u"hangup"_q, u"call"_q },
	.category = "system",
	.restartRequired = true,
});

const char kOptionMprisCallHangup[] = "mpris-call-hangup";

}
