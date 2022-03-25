#ifndef JSON_H
#define JSON_H

#include <stdint.h>
//pool format
//keys: size (1 byte) | key | \0 (1 byte) | next key dir (2 bytes) | value dir (2 bytes) or value (n bytes)

template<uint16_t _poolTSize>
class Json
{
	public:
		Json();
		~Json();

	private:
		uint16_t findKey(const char* _key);
		uint16_t createKey(const char* _key);

		uint8_t pool_[_poolTSize] = {};
		uint8_t* ptr_ = pool_;

		uint16_t freeSpace_ = _poolTSize;
		uint16_t accesDir_ = 0;
};

#include <Json.inl>

#endif