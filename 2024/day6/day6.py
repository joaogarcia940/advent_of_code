from typing import Optional, Tuple
import numpy as np
from pathlib import Path
from enum import Enum
from utils.file_reader import read_file_as_str_lines


class Direction(Enum):
    UP = "^"
    DOWN = "v"
    LEFT = "<"
    RIGHT = ">"


def file_parser(path: Path) -> np.ndarray:
    return np.array([list(line) for line in read_file_as_str_lines(path)], dtype="<U1")


class MapInterpreter:
    def __init__(self, map: np.ndarray):
        self.map = map

    def initial_guard_position(self) -> np.ndarray:
        possible_guard_representations = ["<", "^", ">", "v"]
        mask = np.isin(self.map, possible_guard_representations)
        positions = np.argwhere(mask)
        assert positions.shape[0] == 1, f"Found {positions.shape[0]} guards, expected 1"
        return tuple(positions[0])

    def initial_guard_direction(self, guard_pos: Tuple[int, int]) -> Direction:
        match self.map[guard_pos]:
            case "^":
                return Direction.UP
            case "v":
                return Direction.DOWN
            case "<":
                return Direction.LEFT
            case ">":
                return Direction.RIGHT
            case _:
                raise ValueError(f"Unknown direction: {self.map[guard_pos]}")

    def obstruction_positions(self):
        # return np.argwhere(self.map == "#")
        possible_obstruction_representations = ["#", "O"]
        mask = np.isin(self.map, possible_obstruction_representations)
        return np.argwhere(mask)

    def next_obstruction(
        self, guard_pos: Tuple[int, int], direction: Direction
    ) -> Tuple[int, int]:
        r, c = guard_pos
        obstructions = self.obstruction_positions()  # array of shape (N, 2)

        if direction == Direction.UP:
            # Keep obstructions in the same column, above the guard
            candidates = obstructions[
                (obstructions[:, 1] == c) & (obstructions[:, 0] <= r)
            ]
            if candidates.size == 0:
                return None
            # Pick the closest one (largest row index less than r)
            closest_row = candidates[:, 0].max()
            return (closest_row, c)

        elif direction == Direction.DOWN:
            candidates = obstructions[
                (obstructions[:, 1] == c) & (obstructions[:, 0] >= r)
            ]
            if candidates.size == 0:
                return None
            closest_row = candidates[:, 0].min()
            return (closest_row, c)

        elif direction == Direction.LEFT:
            candidates = obstructions[
                (obstructions[:, 0] == r) & (obstructions[:, 1] <= c)
            ]
            if candidates.size == 0:
                return None
            closest_col = candidates[:, 1].max()
            return (r, closest_col)

        elif direction == Direction.RIGHT:
            candidates = obstructions[
                (obstructions[:, 0] == r) & (obstructions[:, 1] >= c)
            ]
            if candidates.size == 0:
                return None
            closest_col = candidates[:, 1].min()
            return (r, closest_col)

        raise ValueError(f"Unknown direction: {direction}")

    def turn_right(self, current: Direction) -> Direction:
        match current:
            case Direction.UP:
                return Direction.RIGHT
            case Direction.RIGHT:
                return Direction.DOWN
            case Direction.DOWN:
                return Direction.LEFT
            case Direction.LEFT:
                return Direction.UP
            case _:
                raise ValueError(f"Unknown direction: {current}")

    def next_position(
        self, position: Optional[Tuple[int, int]], direction: Direction
    ) -> Tuple[int, int] | None:
        """
        Returns the next position on the map given the guard's current position and direction.
        Returns None if the next move would go outside the map boundaries.
        """
        if position == None:
            return None

        r, c = position
        max_r, max_c = self.map.shape

        match direction:
            case Direction.UP:
                if r > 0:
                    return (r - 1, c)
            case Direction.DOWN:
                if r < max_r - 1:
                    return (r + 1, c)
            case Direction.LEFT:
                if c > 0:
                    return (r, c - 1)
            case Direction.RIGHT:
                if c < max_c - 1:
                    return (r, c + 1)

        # None if next position is outside map
        return None

    def get_map(self):
        return self.map


class Simulator:
    def __init__(self, map_array: np.ndarray, test_loops: bool = True):
        self.map = map_array
        self.original_map = map_array.copy()  # Save original map state
        self.interpreter = MapInterpreter(self.map)
        self.original_obstructions = (
            self.interpreter.obstruction_positions()
        )  # Cache original obstructions
        self.loop_positions = set()
        self.is_loop = False
        self.test_loops = test_loops  # Whether to test for loop positions

    def run(self):
        guard_pos = self.interpreter.initial_guard_position()
        direction = self.interpreter.initial_guard_direction(guard_pos)

        guard_pos, direction = self.run_until_final_obstruction(guard_pos, direction)

        # this means we got light at the end of the tunnel, but still need to get there
        while (
            next_pos := self.interpreter.next_position(guard_pos, direction)
        ) is not None:
            guard_pos = next_pos
            self.commit_move(guard_pos, direction)

    def run_loop_simulator(self, guard_pos, direction):
        if not self.test_loops:
            return

        next_position = self.interpreter.next_position(guard_pos, direction)
        if next_position is None:
            return
        # Check against original obstructions, not current map state
        if any(
            np.array_equal(next_position, obs) for obs in self.original_obstructions
        ):
            return

        # Create a copy of the ORIGINAL map (not the current state with X's)
        map_copy = self.original_map.copy()
        map_copy[next_position] = "O"

        # The original map should have the guard symbol, but just to be safe,
        # restore it if it's missing
        original_interp = MapInterpreter(self.original_map)
        original_guard_pos = original_interp.initial_guard_position()
        original_guard_dir = original_interp.initial_guard_direction(original_guard_pos)

        # Ensure the guard symbol is in map_copy
        if map_copy[original_guard_pos] != original_guard_dir.value:
            map_copy[original_guard_pos] = original_guard_dir.value

        # Create a new simulator that starts from the beginning, without loop testing
        loop_sim = Simulator(map_copy, test_loops=False)
        loop_sim.run()

        # If the main simulation detected a loop, the obstacle creates a loop
        if loop_sim.is_loop:
            self.loop_positions.add(next_position)

    def commit_move(self, guard_pos, direction):
        self.write_X(guard_pos)
        self.run_loop_simulator(guard_pos, direction)

    def write_X(self, position: Tuple[int, int]):
        self.map[position] = "X"

    def run_until_final_obstruction(self, guard_pos, direction):
        guard_move_set = set()
        while (
            next_obstruction := self.interpreter.next_obstruction(guard_pos, direction)
        ) is not None:
            guard_pos = self.move_until_obstruction(
                guard_pos, direction, next_obstruction
            )
            direction = self.interpreter.turn_right(direction)

            move_key = (tuple(guard_pos), direction)
            if move_key in guard_move_set:
                self.is_loop = True
                break
            guard_move_set.add(move_key)
        return guard_pos, direction

    def move_until_obstruction(
        self,
        guard_pos: Tuple[int, int],
        direction: Direction,
        next_obstruction: Optional[Tuple[int, int]],
    ) -> Tuple[int, int] | None:
        if next_obstruction == None:
            return None
        while True:
            self.commit_move(guard_pos, direction)
            next_pos = self.interpreter.next_position(guard_pos, direction)
            if next_pos == next_obstruction:
                return guard_pos
            guard_pos = next_pos

    def get_result_map(self):
        return self.map

    def get_diff_positions(self):
        return np.sum(self.map == "X")

    def get_diff_loops(self):
        return len(self.loop_positions)


def file_writer(file_path: Path, result: np.ndarray):
    np.savetxt(file_path, result, fmt="%s", delimiter="")


class LoopSimulator(Simulator):
    def __init__(self, map, initial_guard_pos, initial_direction):
        super().__init__(map, test_loops=False)  # Don't test for nested loops
        self.initial_guard_pos = initial_guard_pos
        self.initial_direction = initial_direction
        self.is_loop = False  # Reset the flag for this simulation

    def run(self):
        next_direction = self.interpreter.turn_right(self.initial_direction)
        next_guard_pos = self.interpreter.next_position(
            self.initial_guard_pos, next_direction
        )
        while next_guard_pos == self.interpreter.next_obstruction(
            self.initial_guard_pos, next_direction
        ):
            next_direction = self.interpreter.turn_right(next_direction)
            next_guard_pos = self.interpreter.next_position(
                self.initial_guard_pos, next_direction
            )
        self.run_until_final_obstruction(next_guard_pos, next_direction)

    def commit_move(self, guard_pos, direction):
        # Don't write X's or run nested loop simulators
        pass

    def found_loop(self):
        return self.is_loop


if __name__ == "__main__":
    current_dir = Path(__file__).parent
    map = file_parser(current_dir / "real.txt")
    sim = Simulator(map)
    sim.run()
    file_writer(current_dir / "result.txt", sim.get_result_map())
    print("\nPart 1: Number of different positions is " + str(sim.get_diff_positions()))
    print("\nPart 2: Number of different loops is " + str(sim.get_diff_loops()))
