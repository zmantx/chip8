#include "Chip8.h"
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>

Chip8::Chip8()
{
	// Setup random number generator
	srand(time(NULL));

	// Setup window display
	window.create(sf::VideoMode(640, 320), "Chip 8");

	// Initialize registers
	std::memset(v, 0, 16);
	I = 0;
	delay = 0;
	sound = 0;
	sp = 0;
	pc = 0x200;
	std::memset(stack, 0, 16);

	// Initialize memory
	std::memset(memory, 0, 4096);
	std::memset(videoMem, 0, 2048);

	// Load font data into memory
	// 0
	memory[0x000] = 0xF0; // 11110000 ****
	memory[0x001] = 0x90; // 10010000 *  *
	memory[0x002] = 0x90; // 10010000 *  *
	memory[0x003] = 0x90; // 10010000 *  *
	memory[0x004] = 0xF0; // 11110000 ****

	// 1
	memory[0x005] = 0x20; // 00100000
	memory[0x006] = 0x60; // 01100000
	memory[0x007] = 0x20; // 00100000
	memory[0x008] = 0x20; // 00100000
	memory[0x009] = 0x70; // 01110000

	// 2
	memory[0x00A] = 0xF0; // 11110000
	memory[0x00B] = 0x10; // 00010000
	memory[0x00C] = 0xF0; // 11110000
	memory[0x00D] = 0x80; // 10000000
	memory[0x00E] = 0xF0; // 11110000

	// 3
	memory[0x00F] = 0xF0; // 11110000
	memory[0x010] = 0x10; // 00010000
	memory[0x011] = 0xF0; // 11110000
	memory[0x012] = 0x10; // 00010000
	memory[0x013] = 0xF0; // 11110000

	// 4
	memory[0x014] = 0x90; // 10010000
	memory[0x015] = 0x90; // 10010000
	memory[0x016] = 0xF0; // 11110000
	memory[0x017] = 0x10; // 00010000
	memory[0x018] = 0x10; // 00010000

	// 5
	memory[0x019] = 0xF0; // 11110000
	memory[0x01A] = 0x80; // 10000000
	memory[0x01B] = 0xF0; // 11110000
	memory[0x01C] = 0x10; // 00010000
	memory[0x01D] = 0xF0; // 11110000

	// 6
	memory[0x01E] = 0xF0; // 11110000
	memory[0x01F] = 0x80; // 10000000
	memory[0x020] = 0xF0; // 11110000
	memory[0x021] = 0x90; // 10010000
	memory[0x022] = 0xF0; // 11110000

	// 7
	memory[0x023] = 0xF0; // 11110000
	memory[0x024] = 0x10; // 00010000
	memory[0x025] = 0x20; // 00100000
	memory[0x026] = 0x40; // 01000000
	memory[0x027] = 0x40; // 01000000

	// 8
	memory[0x028] = 0xF0; // 11110000
	memory[0x029] = 0x90; // 10010000
	memory[0x02A] = 0xF0; // 11110000
	memory[0x02B] = 0x90; // 10010000
	memory[0x02C] = 0xF0; // 11110000

	// 9
	memory[0x02D] = 0xF0; // 11110000
	memory[0x02E] = 0x90; // 10010000
	memory[0x02F] = 0xF0; // 11110000
	memory[0x030] = 0x10; // 00010000
	memory[0x031] = 0xF0; // 11110000

	// A
	memory[0x032] = 0xF0; // 11110000
	memory[0x033] = 0x90; // 10010000
	memory[0x034] = 0xF0; // 11110000
	memory[0x035] = 0x90; // 10010000
	memory[0x036] = 0x90; // 10010000

	// B
	memory[0x037] = 0xE0; // 11100000
	memory[0x038] = 0x90; // 10010000
	memory[0x039] = 0xE0; // 11100000
	memory[0x03A] = 0x90; // 10010000
	memory[0x03B] = 0xE0; // 11100000

	// C
	memory[0x03C] = 0xF0; // 11110000
	memory[0x03D] = 0x80; // 10000000
	memory[0x03E] = 0x80; // 10000000
	memory[0x03F] = 0x80; // 10000000
	memory[0x040] = 0xF0; // 11110000

	// D
	memory[0x041] = 0xE0; // 11100000
	memory[0x042] = 0x90; // 10010000
	memory[0x043] = 0x90; // 10010000
	memory[0x044] = 0x90; // 10010000
	memory[0x045] = 0xE0; // 11100000

	// E
	memory[0x046] = 0xF0; // 11110000
	memory[0x047] = 0x80; // 10000000
	memory[0x048] = 0xF0; // 11110000
	memory[0x049] = 0x80; // 10000000
	memory[0x04A] = 0xF0; // 11110000

	// F
	memory[0x04B] = 0xF0; // 11110000
	memory[0x04C] = 0x80; // 10000000
	memory[0x04D] = 0xF0; // 11110000
	memory[0x04E] = 0x80; // 10000000
	memory[0x04F] = 0x80; // 10000000


}

Chip8::~Chip8()
{
	
}

void Chip8::kill()
{
	running = false;
	window.close();
}

void Chip8::fetch()
{
	currentInstruction = 0;
	currentInstruction = memory[pc++];				// Loads MSB of instruction into lower bits of currentInstruction
	currentInstruction = currentInstruction << 8;	// Shift MSB of instruction in MSB of currentInstruction
	currentInstruction += memory[pc++];				// LSB of instruction into lower bits of currentInstruction
}

sf::Keyboard::Key Chip8::chip8ToKeyboard(uint8_t keyToCheck)
{
	switch (keyToCheck)
	{
	case 1:
		return sf::Keyboard::Num1;
	case 2:
		return sf::Keyboard::Num2;
	case 3:
		return sf::Keyboard::Num3;
	case 0xC:
		return sf::Keyboard::Num4;
	case 0x4:
		return sf::Keyboard::Q;
	case 0x5:
		return sf::Keyboard::W;
	case 0x6:
		return sf::Keyboard::E;
	case 0xD:
		return sf::Keyboard::R;
	case 0x7:
		return sf::Keyboard::A;
	case 0x8:
		return sf::Keyboard::S;
	case 0x9:
		return sf::Keyboard::D;
	case 0xE:
		return sf::Keyboard::F;
	case 0xA:
		return sf::Keyboard::Z;
	case 0x0:
		return sf::Keyboard::X;
	case 0xB:
		return sf::Keyboard::C;
	case 0xF:
		return sf::Keyboard::F;
	}
}

void Chip8::execute()
{
	if (delay > 0)
	{
		delay--;
	}

	if (sound > 0)
	{
		// TODO: Implement sound
		sound--;
	}

	// Clear out window events, looking for a close event
	//sf::Event event;
	//while (window.pollEvent(event))
	//{
	//	if (event.type == sf::Event::Closed)
	//	{
	//		kill();
	//	}
	//}
	//if (!running)
	//	return;

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t temp = 0;
	uint8_t hundreds = 0;
	uint8_t tens = 0;
	uint8_t ones = 0;
	uint8_t nibble = 0;
	bool waiting = false;
	switch (currentInstruction & 0xF000)
	{
	case 0x0000:
		switch (currentInstruction & 0x00FF)
		{
		case 0x00E0: // CLS
			clearDisplay();
			break;
		case 0x00EE: // RET
			pc = stack[sp];
			sp--;
			break;
		}
		break;

	case 0x1000: // JP addr
		pc = (currentInstruction & 0x0FFF);
		break;

	case 0x2000: // CALL addr
		sp++;
		stack[sp] = pc;
		pc = (currentInstruction & 0x0FFF);
		break;

	case 0x3000: // SE Vx, byte
		x = (currentInstruction & 0x0F00) >> 8;
		if (v[x] = (currentInstruction & 0x00FF))
		{
			pc += 2;
		}
		break;

	case 0x4000: // SNE Vx, byte
		x = (currentInstruction & 0x0F00) >> 8;
		if (v[x] != (currentInstruction & 0x00FF))
		{
			pc += 2;
		}
		break;

	case 0x5000: // SE Vx, Vy
		x = (currentInstruction & 0x0F00) >> 8;
		y = (currentInstruction & 0x00F0) >> 4;
		if (v[x] == v[y])
		{
			pc += 2;
		}
		break;

	case 0x6000: // LD Vx, byte
		x = (currentInstruction & 0x0F00) >> 8;
		v[x] = currentInstruction & 0x00FF;
		break;

	case 0x7000: // ADD Vx, byte
		x = (currentInstruction & 0x0F00) >> 8;
		temp = v[x];
		v[x] = temp + currentInstruction & 0x00FF;
		break;

	case 0x8000:
		x = (currentInstruction & 0x0F00) >> 8;
		y = (currentInstruction & 0x00F0) >> 4;
		switch (currentInstruction & 0x000F)
		{
		case 0x0000: // LD Vx, Vy
			v[x] = v[y];
			break;

		case 0x0001: // OR Vx, Vy
			v[x] = v[y] | v[x];
			break;

		case 0x0002: // AND Vx, Vy
			v[x] = v[y] & v[x];
			break;

		case 0x0003: // XOR Vx, Vy
			v[x] = v[y] ^ v[x];
			break;

		case 0x0004: // ADD Vx, Vy
			temp = v[x] + v[y];
			if (temp > 255)
				v[0xF] = 1;
			else
				v[0xF] = 0;
			v[x] = temp & 0xFF;
			break;

		case 0x0005: // SUB Vx, Vy
			if (v[x] > v[y])
				v[0xF] = 1;
			else
				v[0xF] = 0;
			v[x] = v[x] - v[y];
			break;

		case 0x0006: // SHR Vx {, Vy}
			if ((v[x] & 0x1) == 0x1)
				v[0xF] = 1;
			else
				v[0xF] = 0;
			v[x] = v[x] >> 1;
			break;

		case 0x0007: // SUBN Vx, Vy
			if (v[y] > v[x])
				v[0xF] = 1;
			else
				v[0xF] = 0;
			v[x] = v[y] - v[x];
			break;

		case 0x000E: // SHL Vx {, Vy}
			if ((v[x] & 0x80) == 0x80)
			{
				v[0xF] = 1;
			}
			else
				v[0xF] = 0;
			v[x] = v[x] << 1;
			break;
		}
		break;

	case 0x9000: // SNE Vx, Vy
		x = (currentInstruction & 0x0F00) >> 8;
		y = (currentInstruction & 0x00F0) >> 4;
		if (v[x] != v[y])
			pc += 2;
		break;

	case 0xA000: // LD I, addr
		I = currentInstruction & 0x0FFF;
		break;

	case 0xB000: // JP V0, addr
		pc = (currentInstruction & 0x0FFF) + v[0];
		break;

	case 0xC000: // RND Vx, byte
		x = (currentInstruction & 0x0F00) >> 8;
		v[x] = (currentInstruction & 0x00FF) & (rand() % 256);
		break;

	case 0xD000: // DRW Vx, Vy, nibble
		x = v[(currentInstruction & 0x0F00) >> 8];
		y = v[(currentInstruction & 0x00F0) >> 4];
		nibble = currentInstruction & 0x000F;
		v[15] = 0;
		for (int i = 0; i < nibble; i++)
		{
			uint8_t currentByte = memory[I + i];
			for (int j = 0; j < 8; j++)
			{
				if ((currentByte & (0x80 >> j)) == 1 && (videoMem[x + (y + i) * 64] & (0x80)) >> j == 1)
					v[15] = 1;
			}
			videoMem[x + (y + i) * 64] ^= currentByte;
		}

		break;

	case 0xE000:
		switch (currentInstruction & 0x00FF)
		{
		case 0x009E: // SKP Vx
			x = (currentInstruction & 0x0F00) >> 8;
			if (sf::Keyboard::isKeyPressed(chip8ToKeyboard(v[x])))
			{
				pc += 2;
			}
			break;

		case 0x00A1: // SKNP Vx
			x = currentInstruction & 0x0F00;
			if (!sf::Keyboard::isKeyPressed(chip8ToKeyboard(v[x])))
			{
				pc += 2;
			}
			break;
		}
		break;

	case 0xF000:
		x = (currentInstruction & 0x0F00) >> 8;
		switch (currentInstruction & 0x00FF)
		{
		case 0x0007: // LD Vx, DT
			v[x] = delay;
			break;

		case 0x000A: // LD Vx, K
			x = (currentInstruction & 0x0F00) >> 8;
			waiting = true;
			while (waiting)
			{
				for (uint8_t i = 0; i < 16; i++)
				{
					if (sf::Keyboard::isKeyPressed(chip8ToKeyboard(i)))
					{
						v[x] = i;
						waiting = false;
						break;
					}
				}
			}
			break;

		case 0x0015: // LD DT, Vx
			delay = v[x];
			break;

		case 0x0018:
			sound = v[x];
			break;

		case 0x001E: // ADD I, Vx
			I = I + v[x];
			break;

		case 0x0029: // LD F, Vx
			I = v[x] * 5; // Each sprite in memory is 5 bytes long, starting at 0x000
			break;

		case 0x0033: // LD B, Vx
			temp = v[x];
			hundreds = (uint8_t)(temp / 100);
			temp -= hundreds * 100;
			tens = (uint8_t)(temp / 10);
			temp -= tens * 10;
			ones = temp;
			memory[I] = hundreds;
			memory[I + 1] = tens;
			memory[I + 2] = ones;
			break;

		case 0x0055: // LD [I], Vx
			for (uint8_t i = 0; i <= x; i++)
			{
				memory[I + i] = v[i];
			}
			break;

		case 0x0065: // LD Vx, [I]
			for (uint8_t i = 0; i <= x; i++)
			{
				v[i] = memory[I + i];
			}
			break;
		}
	}
}

void Chip8::clearDisplay()
{
	memset(videoMem, 0, 2048);
}

void Chip8::load_rom(std::string fileName)
{
	std::ifstream file;
	size_t file_size;

	file.open(fileName, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		file.seekg(0, std::ios::end);
		file_size = file.tellg();
		if (file_size > 0xFFF - 0x200)
		{
			// ROM is too big
		}
		else
		{
			char* data_rom = (char*)(&(memory[0x200]));
			file.seekg(0, std::ios::beg);
			file.read(data_rom, file_size);
		}
		file.close();
	}
}

void Chip8::draw()
{
	window.clear(sf::Color::Black);
	sf::RectangleShape shape(sf::Vector2f(10.f, 10.f));
	shape.setFillColor(sf::Color::White);
	for (int y = 0; y < 32; y++)
	{
		for (int x = 0; x < 64; x++)
		{
			if (videoMem[y * 64 + x] > 0)
			{
				shape.setPosition(x * 10, y * 10);
				window.draw(shape);
			}
		}
	}
	window.display();
}