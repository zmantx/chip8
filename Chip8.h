#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <map>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

class Chip8
{
public:
	Chip8();
	~Chip8();
	void load_rom(std::string fileName);
	void kill();
	bool running = true;
	void fetch();
	void execute();
	void draw();
	sf::RenderWindow window;
	void clearDisplay();
	sf::Keyboard::Key chip8ToKeyboard(uint8_t keyToCheck);
	uint8_t memory[4096];
	uint8_t v[16];
	uint16_t I;
	uint8_t delay;
	uint8_t sound;
	uint8_t sp;
	uint16_t pc;
	uint16_t stack[16];
	uint16_t currentInstruction;
	uint8_t videoMem[2048];
	
};