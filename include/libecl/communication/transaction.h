#ifndef ECL_COMMUNICATION_TRANSACTION_H
#define ECL_COMMUNICATION_TRANSACTION_H

#include <cstdint>
#include <span>

namespace ecl::communication {
struct Transaction
{
	enum class Type
	{
		Read,
		ReadWrite,
		Write,
	};

	Type m_type;
	std::span<const uint8_t> m_tx;
	std::span<uint8_t> m_rx;
};
}  // namespace ecl::communication

#endif
