#include "stdafx.h"
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "RatLair.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include <random>
#include <time.h>

/*The Rat Catchers Revenge
Student project created by Mark Tasaka base on John Horton’s 'Zombie Arena',
from */

using namespace sf;


int getLairDimensions(int baseSize)
{
	//srand(time(0));
	default_random_engine generator;
	uniform_int_distribution<int> distribution(5, 12);

	int number = distribution(generator);

	int dimenionSize = number * baseSize;

	return dimenionSize;

}


//Main: program entry
int main()
{
	// Here is the instance of TextureHolder
	TextureHolder holder;

	// The game will always be in one of four states
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// Start with the GAME_OVER state
	State state = State::GAME_OVER;


	// Get the screen resolution and create an SFML window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Rat Lair", Style::Fullscreen);

	// Create a an SFML View for the main action
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	// Here is our clock for timing everything
	Clock clock;
	// How long has the PLAYING state been active
	Time gameTimeTotal;

	// Where is the mouse in relation to world coordinates
	Vector2f mouseWorldPosition;
	// Where is the mouse in relation to screen coordinates
	Vector2i mouseScreenPosition;

	// Create an instance of the Player class
	Player player;

	// The boundaries of the arena
	IntRect arena;

	// Create the background
	VertexArray background;
	// Load the texture for our background vertex array
	Texture textureBackground = TextureHolder::GetTexture(
		"graphics/background_sheet.png");

	// Prepare for a horde of rats
	int numRats;
	int numRatsAlive;
	Rat* rats = NULL;

	// 100 bullets should do
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 48;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	// When was the fire button last pressed?
	Time lastPressed;

	// Hide the mouse pointer and replace it with crosshair
	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	// Create a couple of pickups
	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	// About the game
	int score = 0;
	int hiScore = 0;

	// For the home/game over screen
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.png");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);


	// Create a view for the HUD
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));


	// Create a sprite for the ammo icon
	Sprite spriteAmmoIcon6;
	Texture textureAmmoIcon6 = TextureHolder::GetTexture("graphics/ammo_icon6.png");
	spriteAmmoIcon6.setTexture(textureAmmoIcon6);
	spriteAmmoIcon6.setPosition(20, 980);

	Sprite spriteAmmoIcon5;
	Texture textureAmmoIcon5 = TextureHolder::GetTexture("graphics/ammo_icon5.png");
	spriteAmmoIcon5.setTexture(textureAmmoIcon5);
	spriteAmmoIcon5.setPosition(20, 980);

	Sprite spriteAmmoIcon4;
	Texture textureAmmoIcon4 = TextureHolder::GetTexture("graphics/ammo_icon4.png");
	spriteAmmoIcon4.setTexture(textureAmmoIcon4);
	spriteAmmoIcon4.setPosition(20, 980);

	Sprite spriteAmmoIcon3;
	Texture textureAmmoIcon3 = TextureHolder::GetTexture("graphics/ammo_icon3.png");
	spriteAmmoIcon3.setTexture(textureAmmoIcon3);
	spriteAmmoIcon3.setPosition(20, 980);

	Sprite spriteAmmoIcon2;
	Texture textureAmmoIcon2 = TextureHolder::GetTexture("graphics/ammo_icon2.png");
	spriteAmmoIcon2.setTexture(textureAmmoIcon2);
	spriteAmmoIcon2.setPosition(20, 980);

	Sprite spriteAmmoIcon1;
	Texture textureAmmoIcon1 = TextureHolder::GetTexture("graphics/ammo_icon1.png");
	spriteAmmoIcon1.setTexture(textureAmmoIcon1);
	spriteAmmoIcon1.setPosition(20, 980);

	Sprite spriteAmmoIcon0;
	Texture textureAmmoIcon0 = TextureHolder::GetTexture("graphics/ammo_icon0.png");
	spriteAmmoIcon0.setTexture(textureAmmoIcon0);
	spriteAmmoIcon0.setPosition(20, 980);

	// Load the font
	Font font;
	font.loadFromFile("fonts/vcrOsdMono.ttf");


	// Paused
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	// Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(35);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(155, 990);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	// Load the high score from a text file/
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hi Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	// Rats remaining
	Text ratsRemainingText;
	ratsRemainingText.setFont(font);
	ratsRemainingText.setCharacterSize(55);
	ratsRemainingText.setFillColor(Color::White);
	ratsRemainingText.setPosition(1500, 980);
	ratsRemainingText.setString("Rats: 100");

	// Wave number
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	// Health bar
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);


	// When did we last update the HUD?
	int framesSinceLastHUDUpdate = 0;
	// What time was the last update
	Time timeSinceLastUpdate;
	// How often (in frames) should we update the HUD
	int fpsMeasurementFrameInterval = 1000;

	// Prepare the hit sound
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// Prepare the splat sound
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	sf::Sound splat;
	splat.setBuffer(splatBuffer);

	// Prepare the shoot sound
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// Prepare the reload sound
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	// Prepare the failed sound
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	// Prepare the powerup sound
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// Prepare the pickup sound
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	// The main game loop
	while (window.isOpen())
	{
		/*
		************
		Handle input
		************
		*/

		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Return &&
					state == State::PLAYING)
				{
					state = State::PAUSED;
				}

				// Restart while paused
				else if (event.key.code == Keyboard::Return &&
					state == State::PAUSED)
				{
					state = State::PLAYING;
					// Reset the clock so there isn't a frame jump
					clock.restart();
				}

				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
					wave = 0;
					score = 0;

					// Prepare the gun and ammo for next game
					currentBullet = 0;
					bulletsSpare = 48;
					bulletsInClip = 6;
					clipSize = 6;
					fireRate = 1;

					// Reset the player's stats
					player.resetPlayerStats();
				}

				if (state == State::PLAYING)
				{
					// Reloading
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							// Plenty of bullets. Reload.
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
							reload.play();
						}
						else if (bulletsSpare > 0)
						{
							// Only few bullets left
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else
						{
							reloadFailed.play();
						}
					}

				}

			}
		}// End event polling


		 // Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		// Handle controls while playing
		if (state == State::PLAYING)
		{
			/* Handle the pressing and releasing of the WASD keys of arrow keys*/
			if (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Down))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}
			// Fire a bullet
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{

				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 500 / fireRate && bulletsInClip > 0)
				{

					// Pass the centre of the player 
					// and the centre of the cross-hair
					// to the shoot function
					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;

					bulletsInClip--;
				}

			}// End fire a bullet


		}// End WASD while playing

		 // Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num5)
			{
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num6)
			{
				state = State::PLAYING;
			}

			if (state == State::PLAYING)
			{
				// Prepare thelevel
				// We will modify the next two lines later
				//arena.width = 1000;
				
				arena.width = getLairDimensions(LAIR_BASE_SIZE);
				arena.height = getLairDimensions(LAIR_BASE_SIZE);
				arena.left = 0;
				arena.top = 0;

				// Pass the vertex array by reference 
				// to the createBackground function
				int tileSize = createBackground(background, arena);

				// Spawn the player in the middle of the arena
				player.spawn(arena, resolution, tileSize);

				// Configure the pick-ups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				// Create a horde of rats
				numRats = 20;

				// Delete the previously allocated memory (if it exists)
				delete[] rats;
				rats = createHorde(numRats, arena);
				numRatsAlive = numRats;

				// Reset the clock so there isn't a frame jump
				clock.restart();
			}
		}// End levelling up

		 /*
		 ****************
		 UPDATE THE FRAME
		 ****************
		 */
		if (state == State::PLAYING)
		{
			// Update the delta time
			Time dt = clock.restart();
			// Update the total game time
			gameTimeTotal += dt;
			// Make a decimal fraction of 1 from the delta time
			float dtAsSeconds = dt.asSeconds();

			// Where is the mouse pointer
			mouseScreenPosition = Mouse::getPosition();

			// Convert mouse position to world coordinates of mainView
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);

			// Set the crosshair to the mouse world location
			spriteCrosshair.setPosition(mouseWorldPosition);

			// Update the player
			player.update(dtAsSeconds, Mouse::getPosition());


			//set the ammo player has
			//string bulletSpare = to_string(bulletsSpare);
			//ammoText.setString(bulletSpare);

			// Make a note of the players new position
			Vector2f playerPosition(player.getCenter());

			// Make the view centre around the player				
			mainView.setCenter(player.getCenter());

			// Loop through each Rat and update them
			for (int i = 0; i < numRats; i++)
			{
				if (rats[i].isAlive())
				{
					rats[i].update(dt.asSeconds(), playerPosition);
				}
			}

			// Update any bullets that are in-flight
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}

			// Update the pickups
			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);

			// Collision detection
			// Have any rats been shot?
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numRats; j++)
				{
					if (bullets[i].isInFlight() &&
						rats[j].isAlive())
					{
						if (bullets[i].getPosition().intersects
						(rats[j].getPosition()))
						{
							// Stop the bullet
							bullets[i].stop();

							// Register the hit and see if it was a kill
							if (rats[j].hit()) {
								// Not just a hit but a kill too
								score += 10;
								if (score >= hiScore)
								{
									hiScore = score;
								}

								numRatsAlive--;

								// When all the rats are dead (again)
								if (numRatsAlive == 0) {
									state = State::LEVELING_UP;
								}
							}
							// Make a splat sound
							splat.play();

						}
					}

				}
			}// End Rat being shot

			 // Have any rats touched the player			
			for (int i = 0; i < numRats; i++)
			{
				if (player.getPosition().intersects
				(rats[i].getPosition()) && rats[i].isAlive())
				{

					if (player.hit(gameTimeTotal))
					{
						hit.play();
					}

					if (player.getHealth() <= 0)
					{
						state = State::GAME_OVER;
						std::ofstream outputFile("gamedata/scores.txt");
						outputFile << hiScore;
						outputFile.close();

					}
				}
			}// End player touched

			 // Has the player touched health pickup
			if (player.getPosition().intersects
			(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());
				// Play a sound
				pickup.play();

			}

			// Has the player touched ammo pickup
			if (player.getPosition().intersects
			(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();
				// Play a sound
				reload.play();

			}

			// size up the health bar
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			// Increment the amount of time since the last HUD update
			timeSinceLastUpdate += dt;
			// Increment the number of frames since the last HUD calculation
			framesSinceLastHUDUpdate++;
			// Calculate FPS every fpsMeasurementFrameInterval frames
			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{

				// Update game HUD text
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssRatsAlive;

				// Update the ammo text
				ssAmmo << bulletsSpare;
				//ssAmmo << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				// Update the score text
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// Update the high score text
				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				// Update the wave
				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				// Update the high score text
				ssRatsAlive << "Rats:" << numRatsAlive;
				ratsRemainingText.setString(ssRatsAlive.str());

				framesSinceLastHUDUpdate = 0;
				timeSinceLastUpdate = Time::Zero;
			}// End HUD update


			}// End updating the scene

			 /*
			 **************
			 Draw the scene
			 **************
			 */

			if (state == State::PLAYING)
			{
				window.clear();

				// set the mainView to be displayed in the window
				// And draw everything related to it
				window.setView(mainView);

				// Draw the background
				window.draw(background, &textureBackground);

				// Draw the rats
				for (int i = 0; i < numRats; i++)
				{
					window.draw(rats[i].getSprite());
				}

				for (int i = 0; i < 100; i++)
				{
					if (bullets[i].isInFlight())
					{
						window.draw(bullets[i].getShape());
					}
				}


				// Draw the player
				window.draw(player.getSprite());

				// Draw the pick-ups, if currently spawned
				if (ammoPickup.isSpawned())
				{
					window.draw(ammoPickup.getSprite());
				}
				if (healthPickup.isSpawned())
				{
					window.draw(healthPickup.getSprite());
				}


				//Draw the crosshair
				window.draw(spriteCrosshair);


				window.setView(hudView);

				// Draw all the HUD elements
				window.draw(scoreText);
				window.draw(hiScoreText);
				window.draw(healthBar);
				window.draw(waveNumberText);
				window.draw(ratsRemainingText);

				if (bulletsInClip == 6)
				{
					window.draw(spriteAmmoIcon6);
				}
				else if (bulletsInClip == 5)
				{
					window.draw(spriteAmmoIcon5);
				}
				else if (bulletsInClip == 4)
				{
					window.draw(spriteAmmoIcon4);
				}
				else if (bulletsInClip == 3)
				{
					window.draw(spriteAmmoIcon3);
				}
				else if (bulletsInClip == 2)
				{
					window.draw(spriteAmmoIcon2);
				}
				else if (bulletsInClip == 1)
				{
					window.draw(spriteAmmoIcon1);
				}
				else
				{
					window.draw(spriteAmmoIcon0);
				}


				window.draw(ammoText);

			}

			if (state == State::LEVELING_UP)
			{
				window.draw(spriteGameOver);
				window.draw(levelUpText);
			}

			if (state == State::PAUSED)
			{
				window.draw(pausedText);
			}

			if (state == State::GAME_OVER)
			{
				window.draw(spriteGameOver);
				window.draw(gameOverText);
				window.draw(scoreText);
				window.draw(hiScoreText);
			}

			window.display();

		//end if loop

	}// End game loop

	return 0;

}




