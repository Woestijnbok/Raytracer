#include "Material.h"

namespace dae
{
	Material_SolidColor::Material_SolidColor(const ColorRGB& color) :
		Material(),
		m_Color{ color }
	{

	}

	ColorRGB Material_SolidColor::Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) const
	{
		return m_Color;
	}

	Material_Lambert::Material_Lambert(const ColorRGB& diffuseColor, float diffuseReflectance) :
		Material(),
		m_DiffuseColor{ diffuseColor },
		m_DiffuseReflectance{ diffuseReflectance }
	{

	}

	ColorRGB Material_Lambert::Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) const
	{
		return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor);
	}

	Material_LambertPhong::Material_LambertPhong(const ColorRGB& diffuseColor, float kd, float ks, float phongExponent) :
		Material(),
		m_DiffuseColor{ diffuseColor },
		m_DiffuseReflectance{ kd },
		m_SpecularReflectance{ ks },
		m_PhongExponent{ phongExponent }
	{

	}

	ColorRGB Material_LambertPhong::Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) const
	{
		return BRDF::Lambert(m_DiffuseReflectance, m_DiffuseColor) + BRDF::Phong(m_SpecularReflectance, m_PhongExponent, l, v, hitRecord.normal);
	}

	Material_CookTorrence::Material_CookTorrence(const ColorRGB& albedo, float metalness, float roughness) :
		Material(),
		m_Albedo{ albedo },
		m_Metalness{ metalness },
		m_Roughness{ roughness }
	{

	}

	ColorRGB Material_CookTorrence::Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) const
	{
		const Vector3 h{ (v + l).Normalized() };
		const ColorRGB f0{ (AreEqual(m_Metalness, 0.0f)) ? ColorRGB{ 0.04f, 0.04f, 0.04f } : m_Albedo };

		const ColorRGB F{ BRDF::FresnelFunction_Schlick(h, v, f0) };
		const float D{ BRDF::NormalDistribution_GGX(hitRecord.normal, h, m_Roughness) };
		const float G{ BRDF::GeometryFunction_Smith(hitRecord.normal, v, l, m_Roughness) };

		const ColorRGB specular{ (D * F * G) / (4 * Vector3::Dot(v, hitRecord.normal) * Vector3::Dot(l, hitRecord.normal)) };
		const ColorRGB kd{ (AreEqual(m_Metalness, 1.0f)) ? colors::Black : colors::White - F };
		const ColorRGB diffuse{ BRDF::Lambert(kd, m_Albedo) };

		return diffuse + specular;
	}
}