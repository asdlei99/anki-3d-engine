// Copyright (C) 2009-2023, Panagiotis Christopoulos Charitos and contributors.
// All rights reserved.
// Code licensed under the BSD License.
// http://www.anki3d.org/LICENSE

#pragma once

#include <AnKi/Scene/Components/SceneComponent.h>
#include <AnKi/Scene/GpuSceneArray.h>
#include <AnKi/Collision/Aabb.h>
#include <AnKi/Collision/Sphere.h>

namespace anki {

/// @addtogroup scene
/// @{

/// Fog density component. Controls the fog density.
class FogDensityComponent : public SceneComponent
{
	ANKI_SCENE_COMPONENT(FogDensityComponent)

public:
	static constexpr F32 kMinShapeSize = 1.0_cm;

	FogDensityComponent(SceneNode* node);

	~FogDensityComponent();

	void setBoxVolumeSize(Vec3 sizeXYZ)
	{
		sizeXYZ = sizeXYZ.max(Vec3(kMinShapeSize));
		m_aabbMin = -sizeXYZ / 2.0f;
		m_aabbMax = sizeXYZ / 2.0f;
		m_isBox = true;
		m_dirty = true;
	}

	Vec3 getBoxVolumeSize() const
	{
		ANKI_ASSERT(isAabb());
		return m_aabbMax.xyz() - m_aabbMin.xyz();
	}

	void setSphereVolumeRadius(F32 radius)
	{
		m_sphereRadius = max(kMinShapeSize, radius);
		m_isBox = false;
		m_dirty = true;
	}

	F32 getSphereVolumeRadius() const
	{
		ANKI_ASSERT(isSphere());
		return m_sphereRadius;
	}

	Bool isAabb() const
	{
		return m_isBox == true;
	}

	Bool isSphere() const
	{
		return !m_isBox;
	}

	void setDensity(F32 d)
	{
		ANKI_ASSERT(d >= 0.0f);
		m_dirty = true;
		m_density = d;
	}

	F32 getDensity() const
	{
		return m_density;
	}

private:
	Vec3 m_aabbMin = Vec3(0.0f); ///< In local space.

	union
	{
		Vec3 m_aabbMax = Vec3(1.0f);
		F32 m_sphereRadius;
	};

	Vec3 m_worldPos = Vec3(0.0f);
	F32 m_density = 1.0f;

	GpuSceneArrays::FogDensityVolume::Allocation m_gpuSceneVolume;

	Bool m_isBox = true;
	Bool m_dirty = true;

	Error update(SceneComponentUpdateInfo& info, Bool& updated) override;
};

} // end namespace anki
