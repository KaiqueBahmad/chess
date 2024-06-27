#include <iostream>
#include <string>

class Pessoa {
private:
    std::string nome;

public:
    void setNome(const std::string& novoNome) {
        nome = novoNome;
    }
    std::string getNome() const {
        return nome;
    }

};

int main() {
    Pessoa p;
    p.setNome("João");
    std::cout << "Nome: " << p.getNome() << std::endl;
    return 0;
}