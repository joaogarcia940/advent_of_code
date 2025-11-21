import unittest
from pathlib import Path
import tempfile
from utils.file_reader import read_file_as_str_lines

def make_arbitrary_test_text() -> list[str]:
    width = 10
    lines = [str(i) * width for i in range(1, 10)]
    lines.append("*" * width)
    lines.append("ã" * width)
    return lines

class TestFileReader(unittest.TestCase):
    def setUp(self):
        # Create a temporary directory for test files
        self.tmp_dir = tempfile.TemporaryDirectory()
        self.tmp_path = Path(self.tmp_dir.name)

    def tearDown(self):
        # Clean up the temporary directory after each test
        self.tmp_dir.cleanup()

    def _write_file(self, lines: list[str]) -> Path:
        test_file = self.tmp_path / "test_file.txt"
        test_file.write_text("\n".join(lines), encoding="utf-8")
        return test_file

    def test_arbitrary_file_read_correctly(self):
        text_in = make_arbitrary_test_text()
        file_path = self._write_file(text_in)
        text_out = read_file_as_str_lines(file_path)
        self.assertListEqual(text_in, text_out)

    def test_given_element_in_file_expect_only_those_in_output(self):
        text_in = ["ã", "*", "ç"]
        file_path = self._write_file(text_in)
        text_out = read_file_as_str_lines(file_path)
        self.assertIn("ç", text_out)
        self.assertNotIn("õ", text_out)

    def test_file_not_found(self):
        fake_path = Path("/this/path/does/not/exist.txt")
        with self.assertRaises(FileNotFoundError):
            read_file_as_str_lines(fake_path)

    def test_string_and_path_input_work_the_same(self):
        file_path = self._write_file(make_arbitrary_test_text())
        text_out_file_path = read_file_as_str_lines(file_path)
        text_out_string = read_file_as_str_lines(str(file_path))
        self.assertListEqual(text_out_file_path, text_out_string)

if __name__ == "__main__":
    unittest.main()
