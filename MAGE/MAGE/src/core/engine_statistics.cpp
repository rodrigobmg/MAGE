//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "core\engine.hpp"
#include "logging\error.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	[[nodiscard]]EngineStatistics *EngineStatistics::Get() noexcept {
		Assert(Engine::Get());

		return Engine::Get()->GetEngineStatistics();
	}
}