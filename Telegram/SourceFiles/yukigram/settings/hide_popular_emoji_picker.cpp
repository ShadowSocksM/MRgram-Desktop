#include "yukigram/settings/hide_popular_emoji_picker.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HidePopularEmojiPicker = Yukigram::Options::make<bool>(kOptionHidePopularEmojiPicker, {
	.keywords = { u"yukigram"_q, u"emoji"_q, u"animoji"_q, u"picker"_q, u"popular"_q },
	.category = "picker",
});

const char kOptionHidePopularEmojiPicker[] = "hide-popular-emoji-picker";

}
