#include "yukigram/settings/inferred_peer_dc.h"
#include "yukigram/options.h"

namespace Yukigram::Settings {

rpl::variable<bool> *InferredPeerDc = Yukigram::Options::make<bool>(kOptionInferredPeerDc, {
	.keywords = { u"yukigram"_q, u"inferred"_q, u"peer"_q, u"dc"_q, u"datacenter"_q },
	.category = "info",
});

const char kOptionInferredPeerDc[] = "inferred-peer-dc";

}
