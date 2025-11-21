"""
Where to even begin with this one?
Is it ugly? Oh my god yes.
Is it slow? Absolutely: takes about 2 minutes to complete.
Does it produce the correct answer? After working on this for SO MANY hours.... YES, it does work.

I am pretty frustrated that it took this long and is essentially just the same idea I had to begin with:
Run two simulations: one simulates the guard's path from the starting position until it either gets
stuck in a loop or goes out of bounds, and one that simulates adding an obstacle at each point along
the guard's normal uninterrupted path.

I couldn't figure out a way to do this without running TWO recursive loops, one nested inside the other.
The outer loop controls the blocking simulation and ends when the next block position would be out of bounds.
The inner loop controls the guard and ends when it detects a loop or goes out of bounds.

It sounds conceptually simple but I found implementing it to be a huge pain in the ass and I have
to imagine there are better ways. I cannot understand how some people's solutions run in milliseconds.

The only other idea I had was to look for squares, but a loop can be any shape so that doesn't work.

Time to move on, I guess. Usually I go back and beautify my code a bit but this time I think I'm going
to leave it as-is. 
"""

import os
from collections import namedtuple
from time import sleep
from functools import reduce
import sys

sys.setrecursionlimit(10 ** 5)

Pos = namedtuple("Position", ["x", "y", "d"])


class Guard:
    def __init__(self, file_name, draw: bool = False):
        self.out_of_bounds: bool = False
        self.file_name = file_name
        self.grid: Guard.Grid = self.Grid(file_name)
        self.starting_pos: Pos = self.grid.find_starting_position()
        self.current_pos: Pos = self.grid.find_starting_position()
        self.directions: tuple = ((0, -1), (1, 0), (0, 1), (-1, 0))
        self.steps_taken: list = []
        self.draw = draw
        self.in_a_loop = False
    
    def patrol(self) -> bool:
        while not self.out_of_bounds:
            print(self.current_pos)
            if self.draw: self.grid.draw(self)
            self.take_step(self.get_next_position())
                

    def get_next_position(self) -> Pos:
        # needs to keep checking next position and turning if it hits a corner
        dx = self.directions[self.current_pos.d][0]
        dy = self.directions[self.current_pos.d][1]
        next_char = self.grid.modified[self.current_pos.y + dy][self.current_pos.x + dx]
        while next_char == "#":
            self.change_direction()
            dx = self.directions[self.current_pos.d][0]
            dy = self.directions[self.current_pos.d][1]
            next_char = self.grid.modified[self.current_pos.y + dy][self.current_pos.x + dx]
        return Pos(self.current_pos.x + dx, self.current_pos.y + dy, self.current_pos.d)
    
    def take_step(self, next_pos: Pos) -> Pos:
        self.current_pos = next_pos
        
        if self.current_pos in self.steps_taken:
            self.in_a_loop = True

        if self.grid.modified[next_pos.y][next_pos.x] == "*":
            self.out_of_bounds = True
        
        self.steps_taken.append(self.current_pos)
        return self.current_pos
    
    def change_direction(self):
        # this just cycles through a list 0 -> 1 -> 2 -> 3 -> 0 -> 1...
        new_direction = (self.current_pos.d + 1) % len(self.directions)
        self.current_pos = Pos(self.current_pos.x, self.current_pos.y, new_direction)

    class Grid:
        def __init__(self, file_name):
            self.default: list
            self.modified: list
            self.file_name = file_name
        
        def add_obstacle(self, position: Pos) -> list:
            self.modified = self.default.copy()
            x, y, _ = position
            line = list(self.modified[y])
            line[x] = "#"
            line = "".join(line)
            self.modified[y] = line
            return self.modified

        def generate_grid(self) -> list:
            with open(self.file_name) as file:
                grid = ["*" + line.strip() + "*" for line in file.readlines()]
                pad_row = "*" * len(grid[0])
                grid = [pad_row] + grid + [pad_row]
                return grid

        def find_starting_position(self, grid: list = None) -> Pos:
            if grid == None:
                grid = self.generate_grid()
            for y, row in enumerate(grid):
                for x, col in enumerate(row):
                    if col == "^":
                        starting_pos = Pos(x, y, 0)   # 0 is the default direction (up)
                        # remove the ^ from the grid just to avoid any confusion later
                        line = grid[y]
                        line = list(line)
                        line[x] = "."
                        line = "".join(line)
                        grid[y] = line
                        self.modified = self.default = grid
            return starting_pos

        def draw(self, guard, dt = .15):
            # for testing to see what was happening on-screen
            os.system("cls")
            gx = guard.current_pos.x
            gy = guard.current_pos.y
            sprites = ("^", ">", "v", "<")
            gs = sprites[guard.current_pos.d]
            grid = guard.grid.modified.copy()
            line = grid[gy]
            line = list(line)
            line[gx] = gs
            line = "".join(line)
            grid[gy] = line
            for line in grid:
                print(line)
            print(f"{guard.in_a_loop}")
            print(f"{guard.current_pos}")
            print(f"{guard.steps_taken}")
            sleep(dt)


def find_loops(test: Guard, control: Guard, patrol: int = 0, loop_pos: list = []):
    # control starts off one step ahead of test. if it's out of bounds then we're done
    if control.out_of_bounds:
        return loop_pos
    # set an obstacle in test's path based on where control is
    obstacle_pos = control.current_pos
    test.grid.add_obstacle(obstacle_pos)

    # run the test simulation
    def inner(guard: Guard):
        if guard.draw: guard.grid.draw(guard)
        if guard.in_a_loop:
            # only append the x, y coordinates of an obstacle if it creates a loop.
            # this is to avoid duplicate obstacles where the obstacle is in the same
            # position but the guard encounters it heading in different directions.
            loop_pos.append((obstacle_pos.x, obstacle_pos.y))
            return True
        elif guard.out_of_bounds:
            return False
        else:
            guard.take_step(guard.get_next_position())
            return inner(guard)

    patrol = inner(test)
    # move control forward, reset test
    control.take_step(control.get_next_position())
    new_test = Guard(control.file_name, test.draw)
    
    return find_loops(new_test, control, patrol, loop_pos)

file_name = "/home/joao/workspace/advent_of_code/2024/day6/real.txt"
test = Guard(file_name)
control = Guard(file_name)
control.take_step(control.get_next_position())  # set the control to be one step ahead of the test guard

loops = find_loops(test, control)
print(len(set(loops)))