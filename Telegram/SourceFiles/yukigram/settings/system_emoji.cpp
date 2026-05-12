#include "yukigram/settings/system_emoji.h"
#include "yukigram/options.h"

namespace Ui::Emoji {
extern bool Yukigram_UseSystemEmojiFont;
}

namespace Yukigram::Settings {

rpl::variable<bool> *SystemEmoji = Yukigram::Options::make<bool>(kOptionSystemEmoji, {
	.keywords = { u"yukigram"_q, u"system"_q, u"emoji"_q, u"font"_q },
	.category = "interface",
	.link = [](bool value) { Ui::Emoji::Yukigram_UseSystemEmojiFont = value; },
});

const char kOptionSystemEmoji[] = "system-emoji";

}
