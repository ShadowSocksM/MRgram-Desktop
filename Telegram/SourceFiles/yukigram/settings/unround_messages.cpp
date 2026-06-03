#include "yukigram/settings/unround_messages.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *UnroundMessages = Yukigram::Options::make<bool>(kOptionUnroundMessages, {
	.keywords = { u"yukigram"_q, u"round"_q, u"messages"_q, u"square"_q },
	.category = "style",
});

const char kOptionUnroundMessages[] = "unround-messages";

}
