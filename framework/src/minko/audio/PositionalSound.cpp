/*
Copyright (c) 2014 Aerys

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "minko/audio/PositionalSound.hpp"
#include "minko/audio/SoundChannel.hpp"
#include "minko/audio/SoundTransform.hpp"
#include "minko/scene/Node.hpp"
#include "minko/scene/NodeSet.hpp"
#include "minko/component/Transform.hpp"
#include "minko/component/PerspectiveCamera.hpp"

using namespace minko;
using namespace minko::audio;

void
PositionalSound::update(scene::Node::Ptr target)
{
    if (!_channel->playing())
        return;

    // To compute the 3D volume, we need:
    // - the camera position in world space
    auto camera = getActiveCameraNode(target);
    auto cameraPos = camera->component<component::Transform>()->matrix()->translation();
    // - the target in world space
    auto targetPos = target->component<component::Transform>()->matrix()->translation();
    auto direction = cameraPos->subtract(targetPos);
    // - the distance betwen the camera and the target
    auto distance = direction->length();
    // - the direction the camera is looking to
    auto front = camera->component<component::Transform>()->modelToWorld(math::Vector3::up());

    // Normalize vectors to use the dot product operation.
    front->normalize();
    direction->normalize();

    // We need a vector to orient the angle so it can be signed.
    auto orientation = math::Vector3::create(math::Vector3::up())->cross(front);
    // Now we can get a signed scaled angle between [-1 ; 1] with a dot product.
    auto angle = orientation->dot(direction);

    // The volume is computed based on the distance.
    auto volume = _audibilityCurve(distance);

    // Simply adjust left/right volumes based on the angle.
    auto left = (angle + 1) / 2;
    auto right = (1 - angle) / 2;

    // LOG_INFO("volume: " << volume << ", left: " << left << ", right: " << right);

    // Update the sound transforms.
    SoundTransform::Ptr transform = _channel->transform();

    if (!transform)
        transform = SoundTransform::create();

    transform->volume(volume);
    transform->left(left);
    transform->right(right);

    _channel->transform(transform);
}

void
PositionalSound::stop(scene::Node::Ptr target)
{
    _channel->stop();
}

scene::Node::Ptr
PositionalSound::getActiveCameraNode(scene::Node::Ptr target)
{
    auto activeCameraNode = scene::NodeSet::create(target->root())->descendants(true)->where([](scene::Node::Ptr node)
    {
        auto camera = node->component<component::PerspectiveCamera>();
        auto renderer = node->component<component::Renderer>();
        return !!camera && !!renderer && renderer->enabled();
    });

    return activeCameraNode->size() > 0 ? activeCameraNode->nodes()[0] : nullptr;
}