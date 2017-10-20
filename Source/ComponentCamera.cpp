#include "ComponentCamera.h"
#include "MathGeoLib\Geometry\Plane.h"

ComponentCamera::ComponentCamera(GameObject* attached_gameobject)
{
	SetActive(true);
	SetName("Camera");
	SetType(ComponentType::Camera);
	SetGameObject(attached_gameobject);

	camera_frustum.type = FrustumType::PerspectiveFrustum;
	camera_frustum.nearPlaneDistance = 0.3f;
	camera_frustum.farPlaneDistance = 1000;

	background_color = Black;

	camera_viewport_texture = nullptr;
	camera_target_texture = nullptr;
}

ComponentCamera::~ComponentCamera()
{
	RELEASE(camera_viewport_texture);
	RELEASE(camera_target_texture);
}

void ComponentCamera::DrawFrustum()
{

}

bool ComponentCamera::ContainsGameObjectAABB(AABB& gameobject_bounding_box)
{
	int vertex_num = gameobject_bounding_box.NumVertices();
	for (int i = 0; i < 6; i++) //planes of frustum
	{
		for (int j = 0; j < vertex_num; j++) //vertex number of box
		{
			Plane plane = camera_frustum.GetPlane(i);
			if (plane.Contains(gameobject_bounding_box.CornerPoint(j)))
			{
				return true;
			}
		}
	}
	return false;
}

void ComponentCamera::SetFOV(float fov)
{

}

void ComponentCamera::SetAspectRatio(float aspect_ratio)
{
	/*camera_frustum.horizontalFov*/
}

float ComponentCamera::GetFOV() const
{
	return 0.0f;
}

float ComponentCamera::GetNearPlaneDistance() const
{
	return camera_frustum.nearPlaneDistance;
}

float ComponentCamera::GetFarPlanceDistance() const
{
	return camera_frustum.farPlaneDistance;
}

float ComponentCamera::GetAspectRatio() const
{
	return camera_frustum.AspectRatio();
}

void ComponentCamera::SetLayerToAvoid(std::string layer)
{
	layer_to_avoid = layer;
}
