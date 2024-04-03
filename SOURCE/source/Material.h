#pragma once
#include "Math.h"
#include "DataTypes.h"
#include "BRDFs.h"

namespace dae
{
	class Material
	{
		public:
			Material() = default;
			virtual ~Material() = default;

			Material(const Material&) = delete;
			Material(Material&&) noexcept = delete;
			Material& operator=(const Material&) = delete;
			Material& operator=(Material&&) noexcept = delete;

			virtual ColorRGB Shade(const HitRecord& hitRecord = {}, const Vector3& l = {}, const Vector3& v = {}) const = 0;

		protected:


		private:

	};

	class Material_SolidColor final : public Material
	{
		public:
			Material_SolidColor(const ColorRGB& color);
			virtual ~Material_SolidColor() = default;

			Material_SolidColor(const Material_SolidColor&) = delete;
			Material_SolidColor(Material_SolidColor&&) noexcept = delete;
			Material_SolidColor& operator=(const Material_SolidColor&) = delete;
			Material_SolidColor& operator=(Material_SolidColor&&) noexcept = delete;

			virtual ColorRGB Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) const override;

		private:
			ColorRGB m_Color;
	};

	class Material_Lambert final : public Material
	{
		public:
			Material_Lambert(const ColorRGB& diffuseColor, float diffuseReflectance);
			virtual ~Material_Lambert() = default;

			Material_Lambert(const Material_Lambert&) = delete;
			Material_Lambert(Material_Lambert&&) noexcept = delete;
			Material_Lambert& operator=(const Material_Lambert&) = delete;
			Material_Lambert& operator=(Material_Lambert&&) noexcept = delete;

			virtual ColorRGB Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) const override;

		private:
			ColorRGB m_DiffuseColor;
			float m_DiffuseReflectance;
	};

	class Material_LambertPhong final : public Material
	{
		public:
			Material_LambertPhong(const ColorRGB& diffuseColor, float kd, float ks, float phongExponent);
			virtual ~Material_LambertPhong() = default;

			Material_LambertPhong(const Material_LambertPhong&) = delete;
			Material_LambertPhong(Material_LambertPhong&&) noexcept = delete;
			Material_LambertPhong& operator=(const Material_LambertPhong&) = delete;
			Material_LambertPhong& operator=(Material_LambertPhong&&) noexcept = delete;

			virtual ColorRGB Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) const override;

		private:
			ColorRGB m_DiffuseColor;
			float m_DiffuseReflectance; 
			float m_SpecularReflectance; 
			float m_PhongExponent;
	};

	class Material_CookTorrence final : public Material
	{
		public:
			Material_CookTorrence(const ColorRGB& albedo, float metalness, float roughness);
			virtual ~Material_CookTorrence() = default;

			Material_CookTorrence(const Material_CookTorrence&) = delete;
			Material_CookTorrence(Material_CookTorrence&&) noexcept = delete;
			Material_CookTorrence& operator=(const Material_CookTorrence&) = delete;
			Material_CookTorrence& operator=(Material_CookTorrence&&) noexcept = delete;

			virtual ColorRGB Shade(const HitRecord& hitRecord, const Vector3& l, const Vector3& v) const override;

		private:
			ColorRGB m_Albedo;
			float m_Metalness;
			float m_Roughness;
	};
}
