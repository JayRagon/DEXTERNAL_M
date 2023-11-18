#include "wapi.h"
#include "strutils.h"

#define keydown 0
#define keyup 2

using namespace std::chrono;

int main()
{
	FindDiscord();
	auto duration = high_resolution_clock::now();

	std::cout << "Make sure to put a space then a | after your message/hex!\n\n";

	for (;;)
	{
		std::string ed;

		std::cout << "\nEnter text/hex:\n";

		for (;;)
		{
			std::string inputbuffer;
			std::cin >> inputbuffer;
			if (inputbuffer == "|")
			{
				ed.pop_back(); // remove last char (it will be a space)
				break;
			}
			else
			{
				ed += inputbuffer + ' ';
			}
		}

		bool isB16 = b16(ed);

		if (isB16 == false) // if ! b16 then we encrypt
		{
			size_t encryptmelen = ed.length();
			char* encryptme = new char[encryptmelen + 1];
			for (size_t i = 0; i < encryptmelen; i++) {
				encryptme[i] = ed[i];
			}
			encryptme[encryptmelen] = 0; // nullterminate


			size_t hexlen = ((ed.length()) * 2) + 1;
			char* hexadecimal = new char[hexlen + 1];
			for (size_t i = 0; i < hexlen; i++) {
				hexadecimal[i] = 0;
			}

			hexadecimal[hexlen] = 0;

			char* key = new char[keylen + 1];
			for (size_t i = 0; i < keylen; i++)
			{
				auto finish = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> elapsed = finish - duration;
				char random = (char)(XHASH(((uint64_t)((double)elapsed.count() * 1000)) + i) % 0x100);
				key[i] = random;
			}

			key[keylen] = 0;

			size_t resultbufferlen = hexlen + (keylen * 2) + 1;
			char* resultbuffer = new char[resultbufferlen + 1];
			for (size_t i = 0; i < resultbufferlen + 1; i++) {
				resultbuffer[i] = 0;
			}

			char* keystring = new char[(keylen * 2) + 1];
			keystring[keylen * 2] = 0;

			DTXR(encryptme, key, encryptmelen);
			aob2h(encryptme, hexadecimal, encryptmelen);
			aob2h(key, keystring, keylen);

			joiner(keystring, keylen * 2, hexadecimal, hexlen, resultbuffer);

			std::cout << "\nencrypted: \n";
			for (size_t i = 0; i < resultbufferlen; i++)
			{
				std::cout << resultbuffer[i];
			} std::cout << "\n";

			delete[] keystring;
			delete[] resultbuffer;
			delete[] key;
			delete[] hexadecimal;
			delete[] encryptme;
		}
		else if (isB16 == true) // if b16 then we decrypt
		{
			size_t recbufferlen = ed.length();
			char* recbuffer = new char[ed.length() + 1];
			recbuffer[recbufferlen] = 0;
			for (size_t i = 0; i < recbufferlen; i++)
			{
				recbuffer[i] = ed[i];
			}

			size_t hexlen = GetHexLen(recbuffer, recbufferlen);
			char* hexadecimal = new char[hexlen + 1];
			hexadecimal[hexlen] = 0;

			char keybuffer[keylen * 2];
			char key[keylen];
			splitter(recbuffer, recbufferlen, keybuffer, hexadecimal);
			h2aob(keybuffer, key, keylen);

			char* resbuffer = new char[(hexlen / 2) + 1];
			h2aob(hexadecimal, resbuffer, hexlen / 2);
			DTXR(resbuffer, key, hexlen / 2);

			std::cout << "\ndecrypted:\n";
			for (size_t i = 0; i < hexlen / 2; i++)
			{
				std::cout << resbuffer[i];
			} std::cout << '\n';

			delete[] hexadecimal;
			delete[] resbuffer;
			delete[] recbuffer;
		}
	}
}
