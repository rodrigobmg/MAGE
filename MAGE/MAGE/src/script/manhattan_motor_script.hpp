#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "scripting\behavior_script.hpp"
#include "math\transform.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	class ManhattanMotorScript final : public BehaviorScript {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		explicit ManhattanMotorScript(Transform *transform)
			: BehaviorScript(), m_transform(transform), m_velocity(2.0f) {}
		ManhattanMotorScript(const ManhattanMotorScript &script) = delete;
		ManhattanMotorScript(ManhattanMotorScript &&script) = default;
		virtual ~ManhattanMotorScript() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		ManhattanMotorScript &operator=(const ManhattanMotorScript &script) = delete;
		ManhattanMotorScript &operator=(ManhattanMotorScript &&script) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		virtual void Update(double delta_time) override;

		float GetVelocity() const {
			return m_velocity;
		}
		void SetVelocity(float velocity) {
			m_velocity = velocity;
		}

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		Transform * const m_transform;
		float m_velocity;
	};
}