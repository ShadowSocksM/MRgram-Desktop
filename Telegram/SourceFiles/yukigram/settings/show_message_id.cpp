#include "yukigram/settings/show_message_id.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *ShowMsgId = Yukigram::Options::make<bool>(kOptionShowMsgId, {
	.keywords = { u"yukigram"_q, u"message"_q, u"id"_q },
	.category = "messages",
	.restartRequired = true,
});

const char kOptionShowMsgId[] = "show-msgid";

}

int64 CleanMessageId(MsgId id) {
	if (id.bare > 0) {
		if (id < ServerMaxMsgId) {
			return id.bare;
		} else if (id < ScheduledMaxMsgId) {
			return (id - ServerMaxMsgId).bare;
		} else if (id < ShortcutMaxMsgId) {
			return (id - ScheduledMaxMsgId).bare;
		}
	} else if (IsClientMsgId(id)) {
		return ClientMsgIndex(id);
	} else if (IsStoryMsgId(id)) {
		return StoryIdFromMsgId(id);
	}
	return -1;
};
