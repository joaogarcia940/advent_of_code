from pathlib import Path

def read_file_as_str_lines(file_path: str | Path) -> list[str]:
    if isinstance(file_path, str):
        file_path = Path(file_path)

    if not file_path.is_file():
        raise FileNotFoundError(f"No such file: {file_path}")
    
    with open(file_path, "r", encoding="utf-8") as file:
        return [line.strip() for line in file.readlines()]