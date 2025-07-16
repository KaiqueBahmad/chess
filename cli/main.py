import ctypes
import numpy as np

class ChessLibrary:
    """Python wrapper for the chess C library"""
    
    def __init__(self, lib_path='../bin/libchess.so'):
        """Initialize the chess library wrapper"""
        self.lib = ctypes.CDLL(lib_path)
        self._setup_function_signatures()
        
    def _setup_function_signatures(self):
        """Configure C function signatures for ctypes"""
        # Board type: 8x8 signed char array (flattened to 64 bytes)
        self.BoardType = ctypes.c_byte * 64
        
        # possible_moves(board, row, col) -> int
        self.lib.possible_moves.argtypes = [self.BoardType, ctypes.c_int, ctypes.c_int]
        self.lib.possible_moves.restype = ctypes.c_int
        
        # is_at_check(board) -> int
        self.lib.is_at_check.argtypes = [self.BoardType]
        self.lib.is_at_check.restype = ctypes.c_int
        
        # generate_random_position(board) -> void
        self.lib.generate_random_position.argtypes = [self.BoardType]
        self.lib.generate_random_position.restype = None
        
        # simple_benchmark(num_positions) -> void
        self.lib.simple_benchmark.argtypes = [ctypes.c_int]
        self.lib.simple_benchmark.restype = None
    
    def create_board(self, board_2d=None):
        """Create a board from 2D numpy array or empty board"""
        board_1d = self.BoardType()
        
        if board_2d is not None:
            if board_2d.shape != (8, 8):
                raise ValueError("Board must be 8x8")
            flat = board_2d.flatten().astype(np.int8)
            for i in range(64):
                board_1d[i] = flat[i]
        else:
            # Initialize empty board
            for i in range(64):
                board_1d[i] = 0
                
        return board_1d
    
    def board_to_numpy(self, board_1d):
        """Convert ctypes board to 2D numpy array"""
        return np.frombuffer(board_1d, dtype=np.int8).reshape(8, 8)
    
    def numpy_to_board(self, board_2d):
        """Convert 2D numpy array to ctypes board"""
        return self.create_board(board_2d)
    
    def generate_random_position(self, board=None):
        """Generate a random chess position"""
        if board is None:
            board = self.create_board()
        
        self.lib.generate_random_position(board)
        return board
    
    def possible_moves(self, board, row, col):
        """Get number of possible moves for piece at (row, col)"""
        if not (0 <= row < 8 and 0 <= col < 8):
            raise ValueError("Position must be within 0-7 range")
        
        return self.lib.possible_moves(board, row, col)
    
    def is_at_check(self, board):
        """Check if the current position is in check"""
        result = self.lib.is_at_check(board)
        return bool(result)
    
    def simple_benchmark(self, num_positions=1000):
        """Run performance benchmark on random positions"""
        print(f"Running benchmark with {num_positions} positions...")
        self.lib.simple_benchmark(num_positions)
    
    def print_board(self, board, piece_symbols=None):
        """Print board in a readable format"""
        if piece_symbols is None:
            # Default piece representation
            piece_symbols = {
                0: '.',   # Empty
                1: 'P',   # White pawn
                2: 'N',   # White knight
                3: 'B',   # White bishop
                4: 'R',   # White rook
                5: 'Q',   # White queen
                6: 'K',   # White king
                -1: 'p',  # Black pawn
                -2: 'n',  # Black knight
                -3: 'b',  # Black bishop
                -4: 'r',  # Black rook
                -5: 'q',  # Black queen
                -6: 'k'   # Black king
            }
        
        board_2d = self.board_to_numpy(board)
        print("  a b c d e f g h")
        for i in range(8):
            print(f"{8-i} ", end="")
            for j in range(8):
                piece = board_2d[i][j]
                symbol = piece_symbols.get(piece, str(piece))
                print(f"{symbol} ", end="")
            print(f" {8-i}")
        print("  a b c d e f g h")

# Convenience functions
def create_chess_library(lib_path='../bin/libchess.so'):
    """Create and return a ChessLibrary instance"""
    return ChessLibrary(lib_path)

# Example usage
if __name__ == "__main__":
    # Initialize the library
    chess = ChessLibrary()
    
    # Test basic functionality
    print("Testing chess library...")
    
    # Create a random position
    board = chess.generate_random_position()
    print("\nGenerated random position:")
    chess.print_board(board)
    
    # Test possible moves for different positions
    # print("\nTesting possible moves:")
    # for row in range(2):
    #     for col in range(4):
    #         moves = chess.possible_moves(board, row, col)
    #         print(f"Position ({row},{col}): {moves} possible moves")
    
    # # Check if in check
    # in_check = chess.is_at_check(board)
    # print(f"\nPosition is in check: {in_check}")
    
    # # Run benchmark
    # print("\nRunning benchmark:")
    # chess.simple_benchmark(100)
    
    # # Convert to numpy for analysis
    # board_numpy = chess.board_to_numpy(board)
    # print(f"\nBoard as numpy array shape: {board_numpy.shape}")
    # print(f"Board data type: {board_numpy.dtype}")
    
    # # Create custom board
    # custom_board = np.zeros((8, 8), dtype=np.int8)
    # custom_board[0, 0] = 4  # White rook
    # custom_board[7, 7] = -4  # Black rook
    
    # custom_board_c = chess.numpy_to_board(custom_board)
    # print("\nCustom board:")
    # chess.print_board(custom_board_c)