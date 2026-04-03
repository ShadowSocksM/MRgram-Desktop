#include "yukigram/settings/wide_messages.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *WideMessages = Yukigram::Options::make<bool>(kOptionWideMessages, {
	.keywords = { u"yukigram"_q, u"layout"_q, u"messages"_q, u"wide"_q },
	.category = "style",
});

const char kOptionWideMessages[] = "wide-messages";

}
