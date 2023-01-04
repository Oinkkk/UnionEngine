#pragma once

#include "Unique.h"

namespace Infra
{
	template <typename $Type>
	class Handle : public virtual Unique
	{
	public:
		Handle(const $Type handle) noexcept;
		virtual ~Handle() noexcept = default;

		[[nodiscard]]
		constexpr const $Type &getHandle() noexcept;

	protected:
		[[nodiscard]]
		constexpr const $Type &getHandle() const noexcept;

	private:
		const $Type __handle;
	};

	template <typename $Type>
	Handle<$Type>::Handle(const $Type handle) noexcept :
		__handle{ handle }
	{}

	template <typename $Type>
	constexpr const $Type &Handle<$Type>::getHandle() noexcept
	{
		return __handle;
	}

	template <typename $Type>
	constexpr const $Type &Handle<$Type>::getHandle() const noexcept
	{
		return __handle;
	}
}