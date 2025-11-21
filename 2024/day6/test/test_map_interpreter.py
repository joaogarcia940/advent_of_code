import unittest
import numpy as np
from pathlib import Path
from day6.day6 import MapInterpreter, file_parser, Direction

class TestMapInterpreter(unittest.TestCase):
    def setUp(self):
        # Example map as string lines
        self.map_lines = [
            "....#.....",
            ".........#",
            "..#.......",
            ".......#..",
            "..........",
            ".#..^.....",
            "........#.",
            "#.........",
            "......#..."
        ]
        # Write to a temporary file
        self.test_file = Path("test_map.txt")
        self.test_file.write_text("\n".join(self.map_lines), encoding="utf-8")

        # Create numpy map
        self.map_array = file_parser(self.test_file)
        self.interpreter = MapInterpreter(self.map_array)

    def tearDown(self):
        # Clean up the temporary file
        if self.test_file.exists():
            self.test_file.unlink()

    def test_file_parser(self):
        parsed = file_parser(self.test_file)
        self.assertIsInstance(parsed, np.ndarray)
        self.assertEqual(parsed.shape, (len(self.map_lines), len(self.map_lines[0])))

    def test_guard_position(self):
        pos = self.interpreter.initial_guard_position()
        # Verify the character at position is '^'
        self.assertEqual(self.map_array[pos], "^")

    def test_guard_direction(self):
        pos = self.interpreter.initial_guard_position()
        direction = self.interpreter.initial_guard_direction(pos)
        self.assertEqual(direction, Direction.UP)

    def test_obstruction_positions(self):
        obstacles = self.interpreter.obstruction_positions()
        self.assertTrue(np.all(self.map_array[tuple(obstacles[0])] == "#"))
        # Check that there are multiple obstacles
        self.assertEqual(obstacles.shape[0], 8)

    def test_invalid_guard_direction(self):
        # Put a wrong character at a guard position
        self.map_array[5, 4] = "X"
        with self.assertRaises(ValueError):
            self.interpreter.initial_guard_direction((5, 4))


if __name__ == "__main__":
    unittest.main()
