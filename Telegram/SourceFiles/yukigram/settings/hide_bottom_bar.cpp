#include "yukigram/settings/hide_bottom_bar.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HideBottomBar = Yukigram::Options::make<bool>(kOptionHideBottomBar, {
	.keywords = { u"yukigram"_q, u"bottom"_q, u"bar"_q, u"join"_q, u"mute"_q, u"unmute"_q, u"gift"_q, u"direct"_q },
	.category = "chat",
});

const char kOptionHideBottomBar[] = "hide-bottom-bar";

}
