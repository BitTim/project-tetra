#ifndef TCS_H
#define TCS_H

#include <string>
#include <vector>
#include <fstream>
#include <SDL2/SDL.h>

#include "tcp_file.hh"
#include "datatypes.hh"
#include "var.hh"

struct Sprite
{
	dword data[16];
};

class Spritesheet
{
public:
	word header;
	word nSprites;
	std::vector<Sprite> sprite_data;

	int load_file(const char* path)
	{
		char tmp_word[2];
		char tmp_dword[4];
		Sprite tmp_sprite;

		std::ifstream sprite_file(path, std::ifstream::binary);

		sprite_file.read(tmp_word, 2);
		header = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(header != 0x5453) return -1;

		sprite_file.read(tmp_word, 2);
		nSprites = ((tmp_word[0] << 8) & 0xFF00) + (tmp_word[1] & 0xFF);
		if(nSprites == 0) return -1;

		for(int n = 0; n < nSprites; n++)
		{
			for(int i = 0; i < 16; i++)
			{
				sprite_file.read(tmp_dword, 4);
				tmp_sprite.data[i] = (((tmp_dword[0] << 24) & 0xFF000000) + ((tmp_dword[1] << 16) & 0x00FF0000) + ((tmp_dword[2] << 8) & 0x0000FF00) + (tmp_dword[3] & 0x000000FF));
			}
			sprite_data.push_back(tmp_sprite);
		}
		sprite_file.close();
		return 0;
	}

	void draw_sprite(SDL_Renderer* renderer, int id, Vec2 pos, Palettelist pal, word cPalette, int pixelscale = _PIXELSCALE)
	{
		for(int y = 0; y < 16; y++)
		{
			for(int x = 0; x < 16; x++)
			{
				iSDL_SetRenderDrawColor(renderer, pal.palettes[cPalette].col[(sprite_data[id].data[y] >> (30 - (x * 2))) & 0x03]);
				SDL_Rect pixel = iSDL_Rect((pos.x + x) * pixelscale, (pos.y + y) * pixelscale, pixelscale, pixelscale);
				SDL_RenderFillRect(renderer, &pixel);
			}
		}
	}
};

#endif /* end of include guard: TCS_H */

/*
File Structure:
0x54 0x53 																																			Header (TS)
0x00 0x04																																				Num Sprites (H byte, L byte)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00	Data Sprite 0 (2 Bit = 1 Pixel)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00	Data Sprite 1 (2 Bit = 1 Pixel)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00	Data Sprite 2 (2 Bit = 1 Pixel)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00	Data Sprite 3 (2 Bit = 1 Pixel)
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00
*/
