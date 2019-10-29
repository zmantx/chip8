#include "Chip8.h"

int main(int argc, char* argv[])
{
	Chip8 chip8;
	std::string fileName;
	if (argc == 1)
	{
		//fileName = argv[0];
	}
	else
	{
		std::cout << "File name not provided" << std::endl;
		return -1;
	}
	fileName = "C:\\Chip8\\ROMS\\Chip8EmulatorLogo.ch8";
	chip8.load_rom(fileName);

	//sf::Time deltaTime = sf::Time::Zero;
	//sf::Clock clock;
	//while (chip8.running)
	//{
	//	deltaTime = clock.restart();
	//	while (deltaTime.asSeconds() > 1.f / 60.f)
	//	{
	//		deltaTime -= sf::seconds(1.f / 60.f);
	//		chip8.fetch();
	//		chip8.execute();
	//		chip8.draw();
	//	}
	//}
	
	
	sf::Event event;
	sf::Time deltaTime = sf::Time::Zero;
	sf::Clock clock;
	while (true)
	{
		deltaTime += clock.restart();
		if (deltaTime.asSeconds() >= 1.f/60.f)
		{
			chip8.fetch();
			chip8.execute();
			chip8.draw();
			while (chip8.window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					chip8.window.close();
					return 0;
				}
			}
		}
	}
}