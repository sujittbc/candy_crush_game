#include <SFML/Graphics.hpp>
#include <time.h>
#include <fstream>
#include <vector>
using namespace sf;

int ts = 54; // tile size
Vector2i offset(48, 24); // grid offset for drawing

// Abstract base class for drawable and updatable game objects
class GameObject {
public:
    virtual void update() = 0;
    virtual void draw(RenderWindow& window) = 0;
    virtual ~GameObject() {}
};

// Represents a single gem or tile on the board
class Piece : public GameObject {
public:
    int x, y, col, row, kind, match, alpha;
    Sprite sprite;

    Piece() { match = 0; alpha = 255; }

    // Set piece properties
    void set(int kind, int row, int col) {
        this->kind = kind;
        this->row = row;
        this->col = col;
        this->x = col * ts;
        this->y = row * ts;
        match = 0;
        alpha = 255;
    }

    void update() override {}

    // Draw the gem sprite
    void draw(RenderWindow& window) override {
        sprite.setTextureRect(IntRect(kind * 49, 0, 49, 49));
        sprite.setColor(Color(255, 255, 255, alpha));
        sprite.setPosition(x, y);
        sprite.move(offset.x - ts, offset.y - ts);
        window.draw(sprite);
    }
};

Piece grid[10][10]; // 8x8 board with padding
int score = 0, moves = 0, level = 1;
int cursorRow = 1, cursorCol = 1; // keyboard navigation cursor

// Swap positions of two pieces
void swap(Piece& p1, Piece& p2) {
    std::swap(p1.col, p2.col);
    std::swap(p1.row, p2.row);
    std::swap(grid[p1.row][p1.col], grid[p2.row][p2.col]);
}

// Save game data to file
void saveGame() {
    std::ofstream out("save.txt");
    out << score << " " << moves << " " << level << "\n";
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++)
            out << grid[i][j].kind << " ";
    out.close();
}

// Load game data from file
void loadGame() {
    std::ifstream in("save.txt");
    if (!in.is_open()) return;

    in >> score >> moves >> level;
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++) {
            int kind;
            in >> kind;
            grid[i][j].set(kind, i, j);
        }
    in.close();
}

// Calculate score based on horizontal and vertical matches
void updateScore() {
    bool counted[10][10] = { false };

    // Horizontal match check
    for (int i = 1; i <= 8; i++) {
        int j = 1;
        while (j <= 8) {
            int matchLen = 1;
            int kind = grid[i][j].kind;
            while (j + matchLen <= 8 && grid[i][j + matchLen].kind == kind)
                matchLen++;

            if (matchLen >= 3) {
                int points = (matchLen == 3) ? 10 : (matchLen == 4) ? 20 : 30;
                score += points;
                for (int k = 0; k < matchLen; k++)
                    counted[i][j + k] = true;
            }
            j += matchLen;
        }
    }

    // Vertical match check
    for (int j = 1; j <= 8; j++) {
        int i = 1;
        while (i <= 8) {
            int matchLen = 1;
            int kind = grid[i][j].kind;
            while (i + matchLen <= 8 && grid[i + matchLen][j].kind == kind)
                matchLen++;

            if (matchLen >= 3) {
                int points = (matchLen == 3) ? 10 : (matchLen == 4) ? 20 : 30;
                score += points;
                for (int k = 0; k < matchLen; k++)
                    counted[i + k][j] = true;
            }
            i += matchLen;
        }
    }

    // Level up condition
    if (score >= 100 && level == 1) level = 2;
}

int main() {
    srand(time(0));
    RenderWindow app(VideoMode(740, 480), "Match-3 Game with Inheritance");
    app.setFramerateLimit(60);

    // Load textures and background
    Texture backgroundTex, gemsTex;
    backgroundTex.loadFromFile("assets/background.png");
    gemsTex.loadFromFile("assets/gems.png");
    Sprite background(backgroundTex);

    // Setup text
    Font font;
    font.loadFromFile("assets/font.ttf");
    Text scoreText("", font, 20);
    Text movesText("", font, 20);
    Text levelText("", font, 20);
    scoreText.setFillColor(Color::Black);
    movesText.setFillColor(Color::Black);
    levelText.setFillColor(Color::Black);
    int tileRightX = offset.x + ts * 8;
    int tileBottomY = offset.y + ts * 7;
    scoreText.setPosition(tileRightX + 10, tileBottomY);
    movesText.setPosition(tileRightX + 10, tileBottomY + 20);
    levelText.setPosition(tileRightX + 10, tileBottomY + 40);

    // Initialize board with random pieces
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 8; j++) {
            grid[i][j].set(rand() % 7, i, j);
            grid[i][j].sprite.setTexture(gemsTex);
        }

    int x0, y0, x, y, click = 0;
    Vector2i pos;
    bool isSwap = false, isMoving = false;

    while (app.isOpen()) {
        Event e;
        while (app.pollEvent(e)) {
            if (e.type == Event::Closed)
                app.close();

            // Handle mouse clicks
            if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                if (!isSwap && !isMoving) click++;
                pos = Mouse::getPosition(app) - offset;
            }

            // Keyboard input for cursor movement and save/load
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::W && cursorRow > 1) cursorRow--;
                if (e.key.code == Keyboard::S && cursorRow < 8) cursorRow++;
                if (e.key.code == Keyboard::A && cursorCol > 1) cursorCol--;
                if (e.key.code == Keyboard::D && cursorCol < 8) cursorCol++;
                if (e.key.code == Keyboard::K) saveGame();
                if (e.key.code == Keyboard::L) loadGame();
            }
        }

        // First and second click logic
        if (click == 1) {
            x0 = pos.x / ts + 1;
            y0 = pos.y / ts + 1;
        }
        if (click == 2) {
            x = pos.x / ts + 1;
            y = pos.y / ts + 1;
            if (abs(x - x0) + abs(y - y0) == 1) {
                swap(grid[y0][x0], grid[y][x]);
                isSwap = true;
                moves++;
                click = 0;
            } else click = 1;
        }

        // Mark matching pieces
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++) {
                if (grid[i][j].kind == grid[i + 1][j].kind && grid[i][j].kind == grid[i - 1][j].kind)
                    for (int n = -1; n <= 1; n++) grid[i + n][j].match++;
                if (grid[i][j].kind == grid[i][j + 1].kind && grid[i][j].kind == grid[i][j - 1].kind)
                    for (int n = -1; n <= 1; n++) grid[i][j + n].match++;
            }

        // Move pieces to target positions
        isMoving = false;
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++) {
                Piece& p = grid[i][j];
                int dx = p.x - p.col * ts;
                int dy = p.y - p.row * ts;
                if (dx) p.x -= dx / abs(dx);
                if (dy) p.y -= dy / abs(dy);
                if (dx || dy) isMoving = true;
            }

        // Fade matched pieces
        if (!isMoving)
            for (int i = 1; i <= 8; i++)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match && grid[i][j].alpha > 10) {
                        grid[i][j].alpha -= 10;
                        isMoving = true;
                    }

        // After swap, update score
        if (isSwap && !isMoving) {
            updateScore();
            isSwap = false;
        }

        // Drop and refill matched pieces
        if (!isMoving) {
            for (int i = 8; i > 0; i--)
                for (int j = 1; j <= 8; j++)
                    if (grid[i][j].match)
                        for (int n = i; n > 0; n--)
                            if (!grid[n][j].match) {
                                swap(grid[n][j], grid[i][j]);
                                break;
                            }

            for (int j = 1; j <= 8; j++)
                for (int i = 8, n = 0; i > 0; i--)
                    if (grid[i][j].match) {
                        grid[i][j].set(rand() % 7, i, j);
                        grid[i][j].y = -ts * n++;
                    }
        }

        app.clear();
        app.draw(background);

        // Draw all pieces
        for (int i = 1; i <= 8; i++)
            for (int j = 1; j <= 8; j++)
                grid[i][j].draw(app);

        // Draw cursor box
        RectangleShape cursorBox(Vector2f(ts - 4, ts - 4));
        cursorBox.setPosition(cursorCol * ts + offset.x - ts + 2, cursorRow * ts + offset.y - ts + 2);
        cursorBox.setFillColor(Color::Transparent);
        cursorBox.setOutlineThickness(3);
        cursorBox.setOutlineColor(Color::Red);
        app.draw(cursorBox);

        // Update UI text
        scoreText.setString("Score: " + std::to_string(score));
        movesText.setString("Moves: " + std::to_string(moves));
        levelText.setString("Level: " + std::to_string(level));
        app.draw(scoreText);
        app.draw(movesText);
        app.draw(levelText);

        app.display();
    }
    return 0;
}
