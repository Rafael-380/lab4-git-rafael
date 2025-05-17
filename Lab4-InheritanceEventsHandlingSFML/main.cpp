#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>  // Para gerar numeros aleatorios

// Classe CustomRectangleShape que herda de sf::RectangleShape
class CustomRectangleShape : public sf::RectangleShape {
public:
    // Construtor com inicialização de tamanho e posição
    CustomRectangleShape(const sf::Vector2f &size, const sf::Vector2f &position)
        : sf::RectangleShape(size), m_speed_x(0), m_speed_y(0), m_rotation_speed(0),
        m_left_bound(0), m_right_bound(800), m_top_bound(0), m_bottom_bound(600), m_isSelected(false) {
        setPosition(position);
        setFillColor(sf::Color::Green); // Cor inicial
    }

    // Define a velocidade e a rotação
    void setSpeed(float speed_x, float speed_y, float rotation_speed) {
        m_speed_x = speed_x;
        m_speed_y = speed_y;
        m_rotation_speed = rotation_speed;
    }

    // Método para mover por teclas, com valor fixo de deslocamento
    void moveInDirection(const sf::Keyboard::Key &key) {
        if (!m_isSelected) return;

        float dx = 0, dy = 0;

        if (key == sf::Keyboard::Up) dy = -m_speed_y;
        else if (key == sf::Keyboard::Down) dy = m_speed_y;
        else if (key == sf::Keyboard::Left) dx = -m_speed_x;
        else if (key == sf::Keyboard::Right) dx = m_speed_x;

        move(dx, dy);

        // Garantir que o retângulo não ultrapasse os limites da janela
        sf::Vector2f pos = getPosition();
        if (pos.x < m_left_bound) setPosition(m_left_bound, pos.y);
        if (pos.x + getSize().x > m_right_bound) setPosition(m_right_bound - getSize().x, pos.y);
        if (pos.y < m_top_bound) setPosition(pos.x, m_top_bound);
        if (pos.y + getSize().y > m_bottom_bound) setPosition(pos.x, m_bottom_bound - getSize().y);
    }

    // Método para verificar se o retângulo foi clicado
    bool isClicked(sf::Vector2i &mouse_position) const {
        sf::FloatRect bounds = getGlobalBounds();
        return bounds.contains(static_cast<float>(mouse_position.x), static_cast<float>(mouse_position.y));
    }

    // Método para gerar uma cor aleatória
    sf::Color getRandomColor() {
        return sf::Color(rand() % 256, rand() % 256, rand() % 256);
    }

    // Definir os limites de movimentação do retângulo
    void setBounds(float left, float right, float top, float bottom) {
        m_left_bound = left;
        m_right_bound = right;
        m_top_bound = top;
        m_bottom_bound = bottom;
    }

    // Método para selecionar/deselecionar o retângulo
    void setSelected(bool isSelected) {
        m_isSelected = isSelected;
        if (m_isSelected) {
            setFillColor(getRandomColor()); // mudar para cor aleatoria ao clicar
        } else {
            setFillColor(sf::Color::Green);   // Cor padrão para retângulo não selecionado
        }
    }

    // Método para verificar se o retângulo está selecionado
    bool isSelected() const {
        return m_isSelected;
    }

private:
    float m_speed_x;
    float m_speed_y;
    float m_rotation_speed;

    float m_left_bound;
    float m_right_bound;
    float m_top_bound;
    float m_bottom_bound;

    bool m_isSelected;  // Campo para controlar se o retângulo está selecionado
};

int main() {
    // Criação da janela
    sf::RenderWindow window(sf::VideoMode(800, 600), "Inheritance Events SFML");

    // Criação dos retângulos com posições aleatórias
    std::srand(std::time(nullptr));
    std::vector<CustomRectangleShape> rectangles;

    // Adicionar 10 retângulos ao vetor
    for (int i = 0; i < 10; i++) {
        sf::Vector2f size(120.0f, 60.0f);
        sf::Vector2f position(std::rand() % (window.getSize().x - 120), std::rand() % (window.getSize().y - 60));
        rectangles.emplace_back(CustomRectangleShape(size, position));
    }

    // Configurações dos retângulos
    for (auto &rec : rectangles) {
        rec.setBounds(0, window.getSize().x, 0, window.getSize().y);
        rec.setSpeed(10, 10, 0);  // Agora representa "quantos pixels por tecla"
    }

    // Variável para armazenar o retângulo selecionado
    CustomRectangleShape* selectedRectangle = nullptr;

    // Laço principal
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Verifica clique do mouse
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                    bool clicked = false;

                    for (auto &rec : rectangles) {
                        if (rec.isClicked(mouse_pos)) {
                            if (selectedRectangle != &rec) {
                                if (selectedRectangle != nullptr)
                                    selectedRectangle->setSelected(false);

                                rec.setSelected(true);
                                selectedRectangle = &rec;
                            }
                            clicked = true;
                            break;
                        }
                    }

                    if (!clicked && selectedRectangle != nullptr) {
                        selectedRectangle->setSelected(false);
                        selectedRectangle = nullptr;
                    }
                }
            }

            // Movimento por teclado (agora sem elapsed)
            if (event.type == sf::Event::KeyPressed && selectedRectangle != nullptr) {
                selectedRectangle->moveInDirection(event.key.code);
            }
        }

        window.clear(sf::Color::Black);
        for (auto &rec : rectangles) {
            window.draw(rec);
        }
        window.display();
    }

    return 0;
}


