#include "rive/animation/blend_state_direct.hpp"
#include "rive/animation/state_machine.hpp"
#include "rive/animation/state_machine_number.hpp"
#include "rive/animation/blend_state_direct_instance.hpp"
#include "rive/importers/state_machine_importer.hpp"

using namespace rive;

StateInstance* BlendStateDirect::makeInstance(ArtboardInstance* artboard) const {
    return new BlendStateDirectInstance(this, artboard);
}