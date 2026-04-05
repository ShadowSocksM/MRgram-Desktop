#include "yukigram/settings/custom_emoji_size.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<int> *CustomEmojiSize = Yukigram::Options::make<int>(kOptionCustomEmojiSize, {
	.keywords = { u"yukigram"_q, u"emoji"_q, u"size"_q },
	.category = "style",
	.restartRequired = true,
	.defaultValue = 112,
	.checkValid = Yukigram::Options::Valid::Bounded(32, 256),
	.validHint = Yukigram::Options::Valid::BoundedHint(32, 256),
});

const char kOptionCustomEmojiSize[] = "custom-emoji-size";

}
