#include "yukigram/settings/hide_reply_background_emoji.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HideReplyBackgroundEmoji = Yukigram::Options::make<bool>(kOptionHideReplyBackgroundEmoji, {
	.keywords = { u"yukigram"_q, u"reply"_q, u"emoji"_q, u"background"_q },
	.category = "style",
});

const char kOptionHideReplyBackgroundEmoji[] = "hide-reply-background-emoji";

}
