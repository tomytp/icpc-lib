import difflib
import sys
import subprocess
import pyperclip
import argparse
from typing import Optional, List, Tuple
from pathlib import Path

BASE_FOLDER = Path(__file__).parent


class TypstFileManager:
    def __init__(self, base_folder: Path):
        self.base_folder = base_folder
        self.src_folder = base_folder / 'src'

    def find_best_matches(self, query: str, library_path: Path, num_results: int = 3) -> List[Tuple[Path, float]]:
        matches = []
        for file in Path(library_path).rglob('*'):
            if file.is_file():
                ratio = difflib.SequenceMatcher(None, query.lower(), file.stem.lower()).ratio()
                matches.append((file, ratio))
        return sorted(matches, key=lambda x: x[1], reverse=True)[:num_results]

    def open_in_vscode(self, file_path: Path):
        file_path = file_path.resolve()
        try:
            subprocess.run(['code', '--goto', str(file_path), str(self.base_folder)])
        except FileNotFoundError:
            print("Error: VSCode command line tool 'code' not found. Please make sure VSCode is installed and the 'code' command is in your PATH.")
            sys.exit(1)

    def get_categories(self) -> List[str]:
        return [d.name for d in self.src_folder.iterdir() if d.is_dir()]

    def create_new_file(self, query: str, category: str) -> Path:
        category_path = self.src_folder / category
        if not category_path.exists():
            print(f"Error: Category '{category}' does not exist")
            sys.exit(1)
        
        new_file = category_path / f"{query}.cpp"
        if new_file.exists():
            print(f"Error: File '{new_file}' already exists")
            sys.exit(1)

        new_file.touch()
        return new_file

    def handle_copy_action(self, query: str, path: Path):
        matches = self.find_best_matches(query, path)
        best_match = matches[0]

        if best_match[1] > 0.9:
            pyperclip.copy(Path(best_match[0]).read_text())
            print(f"Copied {best_match[0].relative_to(self.src_folder)} (score: {best_match[1]:.2f}) to clipboard")
        else:
            self.print_available_matches(matches)

    def handle_edit_action(self, query: str, path: Path):
        matches = self.find_best_matches(query, path)
        best_match = matches[0]
        
        if best_match[1] > 0.9:
            print(f"Opening {best_match[0].relative_to(self.src_folder)} (score: {best_match[1]:.2f}) in VSCode")
            self.open_in_vscode(best_match[0])
        else:
            self.print_available_matches(matches)

    def handle_create_action(self, query: str, path: Path):
        matches = self.find_best_matches(query, path)
        if matches and matches[0][1] > 0.9:
            if not self.confirm_creation(matches[0][0]):
                return

        category = self.select_category()
        if category:
            new_file = self.create_new_file(query, category)
            print(f"\nCreated new file: {new_file.relative_to(self.src_folder)}")
            self.open_in_vscode(new_file)

    def print_available_matches(self, matches: List[Tuple[Path, float]]):
        print("No close matches found. Available files:")
        for file, score in matches:
            print(f"{file.relative_to(self.src_folder)} (score: {int(score*100)})")

    def confirm_creation(self, existing_file: Path) -> bool:
        print(f"Warning: Similar file already exists: {existing_file.relative_to(self.src_folder)}")
        response = input("Do you want to continue creating a new file? (y/N): ")
        return response.lower() == 'y'

    def select_category(self) -> Optional[str]:
        categories = self.get_categories()
        print("\nAvailable categories:")
        for i, category in enumerate(categories, 1):
            print(f"{i}. {category}")
        
        while True:
            try:
                choice = int(input("\nSelect category number: ")) - 1
                if 0 <= choice < len(categories):
                    return categories[choice]
                print("Invalid selection. Please try again.")
            except ValueError:
                print("Please enter a number.")
                
def setup_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(description='Library Navigator')
    parser.add_argument('action', choices=['cp', 'edit', 'create'], help='Action to perform (cp/edit/create)')
    parser.add_argument('query', help='Search query for the file')
    return parser

def main():
    parser = setup_argument_parser()
    args = parser.parse_args()
    
    manager = TypstFileManager(BASE_FOLDER)
    
    action_handlers = {
        'cp': manager.handle_copy_action,
        'edit': manager.handle_edit_action,
        'create': manager.handle_create_action
    }
    
    action_handlers[args.action](args.query, manager.src_folder)

if __name__ == "__main__":
    main()