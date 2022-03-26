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
	ptr_[i+4] = jType::undef;
	ptr_[i+5] = 0;
	ptr_[i+6] = 0;
	ptr_ += i + 7;
	return ret;
}

template<uint16_t _poolTSize>
uint16_t Json<_poolTSize>::findKey(const char* _key)
{
	uint8_t* poolKey = pool_ + accesDir_;
	uint8_t sizeKey = *poolKey;
	uint16_t nextKey = ((*(poolKey + sizeKey + 2)) << 8) | (*(poolKey + sizeKey + 1));
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
			nextKey = ((*(poolKey + sizeKey + 2)) << 8) | (*(poolKey + sizeKey + 1));
		}
	}
	return 0;
}