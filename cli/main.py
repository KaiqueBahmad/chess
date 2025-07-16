import ctypes
import random
import os

# Load the shared library
try:
    chess_lib = ctypes.CDLL('../bin/libchess.so')
except OSError:
    print("Error: Could not load libchess.so from ../bin/. Make sure it exists there.")
    exit(1)

# Define the board type (8x8 array of signed chars)
board_t = ctypes.c_byte * 8 * 8

# Define move_t structure based on your header file
class move_t(ctypes.Structure):
    _fields_ = [
        ("from_row", ctypes.c_int),
        ("from_col", ctypes.c_int),
        ("to_row", ctypes.c_int),
        ("to_col", ctypes.c_int),
        ("piece", ctypes.c_byte),
        ("captured", ctypes.c_byte)
    ]

# Define function signatures
chess_lib.possible_moves.argtypes = [board_t, ctypes.c_int, ctypes.c_int, ctypes.POINTER(move_t), ctypes.c_int]
chess_lib.possible_moves.restype = ctypes.c_int

chess_lib.is_at_check.argtypes = [board_t]
chess_lib.is_at_check.restype = ctypes.c_int

# Chess piece values based on your C code
PIECES = {
    'K': 1,   # King (t == 1)
    'Q': 2,   # Queen (t == 2)
    'R': 3,   # Rook (t == 3)
    'B': 4,   # Bishop (t == 4)
    'N': 5,   # Knight (t == 5)
    'P': 6,   # Pawn (t == 6)
    'k': -1,  # Black King
    'q': -2,  # Black Queen
    'r': -3,  # Black Rook
    'b': -4,  # Black Bishop
    'n': -5,  # Black Knight
    'p': -6,  # Black Pawn
    '.': 0    # Empty square
}

PIECE_CHARS = {v: k for k, v in PIECES.items()}

def create_random_board():
    """Create a random chess board position"""
    board = board_t()
    
    # Initialize empty board
    for i in range(8):
        for j in range(8):
            board[i][j] = 0
    
    # Place some random pieces
    pieces_to_place = ['K', 'Q', 'R', 'R', 'B', 'B', 'N', 'N'] + ['P'] * 8
    pieces_to_place += ['k', 'q', 'r', 'r', 'b', 'b', 'n', 'n'] + ['p'] * 8
    
    # Randomly place some pieces (not all, to make it more interesting)
    num_pieces = random.randint(10, 20)
    selected_pieces = random.sample(pieces_to_place, num_pieces)
    
    positions = [(i, j) for i in range(8) for j in range(8)]
    random.shuffle(positions)
    
    for i, piece in enumerate(selected_pieces):
        if i < len(positions):
            row, col = positions[i]
            board[row][col] = PIECES[piece]
    
    return board

def print_board(board):
    """Print the board in a readable format"""
    print("\n  a b c d e f g h")
    for i in range(8):
        print(f"{8-i} ", end="")
        for j in range(8):
            piece_val = board[i][j]
            piece_char = PIECE_CHARS.get(piece_val, '.')
            print(f"{piece_char} ", end="")
        print(f"{8-i}")
    print("  a b c d e f g h\n")

def square_to_notation(row, col):
    """Convert array indices to chess notation"""
    return f"{chr(ord('a') + col)}{8 - row}"

def get_possible_moves(board, from_row, from_col):
    """Get all possible moves for a piece using the C library"""
    max_moves = 64  # Maximum possible moves on a chessboard
    moves_array = (move_t * max_moves)()
    
    try:
        # Call the C function
        num_moves = chess_lib.possible_moves(board, from_row, from_col, moves_array, max_moves)
        
        if num_moves <= 0:
            return []
        
        # Extract moves from the array
        possible_moves = []
        for i in range(num_moves):
            move = moves_array[i]
            
            # Convert to chess notation
            from_square = square_to_notation(move.from_row, move.from_col)
            to_square = square_to_notation(move.to_row, move.to_col)
            
            # Get piece at destination (for capture info)
            target_piece = board[move.to_row][move.to_col]
            target_char = PIECE_CHARS.get(target_piece, '.')
            
            move_info = {
                'from': from_square,
                'to': to_square,
                'from_coords': (move.from_row, move.from_col),
                'to_coords': (move.to_row, move.to_col),
                'capture': target_piece != 0,
                'target_piece': target_char,
                'piece': PIECE_CHARS.get(move.piece, '?'),
                'captured': PIECE_CHARS.get(move.captured, '.') if move.captured != 0 else None
            }
            possible_moves.append(move_info)
        
        return possible_moves
        
    except Exception as e:
        print(f"Error calling possible_moves: {e}")
        return []

def display_moves(moves):
    """Display the possible moves in a readable format"""
    if not moves:
        print("No possible moves found.")
        return
    
    print(f"Found {len(moves)} possible moves:")
    for i, move in enumerate(moves, 1):
        capture_info = f" (captures {move['target_piece']})" if move['capture'] else ""
        print(f"{i:2d}. {move['from']} -> {move['to']}{capture_info}")

def highlight_board_with_moves(board, moves):
    """Print the board with possible moves highlighted"""
    if not moves:
        print_board(board)
        return
    
    # Create a set of destination squares for quick lookup
    dest_squares = {move['to_coords'] for move in moves}
    
    print("\n  a b c d e f g h")
    for i in range(8):
        print(f"{8-i} ", end="")
        for j in range(8):
            piece_val = board[i][j]
            piece_char = PIECE_CHARS.get(piece_val, '.')
            
            # Highlight possible move destinations
            if (i, j) in dest_squares:
                print(f"[{piece_char}]", end="")
            else:
                print(f" {piece_char} ", end="")
        print(f"{8-i}")
    print("  a b c d e f g h")
    print("Note: [] indicates possible move destinations\n")

def get_user_input():
    """Get square selection from user"""
    while True:
        try:
            user_input = input("Enter square (e.g., 'e4') or 'q' to quit: ").strip().lower()
            if user_input == 'q':
                return None, None
            
            if len(user_input) != 2:
                print("Please enter a valid square (e.g., 'e4')")
                continue
            
            col_char, row_char = user_input[0], user_input[1]
            
            if col_char not in 'abcdefgh' or row_char not in '12345678':
                print("Please enter a valid square (e.g., 'e4')")
                continue
            
            # Convert to array indices
            col = ord(col_char) - ord('a')  # a=0, b=1, ..., h=7
            row = 8 - int(row_char)         # 8=0, 7=1, ..., 1=7
            
            return row, col
        except (ValueError, IndexError):
            print("Please enter a valid square (e.g., 'e4')")

def main():
    print("Chess Library Tester")
    print("===================")
    
    # Create a random board
    board = create_random_board()
    
    while True:
        print_board(board)
        
        # Check if in check
        check_status = chess_lib.is_at_check(board)
        if check_status:
            print("CHECK!")
        
        # Get user input
        row, col = get_user_input()
        if row is None:
            break
        
        # Check if there's a piece at the selected square
        piece_value = board[row][col]
        if piece_value == 0:
            print("No piece at that square!")
            continue
        
        piece_char = PIECE_CHARS.get(piece_value, '?')
        print(f"Selected piece: {piece_char} at {chr(ord('a') + col)}{8 - row}")
        
        # Get and display possible moves
        moves = get_possible_moves(board, row, col)
        
        if moves:
            print(f"\nPossible moves for {piece_char}:")
            display_moves(moves)
            
            print("\nBoard with possible moves highlighted:")
            highlight_board_with_moves(board, moves)
        else:
            print("No possible moves for this piece.")
        
        print("-" * 40)

if __name__ == "__main__":
    main()