#include "rpl/rpl.h"
namespace Yukigram::Settings {
extern rpl::variable<bool> *ShowMsgId;
extern const char kOptionShowMsgId[];
}

#include "data/data_msg_id.h"
int64 CleanMessageId(MsgId id);
