#pragma once

#include "tonic/log.h"

#include <string>
#include <unordered_map>
#include <memory>

namespace tonic::core
{
	// A class holding assets od specific type. Basically a map with nicer API with some error checks sprinkled on top.
	template<typename T>
	class AssetLibrary
	{
	public:
		AssetLibrary() = default;
		~AssetLibrary() = default;
		
		//------------------------------------------------------------------
		// GetAll(): return an unordered map of loaded assets
		const std::unordered_map<std::string, std::shared_ptr<T>>& GetAll() { return m_assets; }

		//------------------------------------------------------------------
		// Load(name, asset): insert asset with key: name
		void Load(const std::string& name, std::shared_ptr<T> asset)
		{
			m_assets[name] = asset;
		}

		//------------------------------------------------------------------
		// Get(name): return asset with key: name
		std::shared_ptr<T> Get(const std::string& name) const
		{
			auto it = m_assets.find(name);
			if (it == m_assets.end())
			{
				TONIC_ERROR("AssetLibrary::Get() - Asset not found: {}", name);
				return nullptr;
			}
			return it->second;
		}
		// different syntax
		std::shared_ptr<T> operator [] (const std::string& name) const
		{
			auto it = m_assets.find(name);
			if (it == m_assets.end())
			{
				TONIC_ERROR("AssetLibrary::operator [] - Asset not found: {}", name);
				return nullptr;
			}
			return it->second;
		}

		//------------------------------------------------------------------
		// Exists(name): return true if name is an existing key, otherwise return false
		bool Exists(const std::string& name) const
		{
			return Get(name) == nullptr ? false : true;
		}

		//------------------------------------------------------------------
		// Delete(name): delete an asset with key: name
		void Delete(const std::string& name)
		{
			if (!Exists(name))
			{
				TONIC_ERROR("AssetLibrary::Delete() - Asset not found: {}", name);
				return;
			}
			
			auto it = m_assets.find(name);
			auto use_count = it->second.use_count();
			if (use_count > 1)
			{
				TONIC_INFO("AssetLibrary::Delete() - remaining handles for asset {}: {}", name, use_count);
			}
			m_assets.erase(it);
		}

		//------------------------------------------------------------------
		// Clear(): delete all keys and assets
		void Clear()
		{
			m_assets.clear();
		}

	private:
		
		std::unordered_map<std::string, std::shared_ptr<T>> m_assets;
	};
}