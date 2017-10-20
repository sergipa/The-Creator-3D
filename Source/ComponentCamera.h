#pragma once
#include "Component.h"
#include "RenderTexture.h"
#include "MathGeoLib\Geometry\Frustum.h"
#include "MathGeoLib\Geometry\AABB.h"
#include "MathGeoLib\Math\Rect.h"


//MathGeoLib frustum info: http://clb.demon.fi/MathGeoLib/nightly/docs/Frustum_summary.php

class ComponentCamera :
	public Component
{
public:
	ComponentCamera(GameObject* attached_gameobject);
	~ComponentCamera();

	void DrawFrustum();
	bool ContainsGameObjectAABB(AABB& gameobject_bounding_box);

	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);
	float GetFOV() const;
	float GetNearPlaneDistance() const;
	float GetFarPlanceDistance() const;
	float GetAspectRatio() const;

	void SetLayerToAvoid(std::string layer);

private:
	RenderTexture* camera_viewport_texture;
	RenderTexture* camera_target_texture;
	Frustum camera_frustum;
	Color background_color;
	Rect viewport;
	std::string layer_to_avoid;
};

