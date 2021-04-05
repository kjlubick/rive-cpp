#ifndef _RIVE_STATE_TRANSITION_HPP_
#define _RIVE_STATE_TRANSITION_HPP_
#include "animation/state_transition_flags.hpp"
#include "generated/animation/state_transition_base.hpp"
#include <stdio.h>
namespace rive
{
	class StateTransition : public StateTransitionBase
	{
	private:
		StateTransitionFlags transitionFlags() const
		{
			return static_cast<StateTransitionFlags>(flags());
		}

	public:
		StatusCode onAddedDirty(CoreContext* context) override;
		StatusCode onAddedClean(CoreContext* context) override;

		bool isDisabled() const
		{
			return (transitionFlags() & StateTransitionFlags::Disabled) ==
			       StateTransitionFlags::Disabled;
		}

		/// Whether the animation is held at exit or if it keeps advancing
		/// during mixing.
		bool pauseOnExit() const
		{
			return (transitionFlags() & StateTransitionFlags::PauseOnExit) !=
			       StateTransitionFlags::PauseOnExit;
		}

		/// Whether exit time is enabled. All other conditions still apply, the
		/// exit time is effectively an AND with the rest of the conditions.
		bool enableExitTime() const
		{
			return (transitionFlags() & StateTransitionFlags::EnableExitTime) !=
			       StateTransitionFlags::EnableExitTime;
		}
	};
} // namespace rive

#endif