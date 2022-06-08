#include <stdint.h>

template<uint16_t _poolTSize>
Json<_poolTSize>::Json()
{
	static_assert(_poolTSize > 0, "Json must have an assigned size");
}

template<uint16_t _poolTSize>
uint16_t Json<_poolTSize>::createKey(const char* _key)
{
	uint16_t i = 0;
	uint16_t ret = ptr_ - pool_;
	for(i = 0; _key[i] != 0; i++) ptr_[i+1] = _key[i];
	*ptr_ = i;
	ptr_[i+1] = 0;
	ptr_[i+2] = 0;
	ptr_[i+3] = 0;
	ptr_[i+4] = 0;
	ptr_[i+5] = 0;
	ptr_[i+6] = jType::undef;
	ptr_[i+7] = 0;
	ptr_[i+8] = 0;
	ptr_ += i + 7;
	uint8_t* parent = pool_ + accesDir_;
	uint8_t parentSize = *parent;
	uint16_t parentNext = ((*(parent + parentSize + 3)) << 8) | (*(parent + parentSize + 2));
	uint16_t parentChild = ((*(parent + parentSize + 5)) << 8) | (*(parent + parentSize + 4));
	if(!parentChild)
	{
		parent[parentSize + 4] = (uint8_t)(ret & 0x00FF);
		parent[parentSize + 5] = (uint8_t)(ret & 0xFF00);
	}
	else
	{
		uint8_t* bro = pool_ + parentChild;
		uint8_t broSize = *bro;
		uint8_t broNext = ((*(bro + broSize + 3)) << 8) | (*(bro + broSize + 2));
		while(broNext)
		{
			uint8_t* bro = pool_ + broNext;
			uint8_t broSize = *bro;
			uint8_t broNext = ((*(bro + broSize + 3)) << 8) | (*(bro + broSize + 2));
		}
		bro[broSize + 2] = (uint8_t)(ret & 0x00FF);
		bro[broSize + 3] = (uint8_t)(ret & 0xFF00);
	}
	return ret;
}

template<uint16_t _poolTSize>
uint16_t Json<_poolTSize>::findKey(const char* _key)
{
	uint8_t* poolKey = pool_ + accesDir_;
	uint8_t sizeKey = *poolKey;
	uint16_t nextKey = ((*(poolKey + sizeKey + 3)) << 8) | (*(poolKey + sizeKey + 2));
	while(nextKey != 0)
	{
		poolKey++;
		int i;
		for(i = 0; _key[i] != 0 || poolKey[i] != 0; i++)
		{
			if(_key[i] != poolKey[i]) break;
		}
		if(_key[i] == poolKey[i])
			return poolKey - pool_ - 1;
		else
		{
			poolKey = pool_ + nextKey;
			sizeKey = *poolKey;
			nextKey = ((*(poolKey + sizeKey + 3)) << 8) | (*(poolKey + sizeKey + 2));
		}
	}
	return 0;
}

Json<_poolTSize> Json<_poolTSize>::operator[](const char* _key)
{
}