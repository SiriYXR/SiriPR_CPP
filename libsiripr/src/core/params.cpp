#include "siripr/core/params.h"

namespace siripr {
	CParams* CParams::instance_ = nullptr;

	CParams* CParams::instance() {
		if (!instance_) {
			instance_ = new CParams;
		}
		return instance_;
	}
}/*! \namespace siripr*/