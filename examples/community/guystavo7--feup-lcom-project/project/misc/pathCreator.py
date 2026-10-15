import csv

TILE_SIZE = 32


def load_map(filename):
    with open(filename, newline="") as f:
        return [[int(cell) for cell in row] for row in csv.reader(f)]


def dfs(grid, visited, x, y, component):

    rows = len(grid)
    cols = len(grid[0])

    if x < 0 or x >= cols or y < 0 or y >= rows:
        return

    if visited[y][x]:
        return

    if grid[y][x] == -1:
        return

    visited[y][x] = True

    # store TILE coordinates (not world yet)
    component.append((x, y))

    dfs(grid, visited, x + 1, y, component)
    dfs(grid, visited, x - 1, y, component)
    dfs(grid, visited, x, y + 1, component)
    dfs(grid, visited, x, y - 1, component)


def tile_to_world_top_left(x, y):
    return (x * TILE_SIZE, y * TILE_SIZE)


def find_branches(grid):
    rows = len(grid)
    cols = len(grid[0])

    visited = [[False] * cols for _ in range(rows)]
    branches = []

    for y in range(rows):
        for x in range(cols):

            # DFS ONLY starts from value == 1
            if grid[y][x] == 1 and not visited[y][x]:
                component = []
                dfs(grid, visited, x, y, component)
                branches.append(component)

    return branches


def convert_branches_to_top_lefts(branches):
    return [
        [tile_to_world_top_left(x, y) for x, y in branch]
        for branch in branches
    ]


def main():
    grid = load_map("./../resources/maps/map3/path.csv")

    branches = find_branches(grid)
    branch_tiles_world = convert_branches_to_top_lefts(branches)

    print(f"Found {len(branches)} branches\n")

    for i, tiles in enumerate(branch_tiles_world):
        print(f"Branch {i}")
        print(f"  Tiles: {len(tiles)}")

        for t in tiles:
            print(f"{float(t[0])}, {float(t[1])}", end=", ");

        print()


if __name__ == "__main__":
    main()
