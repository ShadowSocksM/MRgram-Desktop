#include "yukigram/settings/compact_chat_list.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *CompactChatList = Yukigram::Options::make<bool>(kOptionCompactChatList, {
	.keywords = { u"yukigram"_q, u"compact"_q, u"chat"_q },
	.category = "dialogs",
});

const char kOptionCompactChatList[] = "compact-chat-list";

}
