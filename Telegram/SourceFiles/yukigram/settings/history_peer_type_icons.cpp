#include "yukigram/settings/history_peer_type_icons.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *HistoryPeerTypeIcons = Yukigram::Options::make<bool>(kOptionHistoryPeerTypeIcons, {
	.keywords = { u"yukigram"_q, u"peer"_q, u"user"_q, u"icon"_q, u"bot"_q, u"channel"_q, u"group"_q, u"chat"_q, u"forum"_q },
	.category = "messages",
});

const char kOptionHistoryPeerTypeIcons[] = "history-peer-type-icons";

}
