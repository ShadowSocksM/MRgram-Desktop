#include "yukigram/settings/disable_animoji.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *DisableAnimoji = Yukigram::Options::make<bool>(kOptionDisableAnimoji, {
	.keywords = { u"yukigram"_q, u"animoji"_q, u"emoji"_q },
	.category = "messages",
	.restartRequired = true,
});

const char kOptionDisableAnimoji[] = "disable-animoji";

}
