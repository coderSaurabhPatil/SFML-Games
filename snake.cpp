#include <SFML/Graphics.hpp>
#include <ctime>
#include <vector>

// Constants for game dimensions
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 20;
const int GRID_WIDTH = WINDOW_WIDTH / CELL_SIZE;
const int GRID_HEIGHT = WINDOW_HEIGHT / CELL_SIZE;

// Directions for the snake
enum Direction { UP, DOWN, LEFT, RIGHT };

// Main application
int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Snake Game in SFML");
    window.setFramerateLimit(10);

    // Seed for random number generation
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Snake initialization
    std::vector<sf::Vector2i> snake = { {GRID_WIDTH / 2, GRID_HEIGHT / 2} };
    Direction dir = RIGHT;

    // Food initialization
    sf::Vector2i food(std::rand() % GRID_WIDTH, std::rand() % GRID_HEIGHT);

    // Cell shapes
    sf::RectangleShape cellShape(sf::Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Handle keyboard input for direction change
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W && dir != DOWN) dir = UP;
                else if (event.key.code == sf::Keyboard::S && dir != UP) dir = DOWN;
                else if (event.key.code == sf::Keyboard::A && dir != RIGHT) dir = LEFT;
                else if (event.key.code == sf::Keyboard::D && dir != LEFT) dir = RIGHT;
            }
        }

        // Move the snake
        sf::Vector2i newHead = snake.front();
        if (dir == UP) newHead.y -= 1;
        else if (dir == DOWN) newHead.y += 1;
        else if (dir == LEFT) newHead.x -= 1;
        else if (dir == RIGHT) newHead.x += 1;

        // Wrap around the edges of the screen
        if (newHead.x < 0) newHead.x = GRID_WIDTH - 1;
        else if (newHead.x >= GRID_WIDTH) newHead.x = 0;
        if (newHead.y < 0) newHead.y = GRID_HEIGHT - 1;
        else if (newHead.y >= GRID_HEIGHT) newHead.y = 0;

        // Check for collision with itself
        for (const auto& segment : snake) {
            if (newHead == segment) {
                window.close(); // Game over
                return 0;
            }
        }

        // Add new head
        snake.insert(snake.begin(), newHead);

        // Check for food collection
        if (newHead == food) {
            food = sf::Vector2i(std::rand() % GRID_WIDTH, std::rand() % GRID_HEIGHT);
        } else {
            snake.pop_back(); // Remove tail if no food collected
        }

        // Render everything
        window.clear();

        // Draw the snake
        cellShape.setFillColor(sf::Color::Green);
        for (const auto& segment : snake) {
            cellShape.setPosition(segment.x * CELL_SIZE, segment.y * CELL_SIZE);
            window.draw(cellShape);
        }

        // Draw the food
        cellShape.setFillColor(sf::Color::Red);
        cellShape.setPosition(food.x * CELL_SIZE, food.y * CELL_SIZE);
        window.draw(cellShape);

        window.display();
    }

    return 0;
}

