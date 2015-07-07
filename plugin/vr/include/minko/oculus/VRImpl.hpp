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

#pragma once

#include "minko/Common.hpp"
#include "minko/OculusCommon.hpp"
#include "minko/Signal.hpp"

namespace minko
{
    namespace oculus
    {
        class VRImpl : 
            public std::enable_shared_from_this<VRImpl>
        {
        public:
            virtual
            void
            initialize(std::shared_ptr<component::SceneManager> sceneManager) = 0;

            virtual
            void
            initializeVRDevice(std::shared_ptr<component::Renderer> leftRenderer, std::shared_ptr<component::Renderer> rightRenderer, void* window = nullptr) = 0;

            virtual
            void
            targetRemoved() = 0;

            virtual
            void
            updateViewport(int viewportWidth, int viewportHeight) = 0;

            virtual
            void
            updateCameraOrientation(std::shared_ptr<scene::Node> target, std::shared_ptr<scene::Node> leftCamera, std::shared_ptr<scene::Node> rightCamera) = 0;

            virtual
            float
            getLeftEyeFov() = 0;

            virtual
            float
            getRightEyeFov() = 0;

            virtual
            float
            zNear() = 0;

            virtual
            float
            zFar() = 0;

            virtual
            Signal<>::Ptr
            actionButtonPressed() = 0;

            virtual
            Signal<>::Ptr
            actionButtonReleased() = 0;
        };
    }
}
