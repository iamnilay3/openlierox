/////////////////////////////////////////
//
//   OpenLieroX
//
//   Auxiliary Software class library
//
//   based on the work of JasonB
//   enhanced by Dark Charlie and Albert Zeyer
//
//   code under LGPL
//
/////////////////////////////////////////


// Cache system
// Created 7/11/01
// By Jason Boettcher


#include "LieroX.h"
#include "Sounds.h"
#include "Cache.h"
#include "GfxPrimitives.h"
#include "FindFile.h"
#include "CGameScript.h"
#include "CMap.h"
#include "StringUtils.h"

CCache cCache;
float getCurrentTime()
{
	if( tLX == NULL )	// Cache is used before tLX is initialized
		return 0;
	return tLX->fCurTime;
};

//////////////
// Save an image to the cache
void CCache::SaveImage(const std::string& file1, const SmartPointer<SDL_Surface> & img)
{
	if( ! tLXOptions )
		return;
	if( tLXOptions->iMaxCachedEntries == 0 )
		return;	// Cache is disabled
	if (img == NULL)
		return;

	std::string file = file1;
	stringlwr(file);
	if( ImageCache.find(file) != ImageCache.end() )	// Error - already in cache
	{
		printf("Error: image %s already in cache - memleak\n", file.c_str() );
		return;
	};
	//printf("CCache::SaveImage(): %p %s\n", img, file.c_str() );
	ImageCache[file] = std::make_pair( img, getCurrentTime() );
}

//////////////
// Save a sound sample to the cache
void CCache::SaveSound(const std::string& file1, const SmartPointer<SoundSample> & smp)
{
	if( ! tLXOptions )
		return;
	if( tLXOptions->iMaxCachedEntries == 0 )
		return;	// Cache is disabled
	if (smp == NULL)
		return;

	std::string file = file1;
	stringlwr(file);
	if( SoundCache.find(file) != SoundCache.end() )
	{
		printf("Error: sound %s already in cache - memleak\n", file.c_str() );
		return;
	};
	SoundCache[file] = std::make_pair( smp, getCurrentTime() );
}

//////////////
// Save a map to the cache
void CCache::SaveMap(const std::string& file1, CMap *map)
{
	if( ! tLXOptions )
		return;
	if( tLXOptions->iMaxCachedEntries == 0 )
		return;	// Cache is disabled
	if (map == NULL)
		return;

	std::string file = file1;
	stringlwr(file);
	if( MapCache.find(file) != MapCache.end() )	// Error - already in cache
	{
		printf("Error: map %s already in cache\n", file.c_str() );
		return;
	};

	// Copy the map to the cache (not just the pointer because map changes during the game)
	CMap *cached_map = new CMap;
	if (cached_map == NULL)
		return;

	if (!cached_map->NewFrom(map))
		return;

	MapCache[file] = std::make_pair( SmartPointer<CMap>(cached_map), getCurrentTime() );
}

//////////////
// Save a mod to the cache
void CCache::SaveMod(const std::string& file1, const SmartPointer<CGameScript> & mod)
{
	if( ! tLXOptions )
		return;
	if( tLXOptions->iMaxCachedEntries == 0 )
		return;	// Cache is disabled
	if (mod == NULL)
		return;

	std::string file = file1;
	stringlwr(file);
	if( ModCache.find(file) != ModCache.end() )	// Error - already in cache
	{
		printf("Error: mod %s already in cache - memleak\n", file.c_str() );
		return;
	};

	ModCache[file] = std::make_pair( mod, getCurrentTime() );
}

//////////////
// Get an image from the cache
SmartPointer<SDL_Surface> CCache::GetImage(const std::string& file1)
{
	std::string file = file1;
	stringlwr(file);
	ImageCache_t::iterator item = ImageCache.find(file);
	if(item != ImageCache.end())
	{
		item->second.second = getCurrentTime();
		return item->second.first;
	};
	return NULL;
}

//////////////
// Get a sound sample from the cache
SmartPointer<SoundSample> CCache::GetSound(const std::string& file1)
{
	std::string file = file1;
	stringlwr(file);
	SoundCache_t::iterator item = SoundCache.find(file);
	if(item != SoundCache.end())
	{
		item->second.second = getCurrentTime();
		return item->second.first;
	};
	return NULL;
}

//////////////
// Get a map from the cache
SmartPointer<CMap> CCache::GetMap(const std::string& file1)
{
	std::string file = file1;
	stringlwr(file);
	MapCache_t::iterator item = MapCache.find(file);
	if(item != MapCache.end())
	{
		item->second.second = getCurrentTime();
		return item->second.first;
	};
	return NULL;
}

//////////////
// Get a mod from the cache
SmartPointer<CGameScript> CCache::GetMod(const std::string& file1)
{
	std::string file = file1;
	stringlwr(file);
	ModCache_t::iterator item = ModCache.find(file);
	if(item != ModCache.end())
	{
		item->second.second = getCurrentTime();
		return item->second.first;
	};
	return NULL;
}

//////////////
// Free all allocated data
void CCache::Clear()
{
	ModCache.clear();
	MapCache.clear();
	ImageCache.clear();
	SoundCache.clear();
}

void CCache::ClearExtraEntries()
{
	if( ! tLXOptions )
		return;
	if( tLXOptions->iMaxCachedEntries == 0 )
		return;
	if( (int)ImageCache.size() >= tLXOptions->iMaxCachedEntries )
	{	// Sorted by last-access time, iterators are not invalidated in a map when element is erased
		typedef std::multimap< float, ImageCache_t :: iterator > TimeSorted_t;
		TimeSorted_t TimeSorted;
		for( ImageCache_t :: iterator it = ImageCache.begin(); it != ImageCache.end(); it++  )
			TimeSorted.insert( std::make_pair( it->second.second, it ) );
		int clearCount = ImageCache.size() - tLXOptions->iMaxCachedEntries / 2;
		printf("CCache::ClearExtraEntries() clearing %i images\n", clearCount);
		for( TimeSorted_t :: iterator it1 = TimeSorted.begin(); 
				it1 != TimeSorted.end() && clearCount > 0; it1++, clearCount-- )
			ImageCache.erase( it1->second );
	};
	
	if( (int)SoundCache.size() >= tLXOptions->iMaxCachedEntries )
	{	// Sorted by last-access time, iterators are not invalidated in a map when element is erased
		typedef std::multimap< float, SoundCache_t :: iterator > TimeSorted_t;
		TimeSorted_t TimeSorted;
		for( SoundCache_t :: iterator it = SoundCache.begin(); it != SoundCache.end(); it++  )
			TimeSorted.insert( std::make_pair( it->second.second, it ) );
		int clearCount = SoundCache.size() - tLXOptions->iMaxCachedEntries / 2;
		printf("CCache::ClearExtraEntries() clearing %i sounds\n", clearCount);
		for( TimeSorted_t :: iterator it1 = TimeSorted.begin(); 
				it1 != TimeSorted.end() && clearCount > 0; it1++, clearCount-- )
			SoundCache.erase( it1->second );
	};
	
	if( (int)MapCache.size() >= tLXOptions->iMaxCachedEntries / 20 )
	{	// Sorted by last-access time, iterators are not invalidated in a map when element is erased
		typedef std::multimap< float, MapCache_t :: iterator > TimeSorted_t;
		TimeSorted_t TimeSorted;
		for( MapCache_t :: iterator it = MapCache.begin(); it != MapCache.end(); it++  )
			TimeSorted.insert( std::make_pair( it->second.second, it ) );
		int clearCount = MapCache.size() - tLXOptions->iMaxCachedEntries / 40;
		printf("CCache::ClearExtraEntries() clearing %i maps\n", clearCount);
		for( TimeSorted_t :: iterator it1 = TimeSorted.begin(); 
				it1 != TimeSorted.end() && clearCount > 0; it1++, clearCount-- )
			MapCache.erase( it1->second );
	};
	
	if( (int)ModCache.size() >= tLXOptions->iMaxCachedEntries / 20 )
	{	// Sorted by last-access time, iterators are not invalidated in a map when element is erased
		typedef std::multimap< float, ModCache_t :: iterator > TimeSorted_t;
		TimeSorted_t TimeSorted;
		for( ModCache_t :: iterator it = ModCache.begin(); it != ModCache.end(); it++  )
			TimeSorted.insert( std::make_pair( it->second.second, it ) );
		int clearCount = ModCache.size() - tLXOptions->iMaxCachedEntries / 40;
		printf("CCache::ClearExtraEntries() clearing %i mods\n", clearCount);
		for( TimeSorted_t :: iterator it1 = TimeSorted.begin(); 
				it1 != TimeSorted.end() && clearCount > 0; it1++, clearCount-- )
			ModCache.erase( it1->second );
	};
	
};

