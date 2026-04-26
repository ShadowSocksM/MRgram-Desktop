#include "yukigram/options.h"
#include "yukigram/options_database.h"
#include "yukigram/options_runtime.h"
#include "base/variant.h"
#include "yukigram/lang.h"

namespace Yukigram::Options {

std::map<std::string_view, std::variant<Option<bool>, Option<int>, Option<QString>>> &Options() {
	static std::map<std::string_view, std::variant<Option<bool>, Option<int>, Option<QString>>> m;
	return m;
}

std::map<std::string_view, std::vector<std::string_view>> &ByCategory() {
	static std::map<std::string_view, std::vector<std::string_view>> m;
	return m;
}

template<typename T> rpl::variable<T> *make_impl(const char *key, Description<T> &&d) {
	const auto rv = std::make_shared<rpl::variable<T>>(d.defaultValue);

	const auto base = d.mirrorExperimental ? std::nullopt : std::optional(
		std::make_shared<
			base::options::option<T>,
			base::options::descriptor<T>
		>({
			.id = key,
			.defaultValue = d.defaultValue,
			.restartRequired = d.restartRequired,
		})
	);
	rpl::lifetime lifetime;

	const auto key_v = std::string_view(key);
	ByCategory()[d.category].push_back(key_v);
	Options()[key_v] = Option<T>{
		.key = key_v,
		.d = std::move(d),
		.v = rv,
		._base = base,
		._lifetime = std::move(lifetime)
	};

	return rv.get();
}

rpl::variable<bool> *make_bool(const char *key, Description<bool> &&d) {
	return make_impl<bool>(key, std::move(d));
}

rpl::variable<int> *make_int(const char *key, Description<int> &&d) {
	return make_impl<int>(key, std::move(d));
}

rpl::variable<QString> *make_qs(const char *key, Description<QString> &&d) {
	return make_impl<QString>(key, std::move(d));
}

void update_value(const char *key, bool value) {
	if (auto it = Options().find(std::string_view(key)); it != Options().end()) {
		if (const auto *o = std::get_if<Option<bool>>(&it->second)) {
			*o->v = value;
		} else {
			Unexpected("non-bool type in Yukigram::Options::update_value");
		}
	}
}

void init() {
	for (auto &[key, option] : Options()) {
		v::match(option, [](Option<bool> &o) {
			o.init();
		}, [](Option<int> &o) {
			o.init();
		}, [](Option<QString> &o) {
			o.init();
		});
	}
}

void link() {
	for (auto &[key, option] : Options()) {
		v::match(option, [](Option<bool> &o) {
			o.link();
		}, [](Option<int> &o) {
			o.link();
		}, [](Option<QString> &o) {
			o.link();
		});
	}
}


namespace Valid {

rpl::producer<QString> AlwaysHint() {
	return {};
}

QString BoundedImpl(int lower, int upper, int value) {
	if (value < lower) {
		return ktr("input/bounded/lower", { "value", QString::number(value) }, { "lower", QString::number(lower) });
	} else if (value > upper) {
		return ktr("input/bounded/upper", { "value", QString::number(value) }, { "upper", QString::number(upper) });
	}
	return {};
}

CheckValid<int> Bounded(int lower, int upper) {
	return std::bind_front(BoundedImpl, lower, upper);
}

std::function<rpl::producer<QString>()> BoundedHint(int lower, int upper) {
	return [=]() {
		return rktr("input/bounded/hint", { "lower", QString::number(lower) }, { "upper", QString::number(upper) });
	};
}

} // namespace Valid


} // namespace Yukigram::Options
