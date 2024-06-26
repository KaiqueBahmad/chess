#ifndef PIECE_HPP
#define PIECE_HPP

#include<array>

class Piece {
	private:
		array<char, 5> nome;
	public:
		void setNome(char[5] nome_){
			for (int i = 0; i < arr.size(), i++) {			
				nome[i] = nome_[i];
			}
		}

		array<char, 5> getNome() {
			return nome;
		}
}

#endif
