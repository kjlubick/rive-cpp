#ifndef _RIVE_COMPONENT_HPP_
#define _RIVE_COMPONENT_HPP_
#include "generated/component_base.hpp"
#include <stdio.h>
namespace rive
{
	class Component : public ComponentBase
	{
	public:
		Component() { printf("Constructing Component\n"); }
	};
} // namespace rive

#endif