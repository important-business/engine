#include "components/ai.hpp"
#include "components/camera.hpp"
#include "components/collision.hpp"
#include "components/player.hpp"
#include "components/render.hpp"
#include "components/transform.hpp"
#include "components/velocity.hpp"
#include <string>

namespace components
{

const std::string AiComponent::name{"ai"};
const std::string CameraComponent::name{"camera"};
const std::string Collision::name{"collision"};
const std::string MovementComponent::name{"movement"};
const std::string PlayerComponent::name{"player"};
const std::string TextureComponent::name{"texture"};
const std::string TransformComponent::name{"transform"};
const std::string Trigger::name{"trigger"};
const std::string PhysicsComponent::name{"physics"};

} // namespace components
