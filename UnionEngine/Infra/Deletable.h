#pragma once

#include "Unique.h"

namespace Infra
{
	class Deletable : public virtual Unique
	{
	public:
		virtual ~Deletable() noexcept = default;
	};
}