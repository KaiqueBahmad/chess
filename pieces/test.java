


interface Piece {
    String getIdentifier();
    Piece[][] possibleNextStates();
}

public Knight implements Piece {
    

    public String getIdentifier() {
        return "Knight";
    }

    public Piece[][] possibleMoves() {
        Piece[8][8] nextState = new Piece[][];
        ...
        return nextState;
    }

}


