#include "yukigram/settings/right_action_comments.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *RightActionComments = Yukigram::Options::make<bool>(kOptionRightActionComments, {
	.keywords = { u"yukigram"_q, u"comments"_q, u"right"_q, u"compact"_q },
	.category = "messages",
});

const char kOptionRightActionComments[] = "comments-button-always-right";

}
