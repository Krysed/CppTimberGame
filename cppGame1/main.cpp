#include <SFML/Graphics.hpp>


int main()
{
	sf::VideoMode vm(1920, 1080);
	sf::RenderWindow window(vm, "Timber!", sf::Style::Fullscreen);




	while (window.isOpen())
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}
	}
	return 0;
}